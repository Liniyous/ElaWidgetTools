#include "PakPacket.h"
#include "PakSerialize.h"
//! Constructor
//! @param aPacketID ID of the packet
PakPacket::PakPacket(int aPacketID)
    : mPacketID(aPacketID), mConnectionPtr(nullptr)
{
}
