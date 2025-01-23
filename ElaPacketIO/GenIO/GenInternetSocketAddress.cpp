#include "GenInternetSocketAddress.h"

#include <cstring>
#include <sstream>

#include "GenHostName.h"
#include "GenIP.h"
#include "GenSocketIncludes.h"

namespace GenSockets
{
//! Creates a GenInternetSocketAddress in the form: "Hostname:Port"
GenInternetSocketAddress::GenInternetSocketAddress(const std::string& aAddrStr)
{
    mSockAddr = new sockaddr_in;

    bool ok = false;
    size_t colonPos = aAddrStr.find(':');
    if (colonPos != std::string::npos)
    {
        GenInternetAddress addr(aAddrStr.substr(0, colonPos));
        std::stringstream convStream(aAddrStr.substr(colonPos + 1));
        int port(0);
        if ((convStream >> port) && port > 0)
        {
            ok = true;
            *this = GenInternetSocketAddress(addr, port);
        }
    }
    if (!ok)
    {
        *this = GenInternetSocketAddress(GenInternetAddress(), 0);
    }
}

GenInternetSocketAddress::GenInternetSocketAddress(const GenInternetAddress& aAddr, int aPort)
{
    mSockAddr = new sockaddr_in;
    memcpy(&mSockAddr->sin_addr, aAddr.GetInAddr(), sizeof(in_addr));
    mSockAddr->sin_family = AF_INET;
    mSockAddr->sin_port = htons(aPort);
    memset(&mSockAddr->sin_zero, 0, sizeof(mSockAddr->sin_zero));
}

GenInternetSocketAddress::GenInternetSocketAddress(sockaddr_in& aAddr)
{
    mSockAddr = new sockaddr_in;
    *mSockAddr = aAddr;
}

GenInternetSocketAddress::GenInternetSocketAddress(const GenInternetSocketAddress& aSrc)
{
    mSockAddr = new sockaddr_in;
    memcpy(mSockAddr, aSrc.mSockAddr, sizeof(sockaddr_in));
}

GenInternetSocketAddress::GenInternetSocketAddress()
{
    mSockAddr = new sockaddr_in;
    mSockAddr->sin_addr.s_addr = INADDR_ANY;
    mSockAddr->sin_family = AF_INET;
    mSockAddr->sin_port = 0;
    memset(&mSockAddr->sin_zero, 0, sizeof(mSockAddr->sin_zero));
}

GenInternetSocketAddress::~GenInternetSocketAddress()
{
    delete mSockAddr;
}
GenInternetSocketAddress& GenInternetSocketAddress::operator=(const GenInternetSocketAddress& aSrc)
{
    memcpy(mSockAddr, aSrc.mSockAddr, sizeof(sockaddr_in));
    return *this;
}

//! Returns the port related to this GenInternetSocketAddress
int GenInternetSocketAddress::GetPort() const
{
    return ntohs(mSockAddr->sin_port);
}

//! Sets the port
//! @param aPort The port value
void GenInternetSocketAddress::SetPort(int aPort)
{
    mSockAddr->sin_port = htons(aPort);
}

//! Returns the GenInternetAddress related to this GenInternetSocketAddress
GenInternetAddress GenInternetSocketAddress::GetAddress() const
{
    return GenInternetAddress(mSockAddr->sin_addr);
}

bool GenInternetSocketAddress::IsValid() const
{
    return GetAddress().IsValid();
}

//! Returns a string representation of the GenInternetSocketAddress in the form 'host:port'
GenInternetSocketAddress::operator std::string() const
{
    std::string host = (GenHostName)GetAddress();
    std::stringstream convStream;
    convStream << host << ':' << GetPort();
    std::string inetAddrString;
    inetAddrString = convStream.str();
    return inetAddrString;
}

bool GenInternetSocketAddress::operator==(const GenInternetSocketAddress& aRhs) const
{
    return GetPort() == aRhs.GetPort() && mSockAddr->sin_addr.s_addr == aRhs.mSockAddr->sin_addr.s_addr;
}

bool GenInternetSocketAddress::operator<(const GenInternetSocketAddress& aRhs) const
{
    int port = GetPort();
    int rhsPort = aRhs.GetPort();
    if (port < rhsPort)
    {
        return true;
    }
    else if (rhsPort < port)
    {
        return false;
    }
    return mSockAddr->sin_addr.s_addr < aRhs.mSockAddr->sin_addr.s_addr;
}

} // namespace GenSockets
