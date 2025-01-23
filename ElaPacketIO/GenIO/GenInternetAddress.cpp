#include "GenInternetAddress.h"

#include <cstring>

#include "GenHostName.h"
#include "GenIP.h"
#include "GenSocketIncludes.h"
#include "GenSocketManager.h"

namespace GenSockets
{
//! Constructor
GenInternetAddress::GenInternetAddress(const GenIP& aIP)
{
    GenSocketManager::Ready();
    mInAddr = new in_addr;
    aIP.Get(reinterpret_cast<unsigned char*>(&mInAddr->s_addr));
}

//! Constructor
GenInternetAddress::GenInternetAddress(const GenHostName& aHostName)
{
    GenSocketManager::Ready();
    mInAddr = new in_addr;
    std::string hostStr = aHostName;
    if (hostStr.empty())
    {
        mInAddr->s_addr = INADDR_ANY;
    }
    else
    {
        struct addrinfo* result = nullptr;
        int error = getaddrinfo(hostStr.c_str(), nullptr, nullptr, &result);
        if (error == 0 && result)
        {
            struct addrinfo* ptr = nullptr;
            struct sockaddr_in* sockaddr_ipv4;
            for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
            {
                if (ptr->ai_family == AF_INET) // IPv4
                {
                    sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
                    mInAddr->s_addr = sockaddr_ipv4->sin_addr.s_addr;
                    break; // Take first IPv4 address found
                }
            }
        }
        else
        {
            mInAddr->s_addr = INADDR_ANY;
        }
    }
}

//! Constructor
//! @param aAddrStr A string representation of a hostname or IP address
GenInternetAddress::GenInternetAddress(const std::string& aAddrStr)
{
    mInAddr = new in_addr;
    GenIP ipaddr(aAddrStr);
    if (ipaddr.IsValidForm())
    {
        *this = GenInternetAddress(ipaddr);
    }
    else
    {
        GenHostName hostName(aAddrStr);
        *this = GenInternetAddress(hostName);
    }
}

//! Constructor
//! Initialize as INADDR_ANY
GenInternetAddress::GenInternetAddress()
{
    mInAddr = new in_addr;
    mInAddr->s_addr = INADDR_ANY;
}

//! Copy Constructor
GenInternetAddress::GenInternetAddress(const GenInternetAddress& aSrc)
{
    mInAddr = new in_addr;
    memcpy(mInAddr, aSrc.mInAddr, sizeof(in_addr));
}

GenInternetAddress& GenInternetAddress::operator=(const GenInternetAddress& aAddr)
{
    memcpy(mInAddr, aAddr.mInAddr, sizeof(in_addr));
    return *this;
}

//! Returns 'true' if this GenInternetAddress is less than aRhs
bool GenInternetAddress::operator<(const GenInternetAddress& aRhs) const
{
    return (mInAddr->s_addr < aRhs.mInAddr->s_addr);
}

//!  Determine if two Internet Addresses are equal
bool GenInternetAddress::operator==(const GenInternetAddress& aAddr) const
{
    return (mInAddr->s_addr == aAddr.mInAddr->s_addr);
}

GenInternetAddress::~GenInternetAddress()
{
    delete mInAddr;
}

//! User defined conversion
//! @return A GenHostName representing this GenInternetAddress
GenInternetAddress::operator GenHostName() const
{
    return GenIP(*mInAddr);
}

//! User defined conversion
//! @return A GenIP representing this GenInternetAddress
GenInternetAddress::operator GenIP() const
{
    return GenIP(*mInAddr);
}

bool GenInternetAddress::IsValid()
{
    return mInAddr->s_addr != 0;
}

} // namespace GenSockets
