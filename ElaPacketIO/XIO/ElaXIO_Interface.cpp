#include "xio/ElaXIO_Interface.h"

#include <limits>
#include <memory>

#include "ElaXIO_Connection.h"
#include "ElaXIO_PacketRegistry.h"
#include "ElaXIO_UdpHeader.h"
#include "GenIP.h"
#include "GenInternetSocketAddress.h"
#include "GenNetInfo.h"
#include "GenSocket.h"
#include "GenTCP_IO.h"
#include "GenUDP_IO.h"
#include "PakProcessor.h"
#include "PakTCP_Connector.h"
#include "PakTCP_IO.h"
#include "PakUDP_IO.h"

ElaXIO_Interface::ElaXIO_Interface()
    : _applicationName("ElaPacketIO"),
      mTCP_Port(0),
      mMulticastTimeToLive(-1),
      mMulticastLoopback(true),
      mHeartbeatInterval(5.0),
      mShowTransferRate(false),
      _isInit(false),
      mConnectorPtr(nullptr),
      mCurrentTime(0.0),
      mPreviousHeartbeatTime(-1.0E6),
      mPreviousConnectionUpdateTime(-1.0E6),
      mConnectionUpdateInterval(0.5),
      mTotalBytesSent(0),
      mTotalBytesReceived(0),
      mPreviousBytesSent(0),
      mPreviousBytesReceived(0),
      mUDP_HeaderPtr(new ElaXIO_UdpHeader(ElaXIO_PacketRegistry::getPacketVersion()))
{
    ElaXIO_PacketRegistry::registerPackets(*this);

    _callbacks += Connect(&ElaXIO_Interface::_handleHeartbeat, this);
    _callbacks += Connect(&ElaXIO_Interface::_handleInit, this);
    _callbacks += _threadedIO.Disconnected.Connect(&ElaXIO_Interface::_handleDisconnect, this);
}

ElaXIO_Interface::~ElaXIO_Interface()
{
    if (_isInit)
    {
        size_t sentBytes, recvBytes;
        getBytesCommunicated(sentBytes, recvBytes);
        double elapsedTime = mCurrentTime;
        if (elapsedTime <= 1.0)
        {
            elapsedTime = 1.0;
        }
        double sendRate = sentBytes / elapsedTime;
        double recvRate = recvBytes / elapsedTime;

        std::cout << "xio_interface: Terminated."
                  << " Sent " << sentBytes << " bytes at " << sendRate << " bytes/second"
                  << " Received " << recvBytes << " bytes at " << recvRate << " bytes/second"
                  << std::endl;
        _threadedIO.Stop();
        _threadedIO.Join();
        delete mUDP_HeaderPtr;
        delete mConnectorPtr;
        unInit();
    }
    else
    {
        delete mUDP_HeaderPtr;
        delete mConnectorPtr;
        const ConnectionList& connectionList = GetConnections();
        for (auto& connection: mConnections)
        {
            delete connection;
        }
    }
}

void ElaXIO_Interface::init(int port)
{
    if (mUDP_Targets.empty() || _isInit)
    {
        return;
    }
    mConnectorPtr = new PakTCP_Connector(this);
    if (!mConnectorPtr->Listen(port))
    {
        std::cout << "xio_interface: Could not bind to a port." << std::endl;
    }
    else
    {
        mTCP_Port = mConnectorPtr->GetBoundPort();
        std::cout << "xio_interface: Accepting connections." << " Port: " << mTCP_Port << std::endl;
    }
    for (UDP_Target& target: mUDP_Targets)
    {
        _connectToTarget(target);
    }
    _threadedIO.Start();
    _isInit = true;
    std::thread updateThread([=]() {
        while (_isInit)
        {
            _executeCoreProcessor();
            std::this_thread::sleep_for(std::chrono::microseconds(30));
        }
    });
    _updateThread = std::move(updateThread);
}

void ElaXIO_Interface::unInit()
{
    _isInit = false;
    if (_updateThread.joinable())
    {
        _updateThread.join();
    }
    const std::vector<UDP_Target>& udpTargetVector = getUDP_Targets();
    for (int i = 0; i < udpTargetVector.size(); i++)
    {
        removeUDP_Target(i);
    }
    const ConnectionList& connectionList = GetConnections();
    for (auto& connection: mConnections)
    {
        delete connection;
    }
    mConnections.clear();
}

