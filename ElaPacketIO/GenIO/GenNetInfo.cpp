#include "GenNetInfo.h"

#include <cerrno>
#include <cstring>

#if defined(_WIN32)
#include <winsock2.h>

#include <ios>
#include <iostream>
#include <ws2tcpip.h>
#elif defined(__linux__)
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#endif

struct GenNetInfo::InfoImpl {
    std::string mAddr;
    std::string mBroadcastAddr;
    std::string mNetmask;
    std::string mId;
    bool mIsUp;
    bool mIsLoopback;
    bool mIsPointToPoint;
    bool mHasBroadcast;
    bool mHasMulticast;
};

#ifndef _WIN32
bool GetNameInfo(struct sockaddr* aAddr, int aFamily, std::string& aName)
{
    char host[NI_MAXHOST];
    int size = sizeof(struct sockaddr_in);
    if (aFamily == AF_INET6)
    {
        size = sizeof(struct sockaddr_in6);
    }
    int err = getnameinfo(aAddr, size, host, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
    bool success = (err == 0);
    if (success)
    {
        aName = host;
    }
    return success;
}
#endif

// private
GenNetInfo::GenNetInfo()
{
    int numInterfaces = 0;

#if defined(_WIN32)
    SOCKET sd = WSASocketW(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);

    if (sd == SOCKET_ERROR)
    {
        std::cout << "Winsock: " << WSAGetLastError() << std::endl;
        return;
    }

    INTERFACE_INFO InterfaceList[20];
    unsigned long nBytesReturned;
    int result = WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList, sizeof(InterfaceList), &nBytesReturned, 0, 0);
    if (result == SOCKET_ERROR)
    {
        std::cout << "Winsock: " << WSAGetLastError() << std::endl;
        return;
    }

    numInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);

    for (int i = 0; i < numInterfaces; ++i)
    {
        Interface* interfacePtr = new Interface();

        sockaddr_in* pAddress;
        char str[INET_ADDRSTRLEN];
        pAddress = (sockaddr_in*)&(InterfaceList[i].iiAddress);
        const char* result = inet_ntop(AF_INET, &(pAddress->sin_addr), str, INET_ADDRSTRLEN);
        if (result == NULL)
        {
            std::cout << "Failed to convert address to string. " << "Errno: " << std::strerror(errno) << std::endl;
            continue;
        }
        interfacePtr->mInfoPtr->mAddr = str;

        pAddress = (sockaddr_in*)&(InterfaceList[i].iiBroadcastAddress);
        result = inet_ntop(AF_INET, &(pAddress->sin_addr), str, INET_ADDRSTRLEN);
        if (result == NULL)
        {
            std::cout << "Failed to convert broadcast address to string. " << "Errno: " << std::strerror(errno) << std::endl;
            continue;
        }
        interfacePtr->mInfoPtr->mBroadcastAddr = str;

        pAddress = (sockaddr_in*)&(InterfaceList[i].iiNetmask);
        result = inet_ntop(AF_INET, &(pAddress->sin_addr), str, INET_ADDRSTRLEN);
        if (result == NULL)
        {
            std::cout << "Failed to convert netmask to string. " << "Errno: " << std::strerror(errno) << std::endl;
            continue;
        }
        interfacePtr->mInfoPtr->mNetmask = str;

        unsigned long flags = InterfaceList[i].iiFlags;

        interfacePtr->mInfoPtr->mIsUp = ((flags & IFF_UP) != 0);
        interfacePtr->mInfoPtr->mIsLoopback = ((flags & IFF_LOOPBACK) != 0);
        interfacePtr->mInfoPtr->mIsPointToPoint = ((flags & IFF_POINTTOPOINT) != 0);
        interfacePtr->mInfoPtr->mHasBroadcast = ((flags & IFF_BROADCAST) != 0);
        interfacePtr->mInfoPtr->mHasMulticast = ((flags & IFF_MULTICAST) != 0);

        mInterfaces.push_back(interfacePtr);
    }
#elif defined(__linux__)
    struct ifaddrs* ifaddr = nullptr;
    struct ifaddrs* ifa = nullptr;

    if (getifaddrs(&ifaddr) == -1)
    {
        auto out = ut::log::error() << "Failed calling getifaddrs.";
        out.AddNote() << "Errno: " << std::strerror(errno);
    }

    // Walk through linked list
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == nullptr)
        {
            continue;
        }

        int family = ifa->ifa_addr->sa_family;

        // For an AF_INET* interface address, display the address */
        if (family == AF_INET) // restrict ourselves to IPV4 for now || family == AF_INET6)
        {
            numInterfaces += 1;

            Interface* interfacePtr = new Interface();

            unsigned int flags = ifa->ifa_flags;
            interfacePtr->mInfoPtr->mIsUp = ((flags & IFF_UP) != 0);
            interfacePtr->mInfoPtr->mHasBroadcast = ((flags & IFF_BROADCAST) != 0);
            interfacePtr->mInfoPtr->mIsLoopback = ((flags & IFF_LOOPBACK) != 0);
            interfacePtr->mInfoPtr->mIsPointToPoint = ((flags & IFF_POINTOPOINT) != 0);
            interfacePtr->mInfoPtr->mHasMulticast = ((flags & IFF_MULTICAST) != 0);

            GetNameInfo(ifa->ifa_addr, family, interfacePtr->mInfoPtr->mAddr);
            GetNameInfo(ifa->ifa_netmask, family, interfacePtr->mInfoPtr->mNetmask);
            GetNameInfo(ifa->ifa_broadaddr, family, interfacePtr->mInfoPtr->mBroadcastAddr);

            mInterfaces.push_back(interfacePtr);
        }
    }

    freeifaddrs(ifaddr);
#endif
}

