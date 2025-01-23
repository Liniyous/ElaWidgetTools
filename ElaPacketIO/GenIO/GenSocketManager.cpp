#include "GenSocketManager.h"

#include <cstdio>
#include <iostream>
#include <list>
#ifdef __linux__
#include <net/if.h>
#endif

#include "GenHostName.h"
#include "GenSocketIncludes.h"
namespace GenSockets
{

void GenSocketManager::Ready()
{
    GetInstance().Startup();
}

GenSocketManager::GenSocketManager()
{
    mIsInitialized = false;
}
GenSocketManager::~GenSocketManager()
{
    Shutdown();
}
void GenSocketManager::Startup()
{
    if (!mIsInitialized)
    {
        mIsInitialized = true;
#ifdef _WIN32
        int err;
        WSADATA wsaData;
        WORD wVersionRequested = MAKEWORD(2, 0);
        err = WSAStartup(wVersionRequested, &wsaData);
#endif
    }
}

void GenSocketManager::Shutdown()
{
    if (mIsInitialized)
    {
        mIsInitialized = false;
#ifdef _WIN32
        WSACleanup();
#endif
    }
}

// static
GenSocketManager& GenSocketManager::GetInstance()
{
    static GenSocketManager i;
    return i;
}

// static
std::string GenSocketManager::GetLastError()
{
    std::string lReturn;
#ifdef _WIN32
    int error = WSAGetLastError();
    switch (error)
    {
    case WSAECONNREFUSED:
        lReturn = "No connection could be made because the target machine actively refused it.";
        break;
    case WSANOTINITIALISED:
        lReturn = "A successful WSAStartup must occur before using this function.";
        break;
    case WSAENETDOWN:
        lReturn = "The network subsystem has failed.";
        break;
    case WSAEACCES:
        lReturn = "The requested address is a broadcast address, but the "
                  "appropriate flag was not set. ";
        break;
    case WSAEINPROGRESS:
        lReturn = "A blocking Windows Sockets 1.1 call is in progress, "
                  "or the service provider is still processing a callback "
                  "function.";
        break;
    case WSAEFAULT:
        lReturn = "The buf parameter is not totally contained in a valid "
                  "part of the user address space.";
        break;
    case WSAENETRESET:
        lReturn = "The connection has been broken due to the remote host "
                  "resetting. ";
        break;
    case WSAENOBUFS:
        lReturn = "No buffer space is available.";
        break;
    case WSAENOTCONN:
        lReturn = "The socket is not connected.";
        break;
    case WSAENOTSOCK:
        lReturn = "The descriptor is not a socket.";
        break;
    case WSAEOPNOTSUPP:
        lReturn = "MSG_OOB was specified, but the socket is not stream style "
                  "such as type SOCK_STREAM, out-of-band data is not "
                  "supported in the communication domain associated with "
                  "this socket, or the socket is unidirectional and "
                  "supports only receive operations.";
        break;
    case WSAESHUTDOWN:
        lReturn = "The socket has been shut down; it is not possible to send "
                  "on a socket after shutdown has been invoked with how "
                  "set to SD_SEND or SD_BOTH.";
        break;
    case WSAEWOULDBLOCK:
        lReturn = "The socket is marked as nonblocking and the requested "
                  "operation would block.";
        break;
    case WSAEMSGSIZE:
        lReturn = "The socket is message oriented, and the message is "
                  "larger than the maximum supported by the underlying "
                  "transport.";
        break;
    case WSAEHOSTUNREACH:
        lReturn = "The remote host cannot be reached from this host at "
                  "this time.";
        break;
    case WSAEINVAL:
        lReturn = "The socket has not been bound with bind, or an unknown "
                  "flag was specified, or MSG_OOB was specified for a "
                  "socket with SO_OOBINLINE enabled.";
        break;
    case WSAECONNABORTED:
        lReturn = "The virtual circuit was terminated due to a time-out "
                  "or other failure. The application should close the "
                  "socket as it is no longer usable.";
        break;
    case WSAECONNRESET:
        lReturn = "The virtual circuit was reset by the remote side "
                  "executing a hard or abortive close.  "
                  "For UPD sockets, the remote host was unable to "
                  "deliver a previously sent UDP datagram and responded "
                  "with a Port Unreachable ICMP packet. The application "
                  "should close the socket as it is no longer usable.";
        break;
    case WSAETIMEDOUT:
        lReturn = "The connection has been dropped, because of a network "
                  "failure or because the system on the other end went "
                  "down without notice.";
        break;
    }
#else
    lReturn = strerror(errno);
#endif
    return lReturn;
}

// static
void GenSocketManager::PrintLastError(const std::string& aHeader)
{
    std::cout << aHeader << ":" << "Error: " << GetLastError() << std::endl;
}
#ifdef __linux__
bool FindInterfaceIP(const std::string& aInterfaceName, GenIP& aIP)
{
    bool lReturn = false;
    int sfd;
    struct ifreq ifr;
    struct sockaddr_in* sin = (struct sockaddr_in*)&ifr.ifr_addr;

    memset(&ifr, 0, sizeof(ifr));

    if (0 > (sfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        auto out = ut::log::error() << "socket() failed.";
        out.AddNote() << "Errno: " << std::strerror(errno);
    }
    else
    {
        strcpy(ifr.ifr_name, aInterfaceName.c_str());
        sin->sin_family = AF_INET;
        if (0 == ioctl(sfd, SIOCGIFADDR, &ifr))
        {
            aIP = sin->sin_addr;
            lReturn = true;
        }
        close(sfd);
    }
    return lReturn;
}

void GenSocketManager::InsertIP(const GenIP& aIP)
{
    std::vector<GenIP>::iterator i = std::find(mIP_List.begin(), mIP_List.end(), aIP);
    if (i == mIP_List.end())
    {
        mIP_List.push_back(aIP);
    }
}
#endif

void GenSocketManager::CreateIP_List()
{
    if (mIP_List.size() == 0)
    {
        GenHostName hname = GenHostName::LocalHostName();
        hname.GetIP_List(mIP_List);
#ifdef __linux__
        GenIP ip;
        for (char i = '0'; i < '9'; ++i)
        {
            std::string eth = "eth";
            eth += i;
            if (FindInterfaceIP(eth, ip))
            {
                InsertIP(ip);
            }
        }
        if (FindInterfaceIP("lo", ip))
        {
            InsertIP(ip);
        }
#endif
    }
}

// static
//! Returns the number of IP's the local machine has.
int GenSocketManager::GetIP_Count()
{
    GetInstance().CreateIP_List();
    return static_cast<int>(GetInstance().mIP_List.size());
}

// static
//! Returns an IP of the local machine
//! @param aIndex The index of the IP to return
//! @return An IP of the local machine
GenIP GenSocketManager::GetIP(int aIndex)
{
    GetInstance().CreateIP_List();
    return GetInstance().mIP_List[aIndex];
}

// static
//! Returns the local IP of this machine related to an interface
//! @param aInterfaceIP The interface.  Ex:
//!                     192 or 192.168 or 192.168.1 or 192.168.1.1
//! @return Returns the local IP on this interface.  Ex: 192.168.1.1
GenIP GenSocketManager::GetInterfaceLocalIP(const std::string& aInterfaceIP)
{
    GenIP lReturn;
    std::list<GenIP> goodIPs;
    int ipParts[4];
    int count = sscanf(aInterfaceIP.c_str(), "%i.%i.%i.%i", &ipParts[0], &ipParts[1], &ipParts[2], &ipParts[3]);
    for (int i = 0; i < count; ++i)
    {
        if (i == 0)
        {
            for (int j = 0; j < GetIP_Count(); ++j)
            {
                GenIP ip = GetIP(j);
                if (static_cast<unsigned char>(ipParts[0]) == ip.GetAddressPart(0))
                {
                    goodIPs.push_back(ip);
                }
            }
        }
        else
        {
            if (goodIPs.size() <= 1)
            {
                break;
            }
            std::list<GenIP>::iterator iter = goodIPs.begin(), delIter;
            for (; iter != goodIPs.end();)
            {
                if (static_cast<unsigned char>(ipParts[i]) != (*iter).GetAddressPart(i))
                {
                    delIter = iter++;
                    goodIPs.erase(delIter);
                }
                else
                {
                    ++iter;
                }
            }
        }
    }
    if (goodIPs.size())
    {
        lReturn = goodIPs.front();
    }
    return lReturn;
}

} // namespace GenSockets
