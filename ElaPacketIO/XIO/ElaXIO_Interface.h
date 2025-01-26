#ifndef ELAXIO_INTERFACE_H
#define ELAXIO_INTERFACE_H

#include "ElaPacketIO_Export.h"

#include <deque>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "GenUniqueId.h"
#include "PakConnection.h"
#include "PakProcessor.h"
class PakTCP_Connector;
class PakTCP_IO;
#include "PakThreadedIO.h"
#include "UtCallback.h"
#include "UtCallbackHolder.h"
#include "UtWallClock.h"
class ElaXIO_Connection;
class ElaXIO_HeartbeatPkt;
class ElaXIO_InitializePkt;
class ElaXIO_Packet;
class ElaXIO_UdpHeader;

#define PACKET_HANDLE_FUNC_DEFINE(INTERFACE, ...) INTERFACE->addCallback(INTERFACE->Connect(__VA_ARGS__))
#define CONNECT_HANDLE_FUNC_DEFINE(INTERFACE, ...) INTERFACE->addCallback(INTERFACE->OnConnected.Connect(__VA_ARGS__))
#define DISCONNECT_HANDLE_FUNC_DEFINE(INTERFACE, ...) INTERFACE->addCallback(INTERFACE->OnDisconnected.Connect(__VA_ARGS__))

class ELA_PACKETIO_EXPORT ElaXIO_Interface : public PakProcessor
{
public:
    enum UDP_Type
    {
        Broadcast,
        Multicast,
        Unicast
    };
    struct UDP_Target {
        UDP_Target()
            : _sendPort(0), _recvPort(0), _connectionId(0)
        {
        }
        UDP_Type _type;
        std::string _address;
        std::string _interfaceIP;
        int _sendPort;
        int _recvPort;
        int _connectionId;
    };
    typedef std::pair<int, int> SenderAddress;
    typedef std::vector<ElaXIO_Connection*> ConnectionList;
    typedef UtCallbackN<void(ElaXIO_Connection*)> ConnectionCallback;
    typedef UtCallbackListN<void(ElaXIO_Connection*)> ConnectionCallbackList;

    explicit ElaXIO_Interface();
    ~ElaXIO_Interface();

    const GenUniqueId& getApplicationId() const { return _applicationId; }
    void setApplicationName(const std::string& applicationName) { _applicationName = applicationName; }
    std::string getApplicationName() const { return _applicationName; }

    void init(int port);
    void unInit();
    void addCallback(std::unique_ptr<UtCallback> callback);

    void send(ElaXIO_Packet& packet, ElaXIO_Connection* connection);
    void send(ElaXIO_Packet& packet, const std::vector<PakSocketIO*>& connectionVector);
    void sendToAll(ElaXIO_Packet& packet);
    void sendToAllUDP(ElaXIO_Packet& packet);
    void sendToAllTCP(ElaXIO_Packet& packet);

    ElaXIO_Connection* getSender(PakPacket& packet);

    UtCallbackListN<void(ElaXIO_Connection*)> OnConnected;
    UtCallbackListN<void(ElaXIO_Connection*)> OnDisconnected;
    UtCallbackListN<void(uint64_t, GenUniqueId, bool)> OnHeartbeatUpdate;

    std::unique_ptr<ConnectionCallback> disconnectConnect(ElaXIO_Connection* aConnectionPtr, const ConnectionCallback::FunctionType& aFunction);

    void addUDP_Target(UDP_Target aTarget);
    bool removeUDP_Target(int aTargetIndex);
    const std::vector<UDP_Target>& getUDP_Targets() { return mUDP_Targets; }

    const ConnectionList& GetConnections() const { return mConnections; }
    const ConnectionList& GetReliableConnections() const { return mConnectedConnections; }
    ElaXIO_Connection* FindConnection(const GenUniqueId& aApplicationId);
    ElaXIO_Connection* FindConnection(int aConnectionIndex);
    ElaXIO_Connection* FindConnection(const std::string& aApplicationName);
    void Disconnect(ElaXIO_Connection* aConnectionPtr);

    void getBytesCommunicated(size_t& aBytesSent, size_t& aBytesReceived);

private:
    void _executeCoreProcessor();
    void _sendHeartbeat();
    bool _checkForDuplicateConnection(ElaXIO_Connection* checkedConnection);
    ElaXIO_Connection* _getSendConnection(ElaXIO_Connection* connectionPtr);
    void _processMessages();
    void _handleHeartbeat(ElaXIO_HeartbeatPkt& pkt);
    void _handleInit(ElaXIO_InitializePkt& pkt);
    void _handleDisconnect(PakSocketIO* socketIO, PakConnection* aConnectionPtr);
    void _addConnection(ElaXIO_Connection* connection);
    void _acceptConnections();
    bool _connectToTarget(UDP_Target& aTarget);
    struct HeartbeatInfo {
        HeartbeatInfo(GenUniqueId id)
            : mUniqueId(id)
        {
        }
        bool operator<(const HeartbeatInfo& aInfo) const { return mUniqueId < aInfo.mUniqueId; }
        GenUniqueId mUniqueId;
        ElaXIO_Connection* mConnectionPtr;
    };
    using ConnectionCallbackMap = std::map<ElaXIO_Connection*, UtCallbackListN<void(ElaXIO_Connection*)>*>;
    using SenderConnectionMap = std::map<SenderAddress, ElaXIO_Connection*>;
    using ConnectionIdMap = std::map<int, ElaXIO_Connection*>;
    UtCallbackHolder _callbacks;
    UtCallbackHolder _userCallbacks;
    GenUniqueId _applicationId; //!< The application's unique ID
    std::string _applicationName;
    ElaXIO_UdpHeader* mUDP_HeaderPtr; //!< Header used for UDP messages
    PakTCP_Connector* mConnectorPtr{nullptr};
    // The following variables are used for sending out the heartbeat pdu's
    UtWallClock mClock;
    double mCurrentTime;
    double mPreviousHeartbeatTime;
    double mPreviousConnectionUpdateTime;
    double mConnectionUpdateInterval;
    //! Set of application ID's that have already had a connection-chance
    std::set<HeartbeatInfo> mProcessedHeartbeats;
    //! Maintains a mapping between UDP sender address and the related reliable connection
    SenderConnectionMap mSenderConnections;
    //! List of all current connections
    ConnectionList mConnections;
    //! List of current reliable connections
    ConnectionList mConnectedConnections;
    //! Map from connection id to connection
    ConnectionIdMap mConnectionsById;
    PakThreadedIO _threadedIO;
    ConnectionCallbackMap mDisconnectCallbacks;
    std::map<std::string, std::string> mAvailableServices;
    // 用于统计发送接受信息
    size_t mTotalBytesSent;
    size_t mTotalBytesReceived;
    size_t mPreviousBytesSent;
    size_t mPreviousBytesReceived;
    int mTCP_Port;
    int mMulticastTimeToLive;  // 多播连接的生存时间到了
    bool mMulticastLoopback;   // 确定发送的多播是否可以在本地计算机上接收
    double mHeartbeatInterval; // 心跳包发送间隔
    bool mShowTransferRate;
    bool _isInit;
    std::vector<UDP_Target> mUDP_Targets;
    std::thread _updateThread;
    UtWallClock _clock;
};

#endif
