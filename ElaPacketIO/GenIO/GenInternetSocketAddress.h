#ifndef GENINTERNETSOCKETADDRESS_H
#define GENINTERNETSOCKETADDRESS_H

#include <string>
struct sockaddr_in;

#include "ElaPacketIO_Export.h"

#include "GenInternetAddress.h"

namespace GenSockets
{
/** A SocketAddress and port number combined -- everything needed to
    specify a message destination.
*/
class ELA_PACKETIO_EXPORT GenInternetSocketAddress
{
public:
    friend class GenSocket;

    GenInternetSocketAddress(const std::string& aAddrStr);
    GenInternetSocketAddress(const GenInternetAddress& aAddr, int aPort);
    GenInternetSocketAddress(sockaddr_in& aAddr);
    GenInternetSocketAddress(const GenInternetSocketAddress& aSrc);
    GenInternetSocketAddress();
    ~GenInternetSocketAddress();
    GenInternetSocketAddress& operator=(const GenInternetSocketAddress& aSrc);
    bool operator==(const GenInternetSocketAddress& aRhs) const;
    bool operator<(const GenInternetSocketAddress& aRhs) const;
    int GetPort() const;
    void SetPort(int aPort);
    GenInternetAddress GetAddress() const;
    bool IsValid() const;
    operator std::string() const;

protected:
    sockaddr_in* GetSockAddr() { return mSockAddr; }

    sockaddr_in* mSockAddr;
};
} // namespace GenSockets

#endif
