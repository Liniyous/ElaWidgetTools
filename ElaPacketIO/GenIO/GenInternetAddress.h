#ifndef GENINTERNETADDRESS_H
#define GENINTERNETADDRESS_H

#include <string>
struct in_addr;

#include "ElaPacketIO_Export.h"

namespace GenSockets
{
class GenIP;
class GenHostName;

class ELA_PACKETIO_EXPORT GenInternetAddress
{
public:
    GenInternetAddress(const GenIP& aIP);
    GenInternetAddress(const GenHostName& aHostName);
    GenInternetAddress(const std::string& aAddrStr);
    GenInternetAddress(const GenInternetAddress& aSrc);
    GenInternetAddress& operator=(const GenInternetAddress& aAddr);
    bool operator<(const GenInternetAddress& aRhs) const;
    bool operator==(const GenInternetAddress& aAddr) const;
    //! Sets address = INADDR_ANY
    GenInternetAddress();
    ~GenInternetAddress();
    operator GenHostName() const;
    operator GenIP() const;
    const in_addr* GetInAddr() const { return mInAddr; }
    bool IsValid();

protected:
    in_addr* mInAddr;
};
} // namespace GenSockets
#endif
