#include "PakTCP_IO.h"

#include <cassert>
#include <iostream>

#include "GenTCP_Connection.h"
#include "PakI.h"
#include "PakO.h"
#include "PakPacket.h"
#include "PakProcessor.h"
#include "PakSerialize.h"

static std::mutex sendMutex;
static std::mutex receiveMutex;
PakTCP_IO::PakTCP_IO(GenTCP_Connection* aConnectionPtr, PakProcessor* aProcessor, PakHeader* aHeaderType)
    : PakSocketIO(aHeaderType), mPakProcessorPtr(aProcessor), mConnectionPtr(aConnectionPtr), mHasReadHeader(false), mPacketReadyToRead(false), mManualFlushCount(0)
{
    // TCP communication requires a header
    mHeaderSize = GetHeaderSize();
    assert(mHeaderSize != 0);
    mSendBufferSize = 100000;
    mMaximumPacketSize = (int)(mSendBufferSize / 3.0);
    mBufI.SetBigEndian();
    mBufO.SetBigEndian();

    mSerializeWriter = new PakO(&mBufO);
    mSerializeReader = new PakI(&mBufI);
}

PakTCP_IO::~PakTCP_IO()
{
    delete mConnectionPtr;
    delete mSerializeWriter;
    delete mSerializeReader;
}

//! send a packet.
//! @param aPkt The PakPacket to send.
//! @return 'true' if successfully sent.
//! @return 'false' if not successfully sent.
bool PakTCP_IO::Send(const PakPacket& aPkt)
{
    return Send(aPkt, cLARGE_WAIT_TIME);
}

//! send a packet.
//! @param aPkt The PakPacket to send.
//! @return 'true' if successfully sent.
//! @return 'false' if not successfully sent.
//! @note A false return value can mean either the connection no longer exists
//!       or not enough time has passed to complete the Flush() operation.
//!       In this case, If IsConnected() == true,
//!         Flush() should be called until it returns true
bool PakTCP_IO::Send(const PakPacket& aPkt, int aWaitTimeMicroSeconds)
{
    std::lock_guard<std::mutex> guard(sendMutex);
    size_t packetOffset = mBufO.GetPutPos();
    // leave space for header to be inserted later
    mBufO.SetPutPos(packetOffset + mHeaderSize);
    {
        PakProcessor::PacketInfo* info = mPakProcessorPtr->GetPacketInfo(aPkt.ID());
        assert(info); // assert that packet is registered
        // This should be a const operation for aPkt
        (*info->mWriteFn)(const_cast<PakPacket&>(aPkt), *mSerializeWriter);
    }
    size_t endOfPacketOffset = mBufO.GetPutPos();
    size_t packetLength = endOfPacketOffset - packetOffset;
    mBufO.SetPutPos(packetOffset);

    // now write header with correct length info.
    SetPacketHeader(mBufO, aPkt.ID(), (int)packetLength);
    mBufO.SetPutPos(endOfPacketOffset);

    bool ok = true;
    if (mManualFlushCount != 0)
    {
        if (mMaximumPacketSize > (mSendBufferSize - (int)endOfPacketOffset))
        {
            ok = Flush(aWaitTimeMicroSeconds);
        }
    }
    else
    {
        ok = Flush(aWaitTimeMicroSeconds);
    }
    return ok;
}

// Method for sending a buffer of data.
// Try to use the type-safe version of this method.
bool PakTCP_IO::Send(char* aBuffer, int aSize, int aPacketId, int aWaitTimeMicroSeconds /* = cLARGE_WAIT_TIME*/)
{
    std::lock_guard<std::mutex> guard(sendMutex);
    size_t totalBytes = aSize + mHeaderSize;
    mBufO.PutRaw(aBuffer, totalBytes);
    size_t endOfPacketOffset = mBufO.GetPutPos();
    SetPacketHeader(mBufO, aPacketId, (int)totalBytes);
    mBufO.SetPutPos(endOfPacketOffset);

    bool ok = true;
    if (mManualFlushCount != 0)
    {
        if (mMaximumPacketSize > ((int)mSendBufferSize - (int)endOfPacketOffset))
        {
            ok = Flush(aWaitTimeMicroSeconds);
        }
    }
    else
    {
        ok = Flush(aWaitTimeMicroSeconds);
    }
    return ok;
}

