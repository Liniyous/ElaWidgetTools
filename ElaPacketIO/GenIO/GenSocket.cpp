#include "GenSocket.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#ifndef _WIN32
#include <signal.h>
#endif
#ifdef __sun
#include <sys/filio.h>
#endif

#include "GenInternetSocketAddress.h"
#include "GenSocketIncludes.h"

namespace GenSockets
{

namespace
{
template <typename T>
bool SetSockOpt(SockFd aSocket, int aLevel, int aOptionName, const T& aValue)
{
    int rv = setsockopt(aSocket, aLevel, aOptionName, (const char*)&aValue, sizeof(T));
    if (rv == -1)
    {
        GenSockets::GenSocketManager::PrintLastError("setsockopt");
    }
    return rv != -1;
}

template <typename T>
bool GetSockOpt(SockFd aSocket, int aLevel, int aOptionName, T& aValue)
{
    socklen_t size = sizeof(T);
    int rv = getsockopt(aSocket, aLevel, aOptionName, (char*)&aValue, &size);
    if (rv == -1)
    {
        GenSockets::GenSocketManager::PrintLastError("getsockopt");
    }
    return rv != -1;
}
} // namespace

const SockFd GenSocket::InvalidSocket = (SockFd)-1;
GenOConvertBigEndian GenSocket::mBigEndianConvert;

GenSocket::GenSocket(GenSocket::SocketType aType)
    : mBoundPort(-1), mIsConnected(false), mIsPendingConnection(false), mSocketType(aType), mConnectedSocketAddress(nullptr), mTotalBytesSent(0), mTotalBytesReceived(0)
{
    GenSocketManager::Ready();
    mSocketOptions = 0;
    if (mSocketType == cUDP_SOCKET)
    {
        mSocket = socket(AF_INET, SOCK_DGRAM, 0);
        SetSocketOptions(cENABLE_MULTICAST_LOOPBACK | cNON_BLOCKING | cDISABLE_UNIQUE_BINDING_CHECK);
        SetSendBufferSize(1000000);
        SetReceiveBufferSize(1000000);
    }
    else
    {
        IgnoreSigPipe();
        mSocket = socket(AF_INET, SOCK_STREAM, 0);
        SetSocketOptions(cNON_BLOCKING);
    }
}
GenSocket::GenSocket(GenSocket::SocketType aType, bool aNoDelay)
    : mBoundPort(-1), mIsConnected(false), mIsPendingConnection(false), mSocketType(aType), mConnectedSocketAddress(nullptr), mTotalBytesSent(0), mTotalBytesReceived(0)
{
    GenSocketManager::Ready();
    mSocketOptions = 0;
    if (mSocketType == cTCP_SOCKET)
    {
        IgnoreSigPipe();
        mSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (aNoDelay)
        {
            // SetSocketOptions(cNON_BLOCKING);
            // AddSocketOptionsCopy(cTCP_NODELAY);
            SetSocketOptions(cTCP_NODELAY | cNON_BLOCKING);
            // SetSocketOptions(cTCP_NODELAY);
            // AddSocketOptionsCopy(cNON_BLOCKING | cTCP_NODELAY);
        }
        else
        {
            SetSocketOptions(cNON_BLOCKING);
        }
    }
}
GenSocket::GenSocket()
    : mTotalBytesSent(0), mTotalBytesReceived(0)
{
    mSocket = 0;
}
GenSocket::~GenSocket()
{
    delete mConnectedSocketAddress;
    Close();
}

//! Connects this socket to another
//! @param aAddr The address of the computer to connect to.
//! @return A value of type GenSocket::Error.  If the return value is
//!         cWOULD_BLOCK, calls to WaitForConnection() must be made
//!         until it returns 'true'.
int GenSocket::Connect(const GenInternetSocketAddress& aAddr)
{
    int rv = connect(mSocket, (sockaddr*)aAddr.mSockAddr, sizeof(sockaddr_in));
    if (rv != -1)
    {
        delete mConnectedSocketAddress;
        mConnectedSocketAddress = new GenInternetSocketAddress(aAddr);
        mIsConnected = true;
    }
    else
    {
        rv = GetLastError();
        if (rv == cWOULD_BLOCK)
        {
            mIsPendingConnection = true;
        }
    }
    return rv;
}

//! Sets the socket to a listening state.  This must be done
//! before you can accept connections
//! @return 'true' if the socket's state was successfully set to listen
bool GenSocket::Listen()
{
    return 0 == listen(mSocket, 5);
}

//! Accepts any incoming connections.
//! @param aWaitTime Time to wait for connection requests.
//! @return A pointer to the accepted connection, or 0 if
//!         there are no pending connection requests.
GenSocket* GenSocket::Accept(float aWaitTime)
{
    GenSocket* lReturn = nullptr;
    if (ReceiveReady(aWaitTime))
    {
        struct sockaddr_in their_addr;
        socklen_t size = sizeof(sockaddr_in);
        SockFd s = accept(mSocket, reinterpret_cast<sockaddr*>(&their_addr), &size);
        if (s != InvalidSocket)
        {
            lReturn = new GenSocket();
            lReturn->mSocket = s;
            lReturn->mSocketType = cTCP_SOCKET;
            lReturn->mSocketOptions = 0;
            lReturn->mIsConnected = true;
            lReturn->mConnectedSocketAddress = new GenInternetSocketAddress(their_addr);
            sockaddr_in addr;
            socklen_t size1 = sizeof(addr);
            getsockname(lReturn->mSocket, reinterpret_cast<sockaddr*>(&addr), &size1);
            lReturn->mBoundPort = ntohs(addr.sin_port);
            lReturn->SetSocketOptions(mSocketOptions);
        }
    }
    return lReturn;
}

//! Binds the socket to a port.
//! @param aPort Port to which the socket is bound,
//!              If 0, the system assigns an open port
//! @return 'true' if the socket was bound successfully
bool GenSocket::Bind(int aPort)
{
    struct sockaddr_in s_inet;
    memset(&s_inet, 0, sizeof(sockaddr_in));
    s_inet.sin_family = AF_INET;
    s_inet.sin_addr.s_addr = htonl(INADDR_ANY);
    s_inet.sin_port = htons(aPort);
    bool rv = (bind(mSocket, reinterpret_cast<sockaddr*>(&s_inet), sizeof(sockaddr_in)) == 0);
    if (rv)
    {
        sockaddr_in addr;
        socklen_t size = sizeof(addr);
        getsockname(mSocket, reinterpret_cast<sockaddr*>(&addr), &size);
        mBoundPort = ntohs(addr.sin_port);
    }
    return rv;
}

int GenSocket::GetBoundPort()
{
    if (mBoundPort == -1)
    {
        sockaddr_in addr;
        socklen_t size = sizeof(addr);
        if (0 == getsockname(mSocket, reinterpret_cast<sockaddr*>(&addr), &size))
        {
            mBoundPort = ntohs(addr.sin_port);
        }
    }
    return mBoundPort;
}

GenInternetSocketAddress GenSocket::GetBoundAddr()
{
    sockaddr_in addr;
    socklen_t size = sizeof(addr);
    if (0 == getsockname(mSocket, reinterpret_cast<sockaddr*>(&addr), &size))
    {
        return GenInternetSocketAddress(addr);
    }
    return GenInternetSocketAddress();
}

//! Binds the socket to a port and interface.
//! @param aAddr  The local address to bind to.
//! @return 'true' if the socket was bound successfully
bool GenSocket::Bind(const GenInternetSocketAddress& aAddr)
{
    struct sockaddr_in* s_inet = aAddr.mSockAddr;
    bool rv = (bind(mSocket, reinterpret_cast<sockaddr*>(s_inet), sizeof(sockaddr_in)) == 0);
    if (rv)
    {
        sockaddr_in addr;
        socklen_t size = sizeof(addr);
        getsockname(mSocket, reinterpret_cast<sockaddr*>(&addr), &size);
        mBoundPort = ntohs(addr.sin_port);
    }
    return rv;
}

//! send a buffer of data to the connected socket
//! @return Positive number of bytes successfully sent,
//!         Negative identifies an error of type Socket::Error
//! @note This method is only for connected sockets.
int GenSocket::Send(const char* aBuffer, int aBuffSize, float aWaitTime /* = 0*/)
{
    if (mSocketOptions & cNON_BLOCKING && aWaitTime > 0.0f)
    {
        if (!SendReady(aWaitTime))
        {
            return 0;
        }
    }
    if ((mSocketOptions & cEMULATE_MESSAGES_ON_STREAMS) && mSocketType == cTCP_SOCKET)
    {
        int data;
        mBigEndianConvert.PutToBuf(reinterpret_cast<unsigned char*>(&data), aBuffSize);
        SockSend(reinterpret_cast<char*>(&data), sizeof(int));
    }
    return SockSend(aBuffer, aBuffSize);
}

//! send a buffer of data to the connected socket
//! @return Positive number of bytes successfully sent,
//!         Negative identifies an error of type Socket::Error
//! @note This method is only for cUDP_SOCKETs.
int GenSocket::SendTo(const char* aBuffer, int aBuffSize, const GenInternetSocketAddress& aAddr)
{
    int rv = sendto(mSocket, aBuffer, aBuffSize, 0, (sockaddr*)aAddr.mSockAddr, sizeof(sockaddr_in));
    if (rv < 0)
    {
        if (GetLastError() == cWOULD_BLOCK)
        {
            rv = 0;
        }
        else
        {
            rv = cSOCKET_ERROR;
        }
    }
    else
    {
        mTotalBytesSent += rv;
    }
    return rv;
}

//! Receive a buffer of data to the connected socket.
//! @return Positive number of bytes successfully sent,
//!         Negative identifies an error of type Socket::Error
//! @note This method is only for connected sockets.
int GenSocket::Receive(char* aBuffer, int aBuffSize, float aWaitTime)
{
    int lReturn;
    if (mSocketOptions & cNON_BLOCKING && aWaitTime > 0.0f)
    {
        if (!ReceiveReady(aWaitTime))
        {
            return 0;
        }
    }
    if ((mSocketOptions & cEMULATE_MESSAGES_ON_STREAMS) && mSocketType == cTCP_SOCKET)
    {
        // Read message header
        int data;
        // Simulation::GetInstance().ResetClock();
        lReturn = SockRecv(reinterpret_cast<char*>(&data), sizeof(int));
        if (lReturn == 4)
        {
            int msgSize;
            mBigEndianConvert.PutToBuf(reinterpret_cast<unsigned char*>(&msgSize), data);
            if (msgSize <= aBuffSize)
            {
                // Buffer can hold entire message
                lReturn = SockRecv(aBuffer, msgSize);
            }
            else
            {
                // Buffer cannot hold entire message, rest message is dumped!
                lReturn = SockRecv(aBuffer, aBuffSize);
                if (lReturn > 0)
                {
                    // clear the rest of the message
                    int trashRv = ReceiveTrash(msgSize - lReturn);
                    if (trashRv < 0)
                    {
                        lReturn = trashRv;
                    }
                }
            }
        }
    }
    else
    {
        lReturn = SockRecv(aBuffer, aBuffSize);
    }
    return lReturn;
}

//! Receives data from the socket.
//! @param aBytes Number of bytes to read from the socket.
//! @return Positive number of bytes received.  Or one of enum Error.
int GenSocket::ReceiveTrash(int aBytes)
{
    int lReturn = aBytes;
    static char garbage[1024];
    int bytesLeft = aBytes;
    while (bytesLeft > 0)
    {
        int bytesToRead = bytesLeft;
        if (bytesToRead > 1024)
        {
            bytesToRead = 1024;
        }
        int bytesRecvd = recv(mSocket, garbage, bytesToRead, 0);
        if (bytesRecvd > 0)
        {
            bytesLeft -= bytesRecvd;
            mTotalBytesReceived += bytesRecvd;
        }
        else
        {
            lReturn = cSOCKET_ERROR;
            bytesLeft = 0;
        }
    }
    return lReturn;
}

//! Receive a buffer of data to the connected socket.
//! @param aBuffer     A buffer to be filled with incoming data.
//! @param aBuffSize       Size in bytes of aBuffer
//! @param[out] aAddr      A InternetSocketAddress to be filled with the
//!                        sender's Address
//! @param aWaitTime       Time to wait(in seconds) for incoming messages
//!                        before returning
//! @return                Positive number of bytes successfully sent,
//!                        Negative identifies an error of type Socket::Error
//! @note This method is only for connected cUDP_SOCKETs.
int GenSocket::ReceiveFrom(char* aBuffer, int aBuffSize, GenInternetSocketAddress& aAddr, float aWaitTime)
{
    if (mSocketOptions & cNON_BLOCKING && aWaitTime > 0.0f)
    {
        if (!ReceiveReady(aWaitTime))
        {
            return 0;
        }
    }
    socklen_t size = sizeof(sockaddr_in);
    aAddr.SetPort(mBoundPort);
    int rv = recvfrom(mSocket, aBuffer, aBuffSize, 0, reinterpret_cast<sockaddr*>(aAddr.mSockAddr), &size);
    if (rv <= 0)
    {
        // Connection Reset indicates a previously sent message was not received.
        int errorCode = GetLastError();
        if (rv < 0 && (errorCode == cWOULD_BLOCK || errorCode == cCONNECTION_RESET))
        {
            rv = 0;
        }
        else
        {
            mIsConnected = false;
            rv = cNOT_CONNECTED;
        }
    }
    else
    {
        mTotalBytesReceived += rv;
    }
    return rv;
}

//! Waits until this socket is ready to receive.  Returns when either
//! a socket is ready to receive data, or the wait time has expired.
//! @aWaitTime The time to wait.
//! @return True if a socket is ready to receive.
bool GenSocket::WaitUntilReceiveReady(float aWaitTime)
{
    return ReceiveReady(aWaitTime);
}

bool GenSocket::WaitUntilSendReady(float aWaitTime)
{
    return SendReady(aWaitTime);
}

//! Determines if the socket is ready to receive.
//! @param aWaitTime    Time (in seconds) to wait for incoming messages
//! @return 'true'      if the socket can receive now without blocking
//!         'false'     if the socket would block on receive
bool GenSocket::ReceiveReady(float aWaitTime)
{
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(mSocket, &readSet);
    struct timeval timeout;
    int waitSec = static_cast<int>(aWaitTime);
    timeout.tv_sec = waitSec;
    timeout.tv_usec = static_cast<long>((aWaitTime - waitSec) * 1000000.0f);
    return (select(FD_SETSIZE, &readSet, nullptr, nullptr, &timeout) > 0);
}

//! Determines if the socket is ready to send.
//! @param aWaitTime    Time (in seconds) to wait for incoming messages
//! @return 'true'   if the socket can send now without blocking
//!         'false'  if the socket would block on send
bool GenSocket::SendReady(float aWaitTime)
{
    fd_set sendSet;
    FD_ZERO(&sendSet);
    FD_SET(mSocket, &sendSet);
    struct timeval timeout;
    int waitSec = static_cast<int>(aWaitTime);
    timeout.tv_sec = waitSec;
    timeout.tv_usec = static_cast<long>((aWaitTime - waitSec) * 1000000.0f);
    return (select(FD_SETSIZE, nullptr, &sendSet, nullptr, &timeout) > 0);
}

//! Close the socket.
void GenSocket::Close()
{
    if (mSocket != InvalidSocket)
    {
#ifdef _WIN32
        closesocket(mSocket);
#else
        close(mSocket);
#endif
        mIsConnected = false;
        mSocket = InvalidSocket;
    }
}

//! Sets the options for this socket
//! @param aOptions Can be a any combination of GenSocket::SocketOptions
void GenSocket::SetSocketOptions(int aOptions)
{
    int diff = aOptions ^ mSocketOptions;
    SetOptions(diff & mSocketOptions, false);
    SetOptions(diff & aOptions, true);
    mSocketOptions = aOptions;
}

//! Adds options to the socket.
//! @param aOptions Can be any combination of GenSocket::SocketOptions
void GenSocket::AddSocketOptions(int aOptions)
{
    int finalOptions = mSocketOptions | aOptions;
    int newOptions = finalOptions ^ mSocketOptions;
    SetOptions(newOptions, true);
    mSocketOptions = finalOptions;
}

//! Adds options to the socket.
//! @param aOptions Can be any combination of GenSocket::SocketOptions
void GenSocket::AddSocketOptionsCopy(int aOptions)
{
    int finalOptions = mSocketOptions | aOptions;
    int newOptions = finalOptions | mSocketOptions;
    SetOptions(newOptions, true);
    mSocketOptions = finalOptions;
}

//! Removes options from the socket.
//! @param aOptions Can be any combination of GenSocket::SocketOptions
void GenSocket::RemoveSocketOptions(int aOptions)
{
    int removedOptions = mSocketOptions & aOptions;
    SetOptions(removedOptions, false);
    mSocketOptions ^= removedOptions;
}
void GenSocket::SetOptions(int aOptionMask, bool aOnOff)
{
    unsigned long onOff = (aOnOff ? 1UL : 0UL);
    if (aOptionMask & cNON_BLOCKING)
    {
#if defined(_WIN32)
        ioctlsocket(mSocket, FIONBIO, &onOff);
#else
        ioctl(mSocket, FIONBIO, &onOff);
#endif
    }
    if (aOptionMask & cENABLE_BROADCAST)
    {
        SetSockOpt(mSocket, SOL_SOCKET, SO_BROADCAST, onOff);
    }
    if (aOptionMask & cENABLE_MULTICAST_LOOPBACK)
    {
        SetSockOpt(mSocket, IPPROTO_IP, IP_MULTICAST_LOOP, onOff);
    }
    if (aOptionMask & cDISABLE_UNIQUE_BINDING_CHECK)
    {
        SetSockOpt(mSocket, SOL_SOCKET, SO_REUSEADDR, onOff);
    }
    if (aOptionMask & cTCP_NODELAY)
    {
#ifdef _WIN32
        SetSockOpt(mSocket, IPPROTO_TCP, TCP_NODELAY, onOff);
// TODO: Set on unix
#endif
    }
}

//! Become a member of a multicast group
//! @param aInterface Address of the interface to use.
//! @param aGroup Address of the group.
//! @return 'true' if socket was added to the multicast group successfully
bool GenSocket::AddMulticastGroupMembership(GenInternetAddress& aInterface, GenInternetAddress& aGroup)
{
    ip_mreq mreq;
    mreq.imr_interface.s_addr = aInterface.GetInAddr()->s_addr;
    mreq.imr_multiaddr.s_addr = aGroup.GetInAddr()->s_addr;
    return SetSockOpt(mSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, mreq);
}

//! Drop membership of a multicast group
//! @param aInterface Address of the interface to use.
//! @param aGroup Address of the group.
//! @return 'true' if socket was removed from the multicast group successfully
bool GenSocket::RemoveMulticastGroupMembership(GenInternetAddress& aInterface, GenInternetAddress& aGroup)
{
    ip_mreq mreq;
    mreq.imr_interface.s_addr = aInterface.GetInAddr()->s_addr;
    mreq.imr_multiaddr.s_addr = aGroup.GetInAddr()->s_addr;
    return SetSockOpt(mSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, mreq);
}

//! Set the Interface used by default to send multicast messages
//! @param aInterface Address of the interface to use.
void GenSocket::SetDefaultMulticastInterface(GenInternetAddress& aInterface)
{
    SetSockOpt(mSocket, IPPROTO_IP, IP_MULTICAST_IF, aInterface.GetInAddr()->s_addr);
}

//! Set the Time to live for multicast messages
//! @param aTTL The time to live: can be 0 to 255
//! 0:   Host
//! 1:   Subnet
//! 32:  Site         etc...
void GenSocket::SetMulticastTTL(int aTTL)
{
    SetSockOpt(mSocket, IPPROTO_IP, IP_MULTICAST_TTL, aTTL);
}

//! Returns the time to live for multicast messages sent by this socket
//! @return The time to live value
int GenSocket::GetMulticastTTL()
{
    int ttl = 1;
    GetSockOpt(mSocket, IPPROTO_IP, IP_MULTICAST_TTL, ttl);
    return ttl;
}

void GenSocket::GetConnectedSocketAddress(GenInternetSocketAddress& aSockAddr)
{
    if (mConnectedSocketAddress)
    {
        aSockAddr = *mConnectedSocketAddress;
    }
}

//! Translates a system error code into a value in the Error enumeration
GenSocket::Error GenSocket::TranslateError(int aErrorCode)
{
#ifndef _WIN32
    switch (aErrorCode)
    {
    case 0:
        return cNO_ERROR;
    case EAGAIN:
    case EINPROGRESS:
    case ENOBUFS:
        return cWOULD_BLOCK;
    case ECONNREFUSED:
    case ENETUNREACH:
    case ETIMEDOUT:
        return cCONNECTION_REFUSED;
    case ECONNRESET:
        return cCONNECTION_RESET;
    default:
        return cSOCKET_ERROR;
    }
#else
    switch (aErrorCode)
    {
    case 0:
        return cNO_ERROR;
    case WSAECONNREFUSED:
    case WSAENETUNREACH:
    case WSAETIMEDOUT:
        return cCONNECTION_REFUSED;
    case WSAEWOULDBLOCK:
    case WSAENOBUFS: // shouldn't be called for blocking socket, but it can happen
        return cWOULD_BLOCK;
    case WSAECONNRESET:
        return cCONNECTION_RESET;
    default:
        return cSOCKET_ERROR;
    }
#endif
}

//! Returns an Error enumeration describing the last error
GenSocket::Error GenSocket::GetLastError()
{
#ifdef _WIN32
    return TranslateError(WSAGetLastError());
#else
    return TranslateError(errno);
#endif
}

int GenSocket::SockRecv(char* aBuffer, int aBufferSize)
{
    int rv = recv(mSocket, aBuffer, aBufferSize, 0);
    if (rv <= 0)
    {
        GenSocket::Error lastError = GetLastError();
        if (rv < 0 && lastError == cWOULD_BLOCK)
        {
            rv = 0;
        }
        else
        {
            mIsConnected = false;
            rv = cNOT_CONNECTED;
        }
    }
    else
    {
        mTotalBytesReceived += rv;
    }
    return rv;
}

int GenSocket::SockSend(const char* aBuffer, int aBufferSize)
{
    int rv = send(mSocket, aBuffer, aBufferSize, 0);
    if (rv <= 0)
    {
        if (rv < 0 && GetLastError() == cWOULD_BLOCK)
        {
            rv = 0;
        }
        else
        {
            mIsConnected = false;
            rv = cNOT_CONNECTED;
        }
    }
    else
    {
        mTotalBytesSent += rv;
    }
    return rv;
}

//! If Connect() returns cWOULD_BLOCK, this function waits
//! for the connection to complete.
//! @return ConnectStatus describing the state of the connection
GenSocket::ConnectStatus GenSocket::WaitForConnection(float aWaitTime)
{
    ConnectStatus ok = cCONNECT_SUCCESS;
    if (mIsPendingConnection)
    {
        if (SendReady(aWaitTime))
        {
            if (QuerySocketError() == cNO_ERROR)
            {
                mIsConnected = true;
                mIsPendingConnection = false;
            }
            else // Connection failed!
            {
                ok = cCONNECT_FAILED;
                mIsPendingConnection = false;
            }
        }
        else
        {
            ok = cCONNECT_IN_PROGRESS;
        }
    }
    return ok;
}

//! Sets the size of the OS buffer used to receive socket data
void GenSocket::SetReceiveBufferSize(int aBufferBytes)
{
    SetSockOpt(mSocket, SOL_SOCKET, SO_RCVBUF, aBufferBytes);
}

//! Sets the size of the OS buffer used to send socket data
void GenSocket::SetSendBufferSize(int aBufferBytes)
{
    SetSockOpt(mSocket, SOL_SOCKET, SO_SNDBUF, aBufferBytes);
}

//! Sets the size of the OS buffer used to receive socket data
int GenSocket::GetReceiveBufferSize()
{
    int bytes = 0;
    GetSockOpt(mSocket, SOL_SOCKET, SO_RCVBUF, bytes);
    return bytes;
}

//! Sets the size of the OS buffer used to send socket data
int GenSocket::GetSendBufferSize()
{
    int bytes = 0;
    GetSockOpt(mSocket, SOL_SOCKET, SO_SNDBUF, bytes);
    return bytes;
}

//! If there is an error in the socket, returns an error code.
//! Otherwise, returns cNO_ERROR.
GenSocket::Error GenSocket::QuerySocketError()
{
    int errorNumber = 0;
    if (GetSockOpt(mSocket, SOL_SOCKET, SO_ERROR, errorNumber))
    {
        return TranslateError(errorNumber);
    }
    return cSOCKET_ERROR;
}

// static
//! Creates 2 connected sockets.
//! @param aSocket1 Set to a pointer to one of the connected sockets.
//! @param aSocket2 Set to a pointer to one of the connected sockets.
//! @return 'true' if the operation succeeded.
bool GenSocket::CreateSocketPair(GenSocket*& aSocket1, GenSocket*& aSocket2)
{
    bool ok = false;

    // temporary socket
    GenSocket tmpSocket(GenSocket::cTCP_SOCKET);

    // The two connecting sockets
    GenSocket* s1 = nullptr;
    GenSocket* s2 = new GenSocket(GenSocket::cTCP_SOCKET);

    // Bind / listen on a port chosen by the OS

    GenInternetSocketAddress bindAddr(GenIP(127, 0, 0, 1), 0);
    tmpSocket.Bind(bindAddr);
    tmpSocket.Listen();

    // Connect to the other socket in non-blocking mode
    s2->SetSocketOptions(GenSocket::cNON_BLOCKING);
    bindAddr = tmpSocket.GetBoundAddr();
    s2->Connect(bindAddr);
    s1 = tmpSocket.Accept(1.0);

    if (s1 != nullptr)
    {
        if (!s2->IsConnected())
        {
            s2->WaitForConnection(1.0);
            ok = true;
        }
        else
        {
            ok = true;
        }
    }

    if (!ok)
    {
        delete s1;
        delete s2;
        s1 = nullptr;
        s2 = nullptr;
    }

    aSocket1 = s1;
    aSocket2 = s2;
    return ok;
}

//! On some systems, SIGPIPE is sent to the process when send() is called on a disconnected
//! socket.  The default handler for SIGPIPE is to exit.  To play it safe, anytime a stream
//! socket is created, this method is called to ignore SIGPIPE.
// Note that this should be able to be accomplished with a socket-specific ioctl() call,
// but I haven't found the correct parameters yet.
void GenSocket::IgnoreSigPipe()
{
#ifndef _WIN32
    static bool firstTime = true;
    if (firstTime)
    {
        signal(SIGPIPE, SIG_IGN);
        firstTime = false;
    }
#endif
}

} // namespace GenSockets
