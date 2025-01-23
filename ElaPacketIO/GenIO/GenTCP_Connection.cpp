#include "GenTCP_Connection.h"

#include <cstring>
#include <iostream>

#include "GenHostName.h"
#include "GenIP.h"
#include "GenInternetAddress.h"
#include "GenInternetSocketAddress.h"
#include "GenSocket.h"
GenTCP_Connection::GenTCP_Connection()
    : GenSocketConnection(new GenSockets::GenSocket(GenSockets::GenSocket::cTCP_SOCKET)), mUseMessageHeaders(false), mIsInitialized(false)
{
    ActivateSocketOptions();
}

//! Initialize by listening for incoming connection requests.
//! Subsequent calls to Accept() must be made before
//! the connection is complete.
//! @return 'true' if everything initialized correctly
//! @see Accept
bool GenTCP_Connection::InitListen(int aRecvPort)
{
    UnInitialize();

    if (!mSocket->Bind(aRecvPort))
    {
        std::cout << "Cannot bind socket to receiving port. " << "Port: " << aRecvPort << std::endl;
        ResetSocket();
    }
    else
    {
        if (mSocket->Listen())
        {
            mIsInitialized = true;
        }
        else
        {
            GenSockets::GenSocketManager::PrintLastError("InitListen");
            ResetSocket();
        }
    }
    return mIsInitialized;
}

//! Initialize by attempting to connect to a address.
//! @return 'true' if successfully connected.
bool GenTCP_Connection::Init(const std::string& aSendAddress, int aSendToPort, float aWaitTimeSeconds)
{
    UnInitialize();

    GenSockets::GenInternetAddress inAddr(aSendAddress);
    if (inAddr.IsValid())
    {
        mSendAddress = std::make_unique<GenSockets::GenInternetSocketAddress>(inAddr, aSendToPort);
        int connectError = mSocket->Connect(*mSendAddress);
        if (connectError == GenSockets::GenSocket::cWOULD_BLOCK)
        {
            GenSockets::GenSocket::ConnectStatus connectStatus = mSocket->WaitForConnection(aWaitTimeSeconds);
            if (connectStatus == GenSockets::GenSocket::cCONNECT_SUCCESS)
            {
                // connection success
                connectError = 0;
            }
        }
        if (connectError == 0)
        {
            mIsInitialized = true;
        }
        else
        {
            // did not connect in time: cancel connection request
            // Leave it up to the application to print an error
            ResetSocket();
        }
    }
    else
    {
        std::cout << "Error: Unknown host. " << "Host Address: " << aSendAddress << std::endl;
    }
    return mIsInitialized;
}

//! Initialize using a socket that is already connected.  This class assumes
//! ownership of the pointer.
//! @return 'true' if the socket a valid connected TCP socket
bool GenTCP_Connection::Init(std::unique_ptr<GenSockets::GenSocket> aConnectedSocket)
{
    UnInitialize();
    if (aConnectedSocket->IsConnected() && aConnectedSocket->GetSocketType() == GenSockets::GenSocket::cTCP_SOCKET)
    {
        ResetSocket(std::move(aConnectedSocket));
        ActivateSocketOptions();
        mIsInitialized = true;
    }
    return mIsInitialized;
}

//! Accept an incoming connection.  May only be used after calling InitListen()
//! @param  aWaitTimeSeconds The time to wait for incoming connections in seconds
//! @return 'true' if successfully connected
//! @see InitListen
bool GenTCP_Connection::Accept(float aWaitTimeSeconds)
{
    std::unique_ptr<GenSockets::GenSocket> newSocket{mSocket->Accept(aWaitTimeSeconds)};
    if (newSocket)
    {
        ResetSocket(std::move(newSocket));
        return true;
    }
    return false;
}

