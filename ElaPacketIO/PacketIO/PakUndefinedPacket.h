#ifndef PAKUNDEFINEDPACKET_H
#define PAKUNDEFINEDPACKET_H

#include "ElaPacketIO_Export.h"

#include "PakPacket.h"

class ELA_PACKETIO_EXPORT PakUndefinedPacket : public PakPacket
{
public:
    PakUndefinedPacket()
        : PakPacket(cPACKET_ID), mPacketBuffer(nullptr)
    {
    }
    ~PakUndefinedPacket() override;
    template <class T>
    void Serialize(T& aBuff)
    {
        aBuff.SerializeBuffer(mPacketBuffer + 8, mPacketLength - 8);
    }
    void Initialize(int aPacketID, int aPacketLength);

    virtual PakPacket* New() { return new PakUndefinedPacket; }

    // This packet has no constant ID, the ID could be anything.
    enum
    {
        cPACKET_ID = 0
    };

    int mPacketLength;
    char* mPacketBuffer;
};

#endif