//! Sends any buffered packets immediately
//! This is automatically called unless you use BeginManualFlush()
//! @return 'false' if there was a problem sending the data
//! @note A 'false' return means either the connection is closed, or
//! not enough time has passed to send the data.
//! If IsConnected() == true, call Flush() until it returns true.
bool PakTCP_IO::Flush(int aWaitTimeInMicroSec)
{
    if (!sendMutex.try_lock())
    {
        return false;
    }
    // send the packet, loop until PakPacket is sent.
    // This can cause the program to pause if the destination
    // side is frozen -- but this behavior is useful for debugging.
    int totalBytes = (int)mBufO.GetPutPos() - (int)mBufO.GetGetPos();
    if (totalBytes > 0)
    {
        int bytes = mConnectionPtr->SendBuffer(aWaitTimeInMicroSec, mBufO.GetBuffer() + mBufO.GetGetPos(), totalBytes);
        if (bytes > 0)
        {
            totalBytes -= bytes;
            mBufO.SetGetPos(mBufO.GetGetPos() + bytes);
        }
        while (totalBytes > 0 && mConnectionPtr->IsConnected())
        {
            bytes = mConnectionPtr->SendBuffer(aWaitTimeInMicroSec, mBufO.GetBuffer() + mBufO.GetGetPos(), totalBytes);
            if (bytes > 0)
            {
                totalBytes -= bytes;
                mBufO.SetGetPos(mBufO.GetGetPos() + bytes);
            }
            else
            {
                break;
            }
        }
        if (mBufO.GetValidBytes() == 0)
        {
            mBufO.Reset();
        }
    }
    sendMutex.unlock();
    return totalBytes == 0;
}

GenSockets::GenSocket* PakTCP_IO::GetRecvSocket() const
{
    return mConnectionPtr->GetSocket();
}

GenSockets::GenSocket* PakTCP_IO::GetSendSocket() const
{
    return mConnectionPtr->GetSocket();
}

//! Receive a PakPacket header from the stream.
//! @param aPacketId The ID of the incoming packet
//! @param aPacketLength The length of the incoming packet
//! @param aWaitTimeMicroSeconds The time to wait for more incoming data.
//! @return 'true' if a PakPacket header was read, and the rest of the
//!         PakPacket is ready to be read
bool PakTCP_IO::ReceiveHeader(int& aPacketId, int& aPacketLength, int aWaitTimeMicroSeconds)
{
    if (!mHasReadHeader)
    {
        size_t validBytes = mBufI.GetValidBytes();
        bool readyToReadHeader = (mHeaderSize <= (int)validBytes);
        if (!readyToReadHeader)
        {
            if (validBytes == 0)
            {
                mBufI.Reset();
                mConnectionPtr->ReceiveBuffer(aWaitTimeMicroSeconds, mBufI);
            }
            else
            {
                // Not enough room in the buffer for a header
                if ((int)mBufI.GetBytes() - (int)mBufI.GetGetPos() < mHeaderSize)
                {
                    mBufI.Move(mBufI.GetGetPos(), mBufI.GetPutPos(), 0);
                    mBufI.SetPutPos(mBufI.GetPutPos() - mBufI.GetGetPos());
                    mBufI.SetGetPos(0);
                }
                mConnectionPtr->ReceiveBuffer(aWaitTimeMicroSeconds, mBufI);
            }
            readyToReadHeader = (mHeaderSize <= (int)mBufI.GetValidBytes());
        }

        bool headerValid;
        if (readyToReadHeader && GetPacketHeader(mBufI, mHeaderPacketId, mHeaderPacketLength, headerValid))
        {
            if (!headerValid)
            {
                mBufI.GetGetPos() += mHeaderPacketLength;
            }
            else
            {
                int remainderSpace = (int)mBufI.GetBytes() - mHeaderPacketLength;
                if (remainderSpace < 0)
                {
                    mBufI.GrowBy(-remainderSpace);
                }
                assert(mHeaderPacketLength <= (int)mBufI.GetBytes());
                mHasReadHeader = true;
            }
        }
    }
    if (mHasReadHeader)
    {
        mPacketReadyToRead = PacketReadyToRead(aWaitTimeMicroSeconds);
        aPacketId = mHeaderPacketId;
        aPacketLength = mHeaderPacketLength;
    }
    return mPacketReadyToRead;
}