// virtual
//! Fill buffer with an incoming message
//! @param aWaitTimeInMicroSec The Time in microseconds to wait for incoming messages
//! @return The number of received bytes
int GenTCP_Connection::ReceiveBuffer(int aWaitTimeInMicroSec, char* aBuffer, int aMaxBytes)
{
    int bytes = 0;
    if (mSocket->IsConnected())
    {
        // could be erasing unread data...
        bytes = mSocket->Receive(aBuffer, aMaxBytes, aWaitTimeInMicroSec * 1.0E-6F);
    }
    return bytes;
}

int GenTCP_Connection::ReceiveBuffer(int aWaitTimeInMicroSec, GenBuffer& aBuffer, int aMaxBytes)
{
    int bytesToRead = aMaxBytes;
    if (bytesToRead == 0)
    {
        bytesToRead = (int)(aBuffer.GetBytes() - aBuffer.GetPutPos());
    }
    int bytes = ReceiveBuffer(aWaitTimeInMicroSec, aBuffer.GetBuffer() + aBuffer.GetPutPos(), bytesToRead);
    if (bytes > 0)
    {
        aBuffer.SetPutPos(aBuffer.GetPutPos() + bytes);
    }
    return bytes;
}

//! send data in the send buffer across the TCP stream
//! @return The number of bytes successfully sent or a negative
//!         number of ErrorTypes
int GenTCP_Connection::SendBuffer(int aWaitTimeInMicroSec, const char* aDataPtr, int aBytes)
{
    int bytesSent = 0;
    if (mSocket->IsConnected())
    {
        bytesSent = mSocket->Send(aDataPtr, aBytes, aWaitTimeInMicroSec * 1.0E-6F);
    }
    return bytesSent;
}

//! Return the address of the computer we are connected to
//! @return The address of the computer we are connected to
GenSockets::GenInternetSocketAddress GenTCP_Connection::GetSenderAddress() const
{
    return *mSendAddress;
}

// virtual
bool GenTCP_Connection::GetSenderId(unsigned int& aSrcInetAddr, unsigned short int& aSrcPort) const
{
    bool lReturn = false;
    if (mSendAddress)
    {
        GenSockets::GenIP ip = mSendAddress->GetAddress();
        aSrcInetAddr = ip;
        aSrcPort = mSendAddress->GetPort();
        lReturn = true;
    }
    return lReturn;
}

//! Sets the flag that determines if a header is added to messages.
//! This header allows the receiving end of the TCP connection to separate
//! the messages so that each send() call will result in a Receive()
//! returning the same number of bytes that was sent.  However, this header
//! is only compatible with other GenTCP_Connection instances.
//! @param aTF 'true' if headers will be added to messages
void GenTCP_Connection::SetUseMessageHeaders(bool aTF)
{
    mUseMessageHeaders = aTF;
    ActivateSocketOptions();
}

//! Returns 'true' if headers are added to messages.  See SetUseMessageHeaders().
bool GenTCP_Connection::GetUseMessageHeaders() const
{
    return mUseMessageHeaders;
}

void GenTCP_Connection::ActivateSocketOptions()
{
    if (mUseMessageHeaders)
    {
        mSocket->AddSocketOptions(GenSockets::GenSocket::cEMULATE_MESSAGES_ON_STREAMS);
    }
    else
    {
        mSocket->RemoveSocketOptions(GenSockets::GenSocket::cEMULATE_MESSAGES_ON_STREAMS);
    }
}

void GenTCP_Connection::ResetSocket(std::unique_ptr<GenSockets::GenSocket> aNewSocket)
{
    if (aNewSocket)
    {
        mSocket = std::move(aNewSocket);
    }
    else
    {
        mSocket = std::make_unique<GenSockets::GenSocket>(GenSockets::GenSocket::cTCP_SOCKET);
    }
}

void GenTCP_Connection::UnInitialize()
{
    if (mIsInitialized)
    {
        ResetSocket();
        mSendAddress = nullptr;
        mIsInitialized = false;
    }
}