bool ElaXIO_Interface::_connectToTarget(UDP_Target& aTarget)
{
    bool connected = false;

    if (aTarget._type == Broadcast || aTarget._type == Unicast)
    {
        std::unique_ptr<GenUDP_IO> udpIO(new GenUDP_IO());
        if (aTarget._type == Unicast)
        {
            udpIO->GetRecvSocket()->RemoveSocketOptions(GenSockets::GenSocket::cDISABLE_UNIQUE_BINDING_CHECK);
        }
        if (aTarget._recvPort == 0)
        {
            connected = udpIO->Init(aTarget._address, aTarget._sendPort);
        }
        else if (aTarget._sendPort == 0)
        {
            int recvPort = aTarget._recvPort;
            if (recvPort == -1) // -1 indicates we should let the OS choose a port
            {
                recvPort = 0;
            }
            connected = udpIO->InitRecv(aTarget._address, recvPort);
            if (connected && udpIO->GetRecvSocket())
            {
                aTarget._recvPort = udpIO->GetRecvSocket()->GetBoundPort();
            }
        }
        else
        {
            connected = udpIO->Init(aTarget._address, aTarget._sendPort, aTarget._recvPort);
        }

        if (!connected)
        {
            std::cout << "xio_interface: Could not bind to port. " << "Port: " << aTarget._recvPort << std::endl;
        }
        else
        {
            udpIO->RememberSenderAddress(true);
            ElaXIO_Connection* connectionPtr = new ElaXIO_Connection(this, new PakUDP_IO(udpIO.release(), this, mUDP_HeaderPtr->Clone()));
            _threadedIO.AddIO(&connectionPtr->GetIO(), connectionPtr);
            _addConnection(connectionPtr);
            connectionPtr->setInitialized();
            aTarget._connectionId = connectionPtr->getConnectionId();
        }
    }
    else if (aTarget._type == Multicast)
    {
        std::unique_ptr<GenUDP_IO> udpIO(new GenUDP_IO());
        if (!udpIO->Init(aTarget._address, aTarget._interfaceIP, aTarget._sendPort, aTarget._recvPort))
        {
            std::cout << "xio_interface: Could not bind to port. " << "Port: " << aTarget._recvPort << std::endl;
            return false;
        }
        else
        {
            if (mMulticastTimeToLive != -1)
            {
                udpIO->GetSendSocket()->SetMulticastTTL(mMulticastTimeToLive);
            }
            if (!mMulticastLoopback)
            {
                udpIO->GetSendSocket()->RemoveSocketOptions(GenSockets::GenSocket::cENABLE_MULTICAST_LOOPBACK);
            }

            udpIO->RememberSenderAddress(true);
            udpIO->AddMulticastMembership(aTarget._interfaceIP, aTarget._address);
            ElaXIO_Connection* connectionPtr = new ElaXIO_Connection(this, new PakUDP_IO(udpIO.release(), this, mUDP_HeaderPtr->Clone()));
            _threadedIO.AddIO(&connectionPtr->GetIO(), connectionPtr);
            _addConnection(connectionPtr);
            connectionPtr->setInitialized();
            aTarget._connectionId = connectionPtr->getConnectionId();
            connected = true;
        }
    }

    if (connected)
    {
        std::cout << "xio_interface: Connected." << std::endl;
        switch (aTarget._type)
        {
        case Broadcast:
        {
            std::cout << "Broadcast Address: " << aTarget._address << std::endl;
            break;
        }
        case Unicast:
        {
            std::cout << "Unicast Address: " << aTarget._address << std::endl;
            break;
        }
        case Multicast:
        {
            std::cout << "Multicast Address: " << aTarget._address << " " << aTarget._interfaceIP << std::endl;
            break;
        }
        }
        std::cout << "Sending Port: " << aTarget._sendPort << std::endl;
        std::cout << "Receiving Port: " << aTarget._recvPort << std::endl;
    }

    return connected;
}

