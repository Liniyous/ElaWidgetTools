#include "PakUndefinedPacket.h"

#include "PakSerialize.h"

PakUndefinedPacket::~PakUndefinedPacket()
{
    delete[] mPacketBuffer;
}
//! Before this packet can be read, it needs to be initialized with the
//! Packet ID, and size.  PacketHandlers should take care of this.
void PakUndefinedPacket::Initialize(int aPacketID, int aPacketLength)
{
    mPacketID = aPacketID;
    mPacketLength = aPacketLength;
    mPacketBuffer = new char[aPacketLength];
}
