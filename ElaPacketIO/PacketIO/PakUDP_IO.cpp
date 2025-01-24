#include "PakUDP_IO.h"

#include <cassert>

#include "GenBufOManaged.h"
#include "GenUDP_Connection.h"
#include "GenUDP_IO.h"
#include "PakI.h"
#include "PakO.h"
#include "PakPacket.h"
#include "PakProcessor.h"
#include "PakSerialize.h"

PakUDP_IO::PakUDP_IO(GenUDP_Connection* aConnection, PakProcessor* aProcessorPtr, PakHeader* aHeaderType)
    : PakSocketIO(aHeaderType), mConnectionPtr(aConnection), mProcessorPtr(aProcessorPtr), mSerializeWriter(new PakO(&mBufO)), mSerializeReader(new PakI(&mBufI)), mHasReadHeader(false)
{
    mHeaderSize = GetHeaderSize();
    assert(mHeaderSize != 0);
    mBufI.SetBigEndian();
    mBufO.SetBigEndian();
}

PakUDP_IO::~PakUDP_IO()
{
    delete mSerializeWriter;
    delete mSerializeReader;
    delete mConnectionPtr;
}

//! send a packet.
//! @param aPkt The PakPacket to send.
//! @return 'true' if successfully sent.
bool PakUDP_IO::Send(const PakPacket& aPkt)
{
    mBufO.GetPutPos() += mHeaderSize;
    PakProcessor::PacketInfo* info = mProcessorPtr->GetPacketInfo(aPkt.ID());
    // should be constant operation...
    (*info->mWriteFn)(const_cast<PakPacket&>(aPkt), *mSerializeWriter);

    int length = (int)mBufO.GetPutPos();
    mBufO.SetPutPos(0);
    SetPacketHeader(mBufO, aPkt.ID(), length);
    mBufO.SetPutPos(length);
    mConnectionPtr->SendBuffer(mBufO.GetBuffer(), length);
    mBufO.Reset();
    return true;
}

//! Receive a PakPacket header from the UDP_IO
//! @param aPacketId The ID of the incoming packet
//! @param aPacketLength The length of the incoming packet
//! @param aWaitTimeMicroSeconds The time to wait for more incoming data.
//! @return 'true' if a PakPacket header was read
bool PakUDP_IO::ReceiveHeader(int& aPacketId, int& aPacketLength, int aWaitTimeMicroSeconds)
{
    if (!mHasReadHeader)
    {
        mBufI.Reset();
        int bytes = mConnectionPtr->ReceiveBuffer(aWaitTimeMicroSeconds, mBufI.GetBuffer(), (int)mBufI.GetBytes());
        if (bytes > 0)
        {
            mBufI.SetPutPos(bytes);
            bool headerValid;
            mHasReadHeader = GetPacketHeader(mBufI, mHeaderPacketId, mHeaderPacketLength, headerValid);
            mHasReadHeader = mHasReadHeader && headerValid;
            if (!mHasReadHeader)
            {
                mBufI.Reset();
            }
        }
    }
    if (mHasReadHeader)
    {
        aPacketId = mHeaderPacketId;
        aPacketLength = mHeaderPacketLength;
    }
    return mHasReadHeader;
}

//! Receives a packet.  Must follow a successful ReceiveHeader() call
//! @param aPkt The PakPacket to receive.  Must be the correct (verify using ReceiveHeader() )
//! @return 'true' if the PakPacket was successfully read.
bool PakUDP_IO::Receive(PakPacket& aPkt)
{
    bool lReturn = false;
    if (mHasReadHeader)
    {
        PakProcessor::PacketInfo* info = mProcessorPtr->GetPacketInfo(aPkt.ID());
        // should be constant operation...
        (*info->mReadFn)(const_cast<PakPacket&>(aPkt), *mSerializeReader);
        mHasReadHeader = false;
        lReturn = true;
    }
    return lReturn;
}
// virtual
void PakUDP_IO::IgnorePacket()
{
    mHasReadHeader = false;
}

GenSockets::GenSocket* PakUDP_IO::GetRecvSocket() const
{
    return mConnectionPtr->GetRecvSocket();
}

GenSockets::GenSocket* PakUDP_IO::GetSendSocket() const
{
    return mConnectionPtr->GetSendSocket();
}

PakPacket* PakUDP_IO::ReceiveNew()
{
    return mProcessorPtr->ReadPacket(*this);
}
