#ifndef GENIP_H
#define GENIP_H

#include <string>
struct in_addr;

#include "ElaPacketIO_Export.h"

namespace GenSockets
{
class GenHostName;

/** Represents an IP Address
 */
class ELA_PACKETIO_EXPORT GenIP
{
public:
    //! Constructor
    //! Initializes the IP to INADDR_ANY (0.0.0.0)
    GenIP() { mIPAddrDWord = 0; }

    GenIP(const std::string& aIP);

    GenIP(int aA, int aB, int aC, int aD);

    GenIP(const in_addr& aSockAddr);

    GenIP(unsigned int aAddr) { mIPAddrDWord = aAddr; }

    GenIP(const GenIP& aSrc) { mIPAddrDWord = aSrc.mIPAddrDWord; }
    GenIP& operator=(const GenIP& aIP)
    {
        mIPAddrDWord = aIP.mIPAddrDWord;
        return *this;
    }

    //! Fills char array with IP
    //! @param aIPAddr Pointer to array to be filled,
    //!                ex: unsigned char ip = {127,0,0,1};
    void Get(unsigned char* aIPAddr) const { *reinterpret_cast<unsigned int*>(aIPAddr) = mIPAddrDWord; }

    bool Parse(const std::string& aIP);

    unsigned char GetAddressPart(int aPartIndex) { return mIPAddr[aPartIndex]; }

    //! Determines if an IP is a broadcast IP
    //! @return 'true' if this a broadcast IP
    bool IsBroadcast() const { return mIPAddr[3] == 255; }

    //! Determines if an IP is a Multicast IP
    //! @return 'true' if this a Multicast IP
    bool IsMulticast() const { return (mIPAddr[0] >= 224 && mIPAddr[0] <= 239); }

    //! Determines if an IP has a valid value.
    //! Invalid IP's have the value 0.0.0.0
    //! @return 'true' if the IP is not invalid
    bool IsValidForm() const { return mIPAddrDWord != 0; }

    bool operator==(const GenIP& aIP) const { return mIPAddrDWord == aIP.mIPAddrDWord; }

    operator GenHostName() const;

    operator std::string() const;

    operator unsigned int() const { return mIPAddrDWord; }

    unsigned int GetAddress() const { return mIPAddrDWord; }

protected:
    union
    {
        unsigned char mIPAddr[4];
        unsigned int mIPAddrDWord;
    };
};
} // namespace GenSockets

#endif
