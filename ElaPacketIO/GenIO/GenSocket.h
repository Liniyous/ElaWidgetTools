#ifndef GENSOCKET_H
#define GENSOCKET_H

#include "ElaPacketIO_Export.h"

#include <list>

#include "GenOConvertBigEndian.h"
#include "GenSocketManager.h"
#if defined(_WIN32)
#include <basetsd.h>
#endif

namespace GenSockets
{

#if defined(_WIN32)
typedef UINT_PTR SockFd;
#else
typedef int SockFd;
#endif

class GenIP;
class GenInternetAddress;
class GenInternetSocketAddress;

/** Wrapper class for a socket
 */
class ELA_PACKETIO_EXPORT GenSocket
{
    static const SockFd InvalidSocket;

public:
    // General socket errors.  Used to generalize the error code given by the OS.
    enum Error
    {
        cNO_ERROR = 0,
        cSOCKET_ERROR = -1, // get further info w/ errno
        cWOULD_BLOCK = -2,
        cNOT_CONNECTED = -3,
        cCONNECTION_REFUSED = -4,
        cCONNECTION_RESET = -5
    };
    enum ConnectStatus
    {
        cCONNECT_IN_PROGRESS,
        cCONNECT_FAILED,
        cCONNECT_SUCCESS
    };
    enum SocketType
    {
        cTCP_SOCKET,
        cUDP_SOCKET
    };
    enum SocketOptions
    {
        cNON_BLOCKING = 1,
        cENABLE_BROADCAST = 2,
        cENABLE_MULTICAST_LOOPBACK = 4,
        cDISABLE_UNIQUE_BINDING_CHECK = 8,
        cEMULATE_MESSAGES_ON_STREAMS = 0x10,
        cTCP_NODELAY = 0x20
    };
    enum WaitAction
    {
        cWAITED,
        cSOCKET_READY,
        cCAN_NOT_WAIT
    };
    GenSocket(SocketType aType);
    GenSocket(SocketType aType, bool aNoDelay);

    ~GenSocket();

    void Close();

    int Connect(const GenInternetSocketAddress& aAddr);

    bool Listen();

    GenSocket* Accept(float aWaitTime);

    bool Bind(int aPort = 0);

    bool Bind(const GenInternetSocketAddress& aAddr);

    int Send(const char* aBuffer, int aBuffSize, float aWaitTime = 0);

    int SendTo(const char* aBuffer, int aBuffSize, const GenInternetSocketAddress& aAddr);

    int Receive(char* aBuffer, int aBuffSize, float aWaitTime = 0);

    int ReceiveFrom(char* aBuffer, int aBuffSize, GenInternetSocketAddress& aAddr, float aWaitTime = 0);

    bool WaitUntilReceiveReady(float aWaitTime = 1.0E10);

    bool WaitUntilSendReady(float aWaitTime = 1.0E10);

    //! Return the port this socket is bound to.
    //! @return Positive value: The port this socket is bound to.
    //!         -1:             Unbound Socket
    int GetBoundPort();

    //! Return the Socket Address on which this port is bound.
    //! The Address returned will be INADDR_ANY if it is not bound.
    GenInternetSocketAddress GetBoundAddr();

    //! Return the GenSocket::SocketOptions flags.
    //! @return The GenSocket::SocketOptions flags.
    int GetSocketOptions() const { return mSocketOptions; }

    void SetSocketOptions(int aOptions);

    void AddSocketOptions(int aOptions);

    void AddSocketOptionsCopy(int aOptions);
    void RemoveSocketOptions(int aOptions);

    bool AddMulticastGroupMembership(GenInternetAddress& aInterface, GenInternetAddress& aGroup);

    bool RemoveMulticastGroupMembership(GenInternetAddress& aInterface, GenInternetAddress& aGroup);

    void SetDefaultMulticastInterface(GenInternetAddress& aInterface);

    int GetMulticastTTL();

    void SetMulticastTTL(int aTTL);

    //! Returns true if the socket is connected
    bool IsConnected() { return mIsConnected; }

    //! Returns the type of socket
    SocketType GetSocketType() { return mSocketType; }

    //! Sets the aSockAddr parameter to the Socket Address of the foreign
    //! host if the socket is connected.
    //! @param[out] aSockAddr The socked address of the foreign host
    void GetConnectedSocketAddress(GenInternetSocketAddress& aSockAddr);

    ConnectStatus WaitForConnection(float aWaitTime = 1.0);

    //! Sets the size of the OS buffer used to receive socket data
    void SetReceiveBufferSize(int aBufferBytes);

    //! Returns the size of the OS buffer used to receive socket data in bytes
    int GetReceiveBufferSize();

    //! Sets the size of the OS buffer used to send socket data
    void SetSendBufferSize(int aBufferBytes);

    //! Returns the size of the OS buffer used to send socket data in bytes
    int GetSendBufferSize();

    //! Returns the file descriptor for the socket
    SockFd GetSocketFileDescriptor() { return mSocket; }

    //! Returns the number of bytes sent over the socket lifetime
    size_t GetTotalBytesSent() const { return mTotalBytesSent; }

    //! Returns the number of bytes received over the socket lifetime
    size_t GetTotalBytesReceived() const { return mTotalBytesReceived; }

    Error QuerySocketError();

    static bool CreateSocketPair(GenSocket*& aSocket1, GenSocket*& aSocket2);

    double GetClock();

    double GetTime();

    void ResetClock();

protected:
    static void IgnoreSigPipe();

    void SetOptions(int aOptionMask, bool aOnOff);

    GenSocket();

    bool ReceiveReady(float aWaitTime = 0.0f);

    bool SendReady(float aWaitTime = 0.0f);

    int ReceiveTrash(int aBytes);

    Error GetLastError();

    Error TranslateError(int aErrorCode);

    int SockRecv(char* aBuffer, int aBufferSize);

    int SockSend(const char* aBuffer, int aBufferSize);

    int mSocketOptions;
    SockFd mSocket;
    int mBoundPort;
    bool mIsConnected;
    bool mIsPendingConnection;
    SocketType mSocketType;
    GenInternetSocketAddress* mConnectedSocketAddress;

    size_t mTotalBytesSent;
    size_t mTotalBytesReceived;

    static GenOConvertBigEndian mBigEndianConvert;

private:
    // prevent assignment & copy constructor
    GenSocket& operator=(const GenSocket&);
    GenSocket(const GenSocket&);
};
} // namespace GenSockets

#endif