void ElaXIO_Interface::_acceptConnections()
{
    if (mConnectorPtr != nullptr)
    {
        PakTCP_IO* ioPtr;
        while ((ioPtr = mConnectorPtr->Accept(0)) != nullptr)
        {
            ElaXIO_Connection* connectionPtr = new ElaXIO_Connection(this, ioPtr);
            _threadedIO.AddIO(ioPtr, connectionPtr);
            _addConnection(connectionPtr);
        }

        GenSockets::GenInternetSocketAddress inetSockAddr;
        while (mConnectorPtr->CompleteConnect(inetSockAddr, ioPtr))
        {
            ElaXIO_Connection* connectionPtr = new ElaXIO_Connection(this, ioPtr);
            _threadedIO.AddIO(ioPtr, connectionPtr);
            _addConnection(connectionPtr);

            // 开始初始化握手
            ElaXIO_InitializePkt pkt;
            pkt._applicationName = _applicationName;
            pkt._stage = 0;
            connectionPtr->send(pkt);
        }
    }
}

void ElaXIO_Interface::send(ElaXIO_Packet& packet, ElaXIO_Connection* connection)
{
    packet._applicationId = _applicationId;
    packet.SetBaseTime(_clock.GetClock());
    _threadedIO.Send(&connection->GetIO(), packet);
}

void ElaXIO_Interface::send(ElaXIO_Packet& packet, const std::vector<PakSocketIO*>& connectionVector)
{
    packet._applicationId = _applicationId;
    packet.SetBaseTime(_clock.GetClock());
    _threadedIO.Send(connectionVector, packet);
}

ElaXIO_Connection* ElaXIO_Interface::getSender(PakPacket& packet)
{
    return static_cast<ElaXIO_Connection*>(packet.GetSender());
}

void ElaXIO_Interface::sendToAll(ElaXIO_Packet& packet)
{
    packet._applicationId = _applicationId;
    packet.SetBaseTime(_clock.GetClock());
    _threadedIO.SendToAll(packet);
}

void ElaXIO_Interface::sendToAllUDP(ElaXIO_Packet& packet)
{
    packet._applicationId = _applicationId;
    packet.SetBaseTime(_clock.GetClock());
    std::vector<PakSocketIO*> sendList;
    for (auto& connection: mConnections)
    {
        PakUDP_IO* ioPtr = connection->GetUDP_IO();
        if (ioPtr != nullptr && ioPtr->GetConnection().GetSendToPort() > 0)
        {
            sendList.push_back(ioPtr);
        }
    }
    _threadedIO.Send(sendList, packet);
}

void ElaXIO_Interface::sendToAllTCP(ElaXIO_Packet& packet)
{
    packet._applicationId = _applicationId;
    std::vector<PakSocketIO*> sendList;
    for (auto& connection: mConnections)
    {
        if (connection->GetTCP_IO())
        {
            sendList.push_back(connection->GetTCP_IO());
        }
    }
    _threadedIO.Send(sendList, packet);
}

void ElaXIO_Interface::_addConnection(ElaXIO_Connection* connection)
{
    mConnections.push_back(connection);
    mConnectionsById[connection->getConnectionId()] = connection;
    // 在与ElaXIO_InitializePkt握手之前，TCP连接不被视为“已连接”
    if (!connection->GetTCP_IO())
    {
        OnConnected(connection);
    }
}

std::unique_ptr<ElaXIO_Interface::ConnectionCallback> ElaXIO_Interface::disconnectConnect(ElaXIO_Connection* aConnectionPtr, const ConnectionCallback::FunctionType& aFunction)
{
    ConnectionCallbackList* cbListPtr = nullptr;
    ConnectionCallbackMap::iterator i = mDisconnectCallbacks.find(aConnectionPtr);
    if (i == mDisconnectCallbacks.end())
    {
        mDisconnectCallbacks[aConnectionPtr] = cbListPtr = new ConnectionCallbackList;
    }
    else
    {
        cbListPtr = i->second;
    }
    auto callbackPtr = cbListPtr->Connect(aFunction);
    return callbackPtr;
}

void ElaXIO_Interface::Disconnect(ElaXIO_Connection* aConnectionPtr)
{
    // trigger HandleDisconnect()
    _threadedIO.RemoveIO(&aConnectionPtr->GetIO());
}

void ElaXIO_Interface::addCallback(std::unique_ptr<UtCallback> callback)
{
    _userCallbacks.Add(std::move(callback));
}

