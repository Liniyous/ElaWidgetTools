#ifndef GenUDP_Connection_H
#define GenUDP_Connection_H

#include "ElaPacketIO_Export.h"

#include <string>
#include <vector>

#include "GenBuffer.h"
#include "GenIP.h"
#include "GenSocketConnection.h"

namespace GenSockets
{
class GenInternetSocketAddress;
class GenSocket;
} // namespace GenSockets

//! A UDP 'connection'
class ELA_PACKETIO_EXPORT GenUDP_Connection : public GenSocketConnection
{
public:
    GenUDP_Connection();

    ~GenUDP_Connection() override;

    bool Init(const std::string& aSendAddress, int aSendToPort, int aRecvPort);

    bool Init(const std::string& aSendAddress, const std::string& aInterfaceIP, int aSendToPort, int aRecvPort);

    bool Init(int aRecvPort);

    bool InitRecv(const std::string& aRecvAddress, int aRecvPort);

    bool Init(const std::string& aSendAddress, int aSendToPort);

    bool Init(const std::string& aSendAddress, const std::string& aInterfaceIP, int aSendToPort);

    virtual int ReceiveBuffer(int aWaitTimeInMicroSec, char* aBuffer, int aBytes);
    virtual int SendBuffer(const char* aBuffer, int aBytes);

    bool AddMulticastMembership(const std::string& aMulticastAddr);

    bool AddMulticastMembership(const std::string& aInterfaceAddr, const std::string& aMulticastAddr);

    bool RemoveMulticastMembership(const std::string& aMulticastAddr);

    bool RemoveMulticastMembership(const std::string& aInterfaceAddr, const std::string& aMulticastAddr);

    void SetMulticastInterface(const std::string& aInterfaceAddr);

    void SetIgnoreLocalBroadcastPackets();

    GenSockets::GenSocket* GetSocket() const { return mSocket.get(); }
    GenSockets::GenSocket* GetSendSocket() const { return mSendSocket; }
    GenSockets::GenSocket* GetRecvSocket() const { return mReadSocket; }

    void RememberSenderAddress(bool aTF) { mSaveSenderInfo = aTF; }

    GenSockets::GenInternetSocketAddress GetLastSenderAddress();

    virtual bool GetSenderId(unsigned int& aSrcInetAddr, unsigned short& aSrcPort) const;

    int GetSendToPort() const;

    enum ErrorTypes
    {
        cSOCKET_ERROR = -1, // use GenSocketManager to get error type
        cNOT_INITIALIZED = -2
    };
    void SetIsNonBlocking(bool aTF) override;

protected:
private:
    GenSockets::GenInternetSocketAddress* mSendAddress;
    GenSockets::GenInternetSocketAddress* mLastSender;
    bool mSaveSenderInfo;
    bool mIsBroadcast;
    bool mIgnoreLocalBroadcastPackets;
    std::vector<GenSockets::GenIP> mLocalIps;
    bool mHasSentMessages;
    int mLocalPort;

    GenSockets::GenSocket* mSendSocket;
    GenSockets::GenSocket* mReadSocket;
};
#endif