//! Receives a packet.  Must follow a successful ReceiveHeader() call
//! @param aPkt The PakPacket to receive.  Must be the correct (verify using ReceiveHeader() )
//! @return 'true' if the PakPacket was successfully read.
bool PakTCP_IO::Receive(PakPacket& aPkt)
{
    bool isLocked = receiveMutex.try_lock();
    bool lReturn = false;
    if (mPacketReadyToRead)
    {
        int beforeOff = (int)mBufI.GetGetPos();

        PakProcessor::PacketInfo* info = mPakProcessorPtr->GetPacketInfo(aPkt.ID());
        (*info->mReadFn)(aPkt, *mSerializeReader);

        int afterOff = (int)mBufI.GetGetPos();

        if (afterOff - beforeOff + mHeaderSize != mHeaderPacketLength)
        {
            { // RAII block
                std::cout << "Detected error receiving packet."
                          << " Name: " << info->GetPacketName()
                          << " ID: " << aPkt.ID()
                          << " Expected: " << (afterOff - beforeOff) + mHeaderSize << " bytes"
                          << " Received: " << mHeaderPacketLength << " bytes"
                          << std::endl;
            }
            if (isLocked)
            {
                receiveMutex.unlock();
            }
            exit(1);
        }
        beforeOff = afterOff = 0;
        mHasReadHeader = false;
        mPacketReadyToRead = false;
        lReturn = true;
    }
    if (isLocked)
    {
        receiveMutex.unlock();
    }
    return lReturn;
}

// Deprecated method for receiving certain packet types
bool PakTCP_IO::Receive(char* aBuffer, int aSize)
{
    std::lock_guard<std::mutex> guard(receiveMutex);
    bool lReturn = false;
    if (mPacketReadyToRead)
    {
        mBufI.GetRaw(aBuffer, aSize);
        lReturn = true;
    }
    return lReturn;
}

//! Returns a new received packet if one is available, null otherwise
PakPacket* PakTCP_IO::ReceiveNew()
{
    std::lock_guard<std::mutex> guard(receiveMutex);
    return mPakProcessorPtr->ReadPacket(*this);
}

//! Returns true if the connection is still valid
bool PakTCP_IO::IsConnected()
{
    return mConnectionPtr->IsConnected();
}

// virtual
//! After calling ReceiveHeader, this function will discard the packet.
void PakTCP_IO::IgnorePacket()
{
    if (mPacketReadyToRead)
    {
        mBufI.GetGetPos() += mHeaderPacketLength - mHeaderSize;
        mHasReadHeader = false;
        mPacketReadyToRead = false;
    }
}

// NOTE: aWaitTimeMicroSeconds is an approximation.
bool PakTCP_IO::PacketReadyToRead(int aWaitTimeMicroSeconds)
{
    int totalBytes = (int)mBufI.GetPutPos();
    int readBytes = (int)mBufI.GetGetPos();
    int remainingBytes = totalBytes - readBytes;
    int pktSize = mHeaderPacketLength - mHeaderSize;
    int pktRemainingBytes = pktSize - remainingBytes;
    int bytesRead = 1;

    while (pktRemainingBytes > 0 && bytesRead > 0)
    {
        int unusedBytes = (int)(mBufI.GetBytes() - mBufI.GetPutPos());
        if (pktRemainingBytes > unusedBytes)
        {
            size_t beg = mBufI.GetGetPos();
            mBufI.Move(beg, mBufI.GetPutPos(), 0);
            mBufI.SetGetPos(0);
            mBufI.SetPutPos(mBufI.GetPutPos() - beg);
            bytesRead = mConnectionPtr->ReceiveBuffer(aWaitTimeMicroSeconds, mBufI);
        }
        else
        {
            bytesRead = mConnectionPtr->ReceiveBuffer(aWaitTimeMicroSeconds, mBufI, pktRemainingBytes);
        }
        pktRemainingBytes -= bytesRead;
    }
    return pktRemainingBytes <= 0;
}