void ElaXIO_Interface::_executeCoreProcessor()
{
    if (mPreviousConnectionUpdateTime < mCurrentTime - mConnectionUpdateInterval)
    {
        mPreviousConnectionUpdateTime = mCurrentTime;
        _acceptConnections();
    }
    _processMessages();
    for (auto& connection: mConnections)
    {
        PakTCP_IO* ioPtr = connection->GetTCP_IO();
        if (ioPtr != nullptr)
        {
            ioPtr->Flush();
        }
    }
    mCurrentTime = mClock.GetRawClock();
    if (mPreviousHeartbeatTime < mCurrentTime - mHeartbeatInterval)
    {
        mPreviousHeartbeatTime = mCurrentTime;
        _sendHeartbeat();
        if (mShowTransferRate)
        {
            size_t sentBytes, recvBytes;
            getBytesCommunicated(sentBytes, recvBytes);
            size_t newSentBytes = sentBytes - mPreviousBytesSent;
            size_t newRecvBytes = recvBytes - mPreviousBytesReceived;
            mPreviousBytesSent = sentBytes;
            mPreviousBytesReceived = recvBytes;
            if (mShowTransferRate)
            {
                std::cout << "xio_interface: Advanced time."
                          << " Sent " << sentBytes << " bytes at " << newSentBytes / mHeartbeatInterval << " bytes/second"
                          << " Received " << recvBytes << " bytes at " << newRecvBytes / mHeartbeatInterval
                          << " bytes/second"
                          << std::endl;
            }
        }
    }
}

void ElaXIO_Interface::_processMessages()
{
    PakThreadedIO::PacketList packets;
    _threadedIO.Extract(packets);
    for (auto packet: packets)
    {
        ElaXIO_Packet* pkt = dynamic_cast<ElaXIO_Packet*>(packet);
        if (!pkt)
        {
            return;
        }
        ElaXIO_Connection* senderPtr = (ElaXIO_Connection*)pkt->GetSender();
        if (senderPtr && senderPtr->isReliable())
        {
            senderPtr->setLastTimeStamp((double)pkt->getBaseTime());
        }
        PakProcessor::ProcessPacket(pkt, true);
    }
}

void ElaXIO_Interface::_sendHeartbeat()
{
    OnHeartbeatUpdate(_clock.GetClock(), _applicationId, false);
    ElaXIO_HeartbeatPkt pkt;
    pkt._tcpPort = (uint16_t)(mTCP_Port);
    pkt._applicationName = _applicationName;
    sendToAllUDP(pkt);
}

void ElaXIO_Interface::_handleHeartbeat(ElaXIO_HeartbeatPkt& pkt)
{
    OnHeartbeatUpdate(pkt.getBaseTime(), pkt._applicationId, true);
    if (pkt._tcpPort != 0)
    {
        if (mProcessedHeartbeats.find(pkt._applicationId) == mProcessedHeartbeats.end())
        {
            if (FindConnection(pkt._applicationId) == nullptr)
            {
                unsigned int addr = pkt.GetOriginatorAddress();
                GenSockets::GenIP ip(addr);
                GenSockets::GenInternetSocketAddress address(ip, pkt._tcpPort);
                std::cout << "xio_interface: Received heartbeat. " << "Attempting to connect to " << static_cast<std::string>(address) << std::endl;
                mConnectorPtr->BeginConnect(address, 10.0);
                _sendHeartbeat();
            }
            HeartbeatInfo newHeartbeat(pkt._applicationId);
            newHeartbeat.mConnectionPtr = (ElaXIO_Connection*)pkt.GetSender();
            mProcessedHeartbeats.insert(newHeartbeat);

            for (ElaXIO_Connection* connectionPtr: mConnections)
            {
                if (connectionPtr->getApplicationId() == pkt._applicationId && connectionPtr->GetTCP_IO() != nullptr)
                {
                    connectionPtr->SetLinkedConnection((ElaXIO_Connection*)pkt.GetSender());
                }
            }
        }
    }
}

bool ElaXIO_Interface::_checkForDuplicateConnection(ElaXIO_Connection* checkedConnection)
{
    ElaXIO_Connection* duplicatePtr = nullptr;
    for (const auto& connection: mConnections)
    {
        if (checkedConnection != connection && checkedConnection->getApplicationId() == connection->getApplicationId())
        {
            duplicatePtr = connection;
        }
    }
    bool isDuplicate = false;
    if (duplicatePtr != nullptr)
    {
        if (duplicatePtr->isInitialized())
        {
            isDuplicate = true;
            Disconnect(checkedConnection);
        }
        else
        {
            // The application ID is used to make sure each application disconnects the same connection
            if (_applicationId < checkedConnection->getApplicationId())
            {
                isDuplicate = true;
                Disconnect(checkedConnection);
            }
            else
            {
                Disconnect(duplicatePtr);
            }
        }
    }

    return isDuplicate;
}

