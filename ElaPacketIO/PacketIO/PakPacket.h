#ifndef PAKPACKET_H
#define PAKPACKET_H

#include "ElaPacketIO_Export.h"

#include "PakIntTypes.h"

class PakConnection;

class ELA_PACKETIO_EXPORT PakPacket
{
public:
    typedef bool BaseType; // No base type

    PakPacket(int aPacketID);
    virtual ~PakPacket() {}
    virtual void ReceiveCleanup() {}
    void SetSender(PakConnection* aConnection) { mConnectionPtr = aConnection; }
    PakConnection* GetSender() const { return mConnectionPtr; }
    int ID() const { return mPacketID; }

    int GetOriginatorAddress() const { return mOriginatorAddress; }
    void SetOriginatorAddress(unsigned int aAddr) { mOriginatorAddress = aAddr; }
    unsigned short GetOriginatorPort() const { return mOriginatorPort; }
    void SetOriginatorPort(unsigned short aAddr) { mOriginatorPort = aAddr; }

protected:
    int mPacketID;
    PakConnection* mConnectionPtr;
    unsigned int mOriginatorAddress;
    unsigned short mOriginatorPort;
};

#endif
