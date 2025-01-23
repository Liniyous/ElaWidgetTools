#include "GenUDP_IO.h"

#include <iostream>

#include "GenBufIManaged.h"
#include "GenBufOManaged.h"
#include "GenHostName.h"
#include "GenInternetAddress.h"
#include "GenInternetSocketAddress.h"
#include "GenSocketIncludes.h"

GenUDP_IO::GenUDP_IO()
    : GenUDP_Connection(), GenNetIO("GenUDP_IO", "GenUDP_IO")
{
    // Maximum Transmission Unit(MTU) is usually 1400-1500.
    mRecvBuf = new GenBufIManaged(GenBuf::BigEndian, 10000);
    SetGenBufXferI(mRecvBuf);
    mSendBuf = new GenBufOManaged(GenBuf::BigEndian, 10000, 1);
    SetGenBufXferO(mSendBuf);
}

GenUDP_IO::~GenUDP_IO()
{
    delete mRecvBuf;
    delete mSendBuf;
}

// virtual
//! Fill buffer with an incoming message.  Existing data in buffer
//! is removed.
//! @param aWaitTimeInMicroSec The Time in microseconds to wait for incoming messages
int GenUDP_IO::Receive(int aWaitTimeInMicroSec)
{
    // could be erasing unread data...
    mRecvBuf->Reset();
    mRecvBuf->SetNumValidBytes(0);
    ClearInputErrorStatus();

    int bytesRecv = ReceiveBuffer(aWaitTimeInMicroSec,
                                  reinterpret_cast<char*>(mRecvBuf->GetBuffer()),
                                  static_cast<int>(mRecvBuf->GetTotalBytes()));
    if (bytesRecv > 0)
    {
        mRecvBuf->SetNumValidBytes(bytesRecv);
    }
    return bytesRecv;
}

// virtual
//! send data in the send buffer as a single UDP message
int GenUDP_IO::Send()
{
    ClearOutputErrorStatus();
    int bytesToSend = static_cast<int>(mSendBuf->GetNumValidBytes());
    int bytesSent = SendBuffer(reinterpret_cast<const char*>(mSendBuf->GetBuffer()), bytesToSend);
    mSendBuf->Reset();
    mSendBuf->SetNumValidBytes(0);
    return bytesSent;
}