//! Returns a connection which should be used to send to an address.
//! May create a unicast UDP connection if necessary.
//! @param connectionPtr The connection the message was received by.
//! @param aSendAddress   Address of the sender
ElaXIO_Connection* ElaXIO_Interface::_getSendConnection(ElaXIO_Connection* connectionPtr)
{
    ElaXIO_Connection* sendConnectionPtr = nullptr;
    PakUDP_IO* ioPtr = nullptr;
    if (connectionPtr != nullptr)
    {
        ioPtr = connectionPtr->GetUDP_IO();
    }
    if (ioPtr == nullptr)
    {
        sendConnectionPtr = connectionPtr;
    }
    else if (ioPtr->GetConnection().GetSendToPort() > 0)
    {
        sendConnectionPtr = connectionPtr;
    }
    return sendConnectionPtr;
}

void ElaXIO_Interface::_handleInit(ElaXIO_InitializePkt& pkt)
{
    const int cCONNECT_STAGE = 15;
    // ElaXIO_InitializePkt作为三次握手发送3次 防止使用重复连接
    bool ok = true;
    ElaXIO_Connection* connectionPtr = getSender(pkt);
    if (std::find(mConnections.begin(), mConnections.end(), connectionPtr) == mConnections.end())
    {
        return;
    }

    if (pkt._stage <= cCONNECT_STAGE)
    {
        connectionPtr->setApplicationId(pkt._applicationId);
        connectionPtr->setApplicationName(pkt._applicationName);

        ElaXIO_Connection* relatedConnectionPtr = nullptr;
        std::set<HeartbeatInfo>::iterator iter = mProcessedHeartbeats.find(pkt._applicationId);
        if (iter != mProcessedHeartbeats.end())
        {
            relatedConnectionPtr = iter->mConnectionPtr;
        }
        SenderAddress senderAddr(pkt.GetOriginatorAddress(), pkt.GetOriginatorPort());
        ElaXIO_Connection* sendConnectionPtr = _getSendConnection(relatedConnectionPtr);
        if (sendConnectionPtr != nullptr)
        {
            connectionPtr->SetLinkedConnection(sendConnectionPtr);
        }
        else
        {
            connectionPtr->SetLinkedConnection(connectionPtr);
        }
    }

    if (pkt._stage <= cCONNECT_STAGE)
    {
        ok = !_checkForDuplicateConnection(connectionPtr);
    }
    if (ok)
    {
        if (pkt._stage <= cCONNECT_STAGE)
        {
            ElaXIO_InitializePkt response;
            response._stage = pkt._stage + 1;
            response._applicationName = _applicationName;
            connectionPtr->send(response);
        }
        if (pkt._stage >= cCONNECT_STAGE)
        {
            connectionPtr->setInitialized();
            mConnectedConnections.push_back(connectionPtr);
            SenderAddress senderAddr(pkt.GetOriginatorAddress(), pkt.GetOriginatorPort());
            mSenderConnections[senderAddr] = connectionPtr;
            std::cout << "xio_interface: Connected to application. " << "Application: " << connectionPtr->getApplicationName() << std::endl;
            OnConnected(connectionPtr);
        }
    }
}

