#include "GenUDP_Connection.h"

#include <iostream>

#include "GenHostName.h"
#include "GenIP.h"
#include "GenInternetAddress.h"
#include "GenInternetSocketAddress.h"
#include "GenSocket.h"
#include "GenSocketIncludes.h"
GenUDP_Connection::GenUDP_Connection()
    : GenSocketConnection(new GenSockets::GenSocket(GenSockets::GenSocket::cUDP_SOCKET)), mSendAddress(nullptr), mLastSender(new GenSockets::GenInternetSocketAddress), mSaveSenderInfo(false), mIsBroadcast(false), mIgnoreLocalBroadcastPackets(false), mHasSentMessages(false)
{
    mReadSocket = mSendSocket = mSocket.get();

    // Generate IP list of local interfaces used to filter out
    // packets originating from this socket
    GenSockets::GenHostName::LocalHostName().GetIP_List(mLocalIps);
    // Add in loopback: 127.0.0.1
    mLocalIps.emplace_back(127, 0, 0, 1);
}

GenUDP_Connection::~GenUDP_Connection()
{
    if (mReadSocket != mSendSocket)
    {
        delete mReadSocket;
    }
    delete mSendAddress;
    delete mLastSender;
}

//! Initialize and enable sending and receiving.
//! Use the default interface.
bool GenUDP_Connection::Init(const std::string& aSendAddress, int aSendToPort, int aRecvPort)
{
    return Init(aSendAddress, "0.0.0.0", aSendToPort, aRecvPort);
}

//! Initialize and enable sending and receiving.
//! Specifies the interface to use.
bool GenUDP_Connection::Init(const std::string& aSendAddress, const std::string& aInterfaceIP, int aSendToPort, int aRecvPort)
{
    bool lReturn = false;

    bool goodInterface = false;
    GenSockets::GenInternetSocketAddress sendBindAddr;
    if (aInterfaceIP != "0.0.0.0")
    {
        GenSockets::GenIP interfaceIP = GenSockets::GenSocketManager::GetInterfaceLocalIP(aInterfaceIP);
        if (interfaceIP.IsValidForm())
        {
            GenSockets::GenInternetAddress addr(interfaceIP);
            sendBindAddr = GenSockets::GenInternetSocketAddress(addr, 0);
            goodInterface = true;
        }
    }
    else
    {
        goodInterface = true;
    }

    if (goodInterface)
    {
        GenSockets::GenIP ipaddr(aSendAddress);
        if (ipaddr.IsValidForm())
        {
            if (ipaddr.IsBroadcast())
            {
                mSocket->AddSocketOptions(GenSockets::GenSocket::cENABLE_BROADCAST);
                mIsBroadcast = true;
            }
        }
        mSendAddress = new GenSockets::GenInternetSocketAddress(aSendAddress, aSendToPort);
        if (aSendToPort == aRecvPort)
        {
            // bind the send socket if there is an interface specified.
            if (aInterfaceIP == "0.0.0.0" || mSocket->Bind(sendBindAddr))
            {
                // If we are sending and receiving on the same port,
                // we need a way of discarding our own packets,
                // so use Connect() to get a unique local port
                if (0 == mSocket->Connect(*mSendAddress))
                {
                    mLocalPort = mSocket->GetBoundPort();
                    if (ipaddr.IsBroadcast() || ipaddr.IsMulticast())
                    {
                        mIgnoreLocalBroadcastPackets = true;
                    }
                    // But we now need another socket for receiving.
                    mReadSocket = new GenSockets::GenSocket(GenSockets::GenSocket::cUDP_SOCKET);
                    if (mReadSocket->Bind(aRecvPort))
                    {
                        lReturn = true;
                    }
                }
            }
        }
        else
        {
            sendBindAddr.SetPort(aRecvPort);
            if (mSocket->Bind(sendBindAddr))
            {
                lReturn = true;
            }
        }
    }
    return lReturn;
}

//! Receive only
bool GenUDP_Connection::Init(int aRecvPort)
{
    bool lReturn = true;
    if (!mSocket->Bind(aRecvPort))
    {
        std::cout << "Cannot bind socket to receiving port." << "Port: " << aRecvPort << std::endl;
        lReturn = false;
    }
    return lReturn;
}

bool GenUDP_Connection::InitRecv(const std::string& aRecvAddress, int aRecvPort)
{
    GenSockets::GenIP ipaddr(aRecvAddress);
    if (ipaddr.IsValidForm())
    {
        if (ipaddr.IsBroadcast())
        {
            mSocket->AddSocketOptions(GenSockets::GenSocket::cENABLE_BROADCAST);
            mIsBroadcast = true;
        }
    }
    GenSockets::GenInternetSocketAddress addr(aRecvAddress, aRecvPort);
    bool ok = mSocket->Bind(addr);
    return ok;
}