// static
//! Print all information about this machine's interfaces to stdout.
void GenNetInfo::Print()
{
    int numInterfaces = GetInterfaceCount();

    std::cout << "GenNetInfo Data Dump:" << std::endl;

    for (int i = 0; i < numInterfaces; ++i)
    {
        Interface* interfacePtr = GetInstance().mInterfaces[i];
        std::cout << "Interface " << i << ":" << std::boolalpha
                  << "Address: " << interfacePtr->mInfoPtr->mAddr
                  << "Broadcast Address: " << interfacePtr->mInfoPtr->mBroadcastAddr
                  << "Netmask: " << interfacePtr->mInfoPtr->mNetmask
                  << "Up: " << interfacePtr->mInfoPtr->mIsUp
                  << "Point-to-Point: " << interfacePtr->mInfoPtr->mIsPointToPoint
                  << "Loopback: " << interfacePtr->mInfoPtr->mIsLoopback
                  << "Has Broadcast: " << interfacePtr->mInfoPtr->mHasBroadcast
                  << "Has Multicast: " << interfacePtr->mInfoPtr->mHasMulticast
                  << std::endl;
    }
}

GenNetInfo::~GenNetInfo() {}

// static private
GenNetInfo& GenNetInfo::GetInstance()
{
    static GenNetInfo sInfo;
    return sInfo;
}

GenNetInfo::Interface::Interface()
    : mInfoPtr(new InfoImpl())
{
}

GenNetInfo::Interface::~Interface()
{
    delete mInfoPtr;
}

// static
//! Retrieve "aInterfaceNumber" in the list of interfaces that are broadcast interfaces.
//! For example, calling GetBroadcastInterface(1) will return the second broadcast capable interface in
//! the interface list, while GetBroadcastInterface(0) or GetBroadcastInterface(), returns the first such interface.
//! @return The specified interface, or zero if the interface does not exist.
const GenNetInfo::Interface* GenNetInfo::GetBroadcastInterface(unsigned aInterfaceNumber) // = 0
{
    unsigned interfaceNum = 0;
    const Interface* interfacePtr = nullptr;
    const Interface* broadcastInterfacePtr = nullptr;
    unsigned hasBroadcastNum = 0;

    while (interfaceNum < GetInterfaceCount())
    {
        interfacePtr = GenNetInfo::GetInterfaceEntry(interfaceNum);
        if ((interfacePtr != nullptr) && (interfacePtr->HasBroadcast()))
        {
            if (hasBroadcastNum == aInterfaceNumber)
            {
                broadcastInterfacePtr = interfacePtr;
                break;
            }
            else
            {
                ++hasBroadcastNum;
            }
        }
        ++interfaceNum;
    }

    return broadcastInterfacePtr;
}

// static
//! Retrieve "aInterfaceNumber" in the list of interfaces that are multicast interfaces.
//! For example, calling GetMulticastInterface(1) will return the second multicast capable interface in
//! the interface list, while GetMulticastInterface(0) or GetMulticastInterface(), returns the first such interface.
//! @return The specified interface, or zero if the interface does not exist.
const GenNetInfo::Interface* GenNetInfo::GetMulticastInterface(unsigned aInterfaceNumber) // = 0
{
    unsigned interfaceNum = 0;
    const Interface* interfacePtr = nullptr;
    const Interface* multicastInterfacePtr = nullptr;
    unsigned hasmulticastNum = 0;

    while (interfaceNum < GetInterfaceCount())
    {
        interfacePtr = GenNetInfo::GetInterfaceEntry(interfaceNum);
        if ((interfacePtr != nullptr) && (interfacePtr->HasMulticast()) && (!interfacePtr->IsLoopback()))
        {
            if (hasmulticastNum == aInterfaceNumber)
            {
                multicastInterfacePtr = interfacePtr;
                break;
            }
            else
            {
                ++hasmulticastNum;
            }
        }
        ++interfaceNum;
    }

    return multicastInterfacePtr;
}

// static
//! Return the network interface information object associated with the given network controller entry.
const GenNetInfo::Interface* GenNetInfo::GetInterfaceEntry(unsigned aEntry)
{
    Interface* interfacePtr = nullptr;
    if (aEntry < GetInstance().mInterfaces.size())
    {
        interfacePtr = GetInstance().mInterfaces[aEntry];
    }
    return interfacePtr;
}

// static
//! Return the number of interfaces this machine supports.
unsigned GenNetInfo::GetInterfaceCount()
{
    return static_cast<unsigned int>(GetInstance().mInterfaces.size());
}

//! Return whether the interface is enabled.
bool GenNetInfo::Interface::IsUp() const
{
    return mInfoPtr->mIsUp;
}

//! Return whether the interface is a loopback interface.
bool GenNetInfo::Interface::IsLoopback() const
{
    return mInfoPtr->mIsLoopback;
}

//! Return whether the interface has broadcast capability.
bool GenNetInfo::Interface::HasBroadcast() const
{
    return mInfoPtr->mHasBroadcast;
}

//! Return whether the interface has multicast capability.
bool GenNetInfo::Interface::HasMulticast() const
{
    return mInfoPtr->mHasMulticast;
}

//! Return the broadcast address associated with this interface.
//! @note The return value is only valid if Interface::HasBroadcas() is true.
const std::string& GenNetInfo::Interface::GetBroadcastAddr() const
{
    return mInfoPtr->mBroadcastAddr;
}

//! Return the network address assoicated with this interface.
const std::string& GenNetInfo::Interface::GetAddress() const
{
    return mInfoPtr->mAddr;
}

//! Return the netmask associated with this interface.
const std::string& GenNetInfo::Interface::GetNetmask() const
{
    return mInfoPtr->mNetmask;
}