void ElaXIO_Interface::_handleDisconnect(PakSocketIO* socketIO, PakConnection* connection)
{
    ElaXIO_Connection* connectionPtr = static_cast<ElaXIO_Connection*>(connection);
    connectionPtr->setDisconnecting();
    if (connectionPtr->isInitialized())
    {
        if (connectionPtr->GetTCP_IO() != nullptr)
        {
            GenSockets::GenSocket* socketPtr = connectionPtr->GetTCP_IO()->GetSendSocket();
            mTotalBytesReceived += socketPtr->GetTotalBytesReceived();
            mTotalBytesSent += socketPtr->GetTotalBytesSent();
        }
        OnDisconnected(connectionPtr);
        ConnectionCallbackMap::iterator cbIter = mDisconnectCallbacks.find(connectionPtr);
        if (cbIter != mDisconnectCallbacks.end())
        {
            (*cbIter->second)(connectionPtr);
        }
    }

    if (connectionPtr->isInitialized() && connectionPtr->isReliable())
    {
        std::cout << "xio_interface: Disconnected from application. " << "Application: " << connectionPtr->getApplicationName() << std::endl;
    }

    ConnectionList::iterator i = std::find(mConnections.begin(), mConnections.end(), connection);
    if (i != mConnections.end())
    {
        mConnections.erase(i);
        mConnectionsById.erase(connectionPtr->getConnectionId());
        ConnectionList::iterator j = std::find(mConnectedConnections.begin(), mConnectedConnections.end(), connection);
        if (j != mConnectedConnections.end())
        {
            mConnectedConnections.erase(j);
        }
        for (SenderConnectionMap::iterator k = mSenderConnections.begin(); k != mSenderConnections.end(); ++k)
        {
            if (k->second == connection)
            {
                mSenderConnections.erase(k);
                break;
            }
        }
    }
    delete connection;
}

ElaXIO_Connection* ElaXIO_Interface::FindConnection(const GenUniqueId& aApplicationId)
{
    ElaXIO_Connection* connectionPtr = nullptr;
    for (auto& connection: mConnections)
    {
        if (aApplicationId == connection->getApplicationId())
        {
            connectionPtr = connection;
        }
    }
    return connectionPtr;
}

//! Returns a pointer to the connection with the given ID
ElaXIO_Connection* ElaXIO_Interface::FindConnection(int aConnectionIndex)
{
    ConnectionIdMap::iterator i = mConnectionsById.find(aConnectionIndex);
    if (i != mConnectionsById.end())
    {
        return i->second;
    }
    return nullptr;
}

ElaXIO_Connection* ElaXIO_Interface::FindConnection(const std::string& aApplicationName)
{
    ElaXIO_Connection* connectionPtr = nullptr;
    for (auto& connection: mConnections)
    {
        if (aApplicationName == connection->getApplicationName())
        {
            connectionPtr = connection;
        }
    }
    return connectionPtr;
}

void ElaXIO_Interface::getBytesCommunicated(size_t& aBytesSent, size_t& aBytesReceived)
{
    aBytesSent = mTotalBytesSent;
    aBytesReceived = mTotalBytesReceived;

    for (ElaXIO_Connection* connectionPtr: mConnections)
    {
        PakTCP_IO* tcpPtr = connectionPtr->GetTCP_IO();
        if (tcpPtr != nullptr)
        {
            aBytesReceived += tcpPtr->GetRecvSocket()->GetTotalBytesReceived();
            aBytesSent += tcpPtr->GetSendSocket()->GetTotalBytesSent();
        }
        else
        {
            PakUDP_IO* udpPtr = connectionPtr->GetUDP_IO();
            aBytesReceived += udpPtr->GetRecvSocket()->GetTotalBytesReceived();
            aBytesSent += udpPtr->GetSendSocket()->GetTotalBytesSent();
        }
    }
}

void ElaXIO_Interface::addUDP_Target(UDP_Target aTarget)
{
    if (_isInit)
    {
        if (_connectToTarget(aTarget))
        {
            mUDP_Targets.push_back(aTarget);
        }
    }
    else
    {
        mUDP_Targets.push_back(aTarget);
    }
}

bool ElaXIO_Interface::removeUDP_Target(int aTargetIndex)
{
    bool removed = false;
    if (_isInit)
    {
        const UDP_Target& target = mUDP_Targets[aTargetIndex];
        ElaXIO_Connection* connectionPtr = FindConnection(target._connectionId);
        if (connectionPtr != nullptr)
        {
            // Don't remove if it is linked to another connection.
            for (ElaXIO_Connection* cPtr: mConnections)
            {
                if (cPtr->GetLinkedConnection() == connectionPtr)
                {
                    return false;
                }
            }
        }
        Disconnect(connectionPtr);
        mUDP_Targets.erase(mUDP_Targets.begin() + aTargetIndex);
        removed = true;
    }
    else
    {
        mUDP_Targets.erase(mUDP_Targets.begin() + aTargetIndex);
        removed = true;
    }
    return removed;
}