//! send only
bool GenUDP_Connection::Init(const std::string& aSendAddress, int aSendToPort)
{
    bool lReturn = true;

    GenSockets::GenIP ipaddr(aSendAddress);
    if (ipaddr.IsValidForm())
    {
        mSendAddress = new GenSockets::GenInternetSocketAddress(GenSockets::GenInternetAddress(ipaddr), aSendToPort);
        if (ipaddr.IsBroadcast())
        {
            mSocket->AddSocketOptions(GenSockets::GenSocket::cENABLE_BROADCAST);
            mIsBroadcast = true;
        }
    }
    else
    {
        GenSockets::GenHostName hName(aSendAddress);
        mSendAddress = new GenSockets::GenInternetSocketAddress(GenSockets::GenInternetAddress(hName), aSendToPort);
    }
    return lReturn;
}

//! send only.  Specifies the interface to use.
bool GenUDP_Connection::Init(const std::string& aSendAddress, const std::string& aInterfaceIP, int aSendToPort)
{
    bool ok = false;
    if (Init(aSendAddress, aSendToPort))
    {
        GenSockets::GenIP interfaceIp = GenSockets::GenSocketManager::GetInterfaceLocalIP(aInterfaceIP);
        if (interfaceIp.IsValidForm())
        {
            GenSockets::GenInternetAddress addr(interfaceIp);
            GenSockets::GenInternetSocketAddress sockAddr(addr, 0);
            ok = mSocket->Bind(sockAddr);
        }
    }
    return ok;
}

// virtual
//! Fill buffer with an incoming message.  Existing data in buffer
//! is removed.
//! @param aWaitTimeInMicroSec The Time in microseconds to wait for incomming messages
int GenUDP_Connection::ReceiveBuffer(int aWaitTimeInMicroSec, char* aBuffer, int aMaxBytes)
{
    if (mReadSocket->GetBoundPort() == -1)
    {
        std::cout << "GenUDP_Connection not initialized for receiving. " << std::endl;
        return cNOT_INITIALIZED;
    }
    // Dont ignore local broadcast packets unless this class has sent
    // at least one.
    bool filterLocalPackets = mHasSentMessages && mIgnoreLocalBroadcastPackets;
    int bytes;
    if (!mSaveSenderInfo && !filterLocalPackets)
    {
        bytes = mReadSocket->Receive(aBuffer, aMaxBytes, aWaitTimeInMicroSec * 1.0E-6F);
    }
    else
    {
        bool dropped;
        do
        {
            dropped = false;
            bytes = mReadSocket->ReceiveFrom(aBuffer, aMaxBytes, *mLastSender, aWaitTimeInMicroSec * 1.0E-6F);
            if (bytes != 0 && filterLocalPackets)
            {
                if (mLastSender->GetPort() == mLocalPort)
                {
                    // The ports match up, just make sure this really is us by
                    // checking the IP
                    unsigned long senderAddr = mLastSender->GetAddress().GetInAddr()->s_addr;
                    for (unsigned i = 0; i < mLocalIps.size(); ++i)
                    {
                        if (mLocalIps[i].GetAddress() == senderAddr)
                        {
                            // It is from us, so drop the message.  Make this IP the first to check next time
                            if (i != 0)
                            {
                                std::swap(mLocalIps[0], mLocalIps[i]);
                            }
                            dropped = true;
                            bytes = 0;
                            break;
                        }
                    }
                }
            }
        } while (dropped);
    }
    return bytes;
}

// virtual
//! send data in the send buffer as a single UDP message
int GenUDP_Connection::SendBuffer(const char* aBuffer, int aBytes)
{
    if (!mSendAddress)
    {
        std::cout << "GenUDP_Connection not initialized for sending." << std::endl;
        return cNOT_INITIALIZED;
    }
    mHasSentMessages = true;
    return mSendSocket->SendTo(aBuffer, aBytes, *mSendAddress);
}

//! Become a member of a multicast group.  The default interface is used.
//! @param aMulticastAddr The IP Address of the multicast group
//! @return 'true' if successful
bool GenUDP_Connection::AddMulticastMembership(const std::string& aMulticastAddr)
{
    bool lReturn = false;
    GenSockets::GenIP multicastAddr(aMulticastAddr);
    if (multicastAddr.IsMulticast())
    {
        lReturn = AddMulticastMembership("0.0.0.0", aMulticastAddr);
    }
    else
    {
        std::cout << "Invalid multicast IP. " << "Address: " << aMulticastAddr;
    }
    return lReturn;
}

