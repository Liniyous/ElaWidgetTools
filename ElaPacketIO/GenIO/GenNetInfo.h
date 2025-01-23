#ifndef GEN_NETINFO_H
#define GEN_NETINFO_H

#include "ElaPacketIO_Export.h"

#include <string>
#include <vector>

class ELA_PACKETIO_EXPORT GenNetInfo
{
    struct InfoImpl;

public:
    class ELA_PACKETIO_EXPORT Interface
    {
    public:
        Interface();
        ~Interface();

        bool HasBroadcast() const;
        bool HasMulticast() const;
        bool IsUp() const;
        bool IsLoopback() const;

        const std::string& GetAddress() const;
        const std::string& GetBroadcastAddr() const;
        const std::string& GetNetmask() const;

    private:
        friend class GenNetInfo;
        InfoImpl* mInfoPtr;
    };

public:
    ~GenNetInfo();

    //! Methods for iterating over available interfaces.
    static unsigned GetInterfaceCount();
    static const Interface* GetInterfaceEntry(unsigned aEntry);

    //! Methods for retrieving specific broadcast or multicast interfaces.
    static const Interface* GetBroadcastInterface(unsigned aInterfaceNumber = 0);
    static const Interface* GetMulticastInterface(unsigned aInterfaceNumber = 0);

    static void Print();

private:
    GenNetInfo();

    static GenNetInfo& GetInstance();

    std::vector<Interface*> mInterfaces;
};

#endif
