#include "GenTCP_IO.h"

#include <cstddef> // for size_t
#include <cstring>
#include <iostream>

#include "GenBufIManaged.h"
#include "GenBufOManaged.h"
#include "GenHostName.h"
#include "GenIP.h"
#include "GenInternetAddress.h"
#include "GenInternetSocketAddress.h"
#include "GenSocket.h"

GenTCP_IO::GenTCP_IO(int aSendBufferSize /*=100000*/, int aRecvBufferSize /*=100000*/)
    : GenTCP_Connection(), GenNetIO("GenTCP_IO", "GenTCP_IO"), mBufferBytesSent(0)
{
    mRecvBuf = new GenBufIManaged(GenBuf::BigEndian, aRecvBufferSize, true);
    SetGenBufXferI(mRecvBuf);
    mSendBuf = new GenBufOManaged(GenBuf::BigEndian, aSendBufferSize, 1);
    SetGenBufXferO(mSendBuf);
}

GenTCP_IO::~GenTCP_IO()
{
    delete mRecvBuf;
    delete mSendBuf;
}

// virtual
//! Fill buffer with an incoming message
//! @param aWaitTimeInMicroSec The Time in microseconds to wait for incoming messages
//! @return The number of received bytes
int GenTCP_IO::Receive(int aWaitTimeInMicroSec)
{
    mRecvBuf->Reset();
    mRecvBuf->SetNumValidBytes(0);
    ClearInputErrorStatus();
    int bytes = ReceiveBuffer(aWaitTimeInMicroSec,
                              reinterpret_cast<char*>(mRecvBuf->GetBuffer()),
                              static_cast<size_t>(mRecvBuf->GetTotalBytes()));
    if (bytes > 0)
    {
        mRecvBuf->SetNumValidBytes(bytes);
    }
    return bytes;
}

// virtual
//! Fill the buffer with the incomming message -- without clearing
//! the last received data.
//! @param aWaitTimeInMicroSec The Time in microseconds to wait for incomming messages
//! @return The number of received bytes
int GenTCP_IO::ReceiveContinue(int aWaitTimeInMicroSec)
{
    int nPreReadBytes = static_cast<int>(mRecvBuf->GetNumValidBytes());
    int bytes = ReceiveBuffer(aWaitTimeInMicroSec,
                              reinterpret_cast<char*>(mRecvBuf->GetBuffer() + nPreReadBytes),
                              static_cast<int>(mRecvBuf->GetTotalBytes()) - nPreReadBytes);
    if (bytes > 0)
    {
        mRecvBuf->SetNumValidBytes(bytes + mRecvBuf->GetNumValidBytes());
    }
    return bytes;
}

// virtual
//! Fill the buffer with the incomming message -- without clearing
//! the last received data.  This is good for TCP communication
//! because the stream based architecture requires the receiver
//! to manually find the message boundaries.
//! @param aWaitTimeInMicroSec The Time in microseconds to wait for incomming messages
//! @param aBytesToReceive The maximum number of bytes to receive.
//! @return The number of received bytes
int GenTCP_IO::ReceiveContinueN(int aWaitTimeInMicroSec, int aBytesToReceive)
{
    int bytes = ReceiveBuffer(aWaitTimeInMicroSec,
                              reinterpret_cast<char*>(mRecvBuf->GetBuffer() + mRecvBuf->GetNumValidBytes()),
                              aBytesToReceive);
    if (bytes > 0)
    {
        mRecvBuf->SetNumValidBytes(bytes + mRecvBuf->GetNumValidBytes());
    }
    return bytes;
}

// virtual
//! Ignores a number of bytes in the receive buffer.
//! @aNumberBytes The number of bytes to ignore.
void GenTCP_IO::IgnoreBytes(int aNumberBytes)
{
    int bytesLeft = static_cast<int>(mRecvBuf->GetNumValidBytes()) - aNumberBytes;
    if (bytesLeft <= 0)
    {
        mRecvBuf->Reset();
    }
    else
    {
        mRecvBuf->SetGetOffset(aNumberBytes, GenBuf::FromCur);
    }
}

//! send data in the send buffer across the TCP stream
//! @return The number of bytes successfully sent or a negative
//!         number of ErrorTypes
//!         If the entire buffer was not sent, send() must be
//!         called again until the number of valid bytes in the buffer
//!         is 0.
int GenTCP_IO::Send()
{
    return Send(0);
}

//! send data in the send buffer across the TCP stream
//! @return The number of bytes successfully sent or a negative
//!         number of ErrorTypes
//!         If the entire buffer was not sent, send() must be
//!         called again until the number of valid bytes in the buffer
//!         is 0.
int GenTCP_IO::Send(int aWaitTimeInMicroSec)
{
    ClearOutputErrorStatus();
    int bytesToSend = static_cast<int>(mSendBuf->GetNumValidBytes()) - mBufferBytesSent;
    int bytesSent =
        SendBuffer(aWaitTimeInMicroSec, reinterpret_cast<const char*>(mSendBuf->GetBuffer() + mBufferBytesSent), bytesToSend);
    if (bytesToSend == bytesSent)
    {
        mBufferBytesSent = 0;
        mSendBuf->Reset();
        mSendBuf->SetNumValidBytes(0);
    }
    else if (bytesSent > 0)
    {
        mBufferBytesSent += bytesSent;
    }
    return bytesSent;
}

//! Returns the number of bytes remaining to send.
//! Call Flush() to send this data
int GenTCP_IO::GetUnsentBytes()
{
    return static_cast<int>(mSendBuf->GetNumValidBytes()) - mBufferBytesSent;
}

//! Removes all processed data from input buffer, and moves unprocessed data
//! to the beginning of the input buffer.
void GenTCP_IO::RemoveProcessedBufferData()
{
    long bytesToMove = GetGetOffset(GenBuf::FromDataEnd);
    memmove(mRecvBuf->GetBuffer(), mRecvBuf->GetBuffer() + GetGetOffset(GenBuf::FromBeg), bytesToMove);
    mRecvBuf->SetNumValidBytes(bytesToMove);
    mRecvBuf->SetGetOffset(0, GenBuf::FromBeg);
}

unsigned long GenTCP_IO::GetInputBufferSize()
{
    return mRecvBuf->GetTotalBytes();
}