bool GenUDP_Connection::AddMulticastMembership(const std::string& aInterfaceAddr, const std::string& aMulticastAddr)
{
    bool lReturn = false;
    GenSockets::GenIP multicastAddr(aMulticastAddr);
    if (multicastAddr.IsMulticast())
    {
        GenSockets::GenIP inter = GenSockets::GenSocketManager::GetInterfaceLocalIP(aInterfaceAddr);
        if (inter.IsValidForm() || aInterfaceAddr == "0.0.0.0")
        {
            if (mSendAddress)
            {
                int rcvPort = mReadSocket->GetBoundPort();
                int sendPort = mSendAddress->GetPort();
                if (rcvPort == sendPort)
                {
                    mIgnoreLocalBroadcastPackets = true;
                }
            }
            GenSockets::GenInternetAddress interfaceAddr(inter), multicastAddr1(aMulticastAddr);
            lReturn = mReadSocket->AddMulticastGroupMembership(interfaceAddr, multicastAddr1);
        }
        else
        {
            std::cout << "Invalid interface for multicast. " << "Address: " << aMulticastAddr << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid multicast IP. " << "Address: " << aMulticastAddr << std::endl;
    }
    return lReturn;
}

//! Drop membership of a multicast group.  The default interface is used.
//! @param aMulticastAddr The IP Address of the multicast group.
//! @return 'true' if successful
bool GenUDP_Connection::RemoveMulticastMembership(const std::string& aMulticastAddr)
{
    bool lReturn = false;
    GenSockets::GenIP multicastAddr(aMulticastAddr);
    if (multicastAddr.IsMulticast())
    {
        lReturn = RemoveMulticastMembership("0.0.0.0", aMulticastAddr);
    }
    else
    {
        std::cout << "Invalid multicast IP. " << "Address: " << aMulticastAddr << std::endl;
    }
    return lReturn;
}

//! Drop membership of a multicast group
//! @param aInterfaceAddr The IP Address of the interface to use.
//! @param aMulticastAddr The IP Address of the multicast group.
//! @return 'true' if successful
bool GenUDP_Connection::RemoveMulticastMembership(const std::string& aInterfaceAddr, const std::string& aMulticastAddr)
{
    bool lReturn = false;
    GenSockets::GenIP multicastAddr(aMulticastAddr);
    if (multicastAddr.IsMulticast())
    {
        GenSockets::GenIP inter = GenSockets::GenSocketManager::GetInterfaceLocalIP(aInterfaceAddr);
        if (inter.IsValidForm())
        {
            GenSockets::GenInternetAddress interfaceAddr(inter), multicastAddr1(aMulticastAddr);
            lReturn = mReadSocket->RemoveMulticastGroupMembership(interfaceAddr, multicastAddr1);
        }
        else
        {
            std::cout << "Invalid interface for multicast. " << "Address: " << aMulticastAddr << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid multicast IP. " << "Address: " << aMulticastAddr << std::endl;
    }
    return lReturn;
}

//! Set the default multicast interface.
//! @param aInterfaceAddr The IP Address of the interface to use.
void GenUDP_Connection::SetMulticastInterface(const std::string& aInterfaceAddr)
{
    GenSockets::GenIP inter = GenSockets::GenSocketManager::GetInterfaceLocalIP(aInterfaceAddr);
    GenSockets::GenInternetAddress addr(inter);
    mSendSocket->SetDefaultMulticastInterface(addr);
}

//! Return the address of the sender of the last message received
//! @return The address of the sender of the last message received
GenSockets::GenInternetSocketAddress GenUDP_Connection::GetLastSenderAddress()
{
    return *mLastSender;
}

// virtual
bool GenUDP_Connection::GetSenderId(unsigned int& aSrcInetAddr, unsigned short& aSrcPort) const
{
    bool lReturn = false;
    if (mLastSender)
    {
        GenSockets::GenIP ip = mLastSender->GetAddress();
        aSrcInetAddr = ip;
        aSrcPort = mLastSender->GetPort();
        lReturn = true;
    }
    return lReturn;
}

//! getValue the destination port number.
int GenUDP_Connection::GetSendToPort() const
{
    return mSendAddress ? mSendAddress->GetPort() : 0;
}

// virtual
//! Sets the blocking flag.
//! @param aTF 'true' makes the IO non-blocking.  'false' makes the IO blocking.
void GenUDP_Connection::SetIsNonBlocking(bool aTF)
{
    if (aTF)
    {
        mReadSocket->AddSocketOptions(GenSockets::GenSocket::cNON_BLOCKING);
        if (mReadSocket != mSendSocket)
        {
            mSendSocket->AddSocketOptions(GenSockets::GenSocket::cNON_BLOCKING);
        }
    }
    else
    {
        mReadSocket->RemoveSocketOptions(GenSockets::GenSocket::cNON_BLOCKING);
        if (mReadSocket != mSendSocket)
        {
            mSendSocket->RemoveSocketOptions(GenSockets::GenSocket::cNON_BLOCKING);
        }
    }
}
