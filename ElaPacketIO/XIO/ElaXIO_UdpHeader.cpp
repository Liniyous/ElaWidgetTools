#include "xio/ElaXIO_UdpHeader.h"
#include <iostream>

#include "GenI.h"
#include "GenO.h"
ElaXIO_UdpHeader::ElaXIO_UdpHeader(int aVersionID)
    : mXIO_Version(0xfeedbeef ^ aVersionID)
{
    mReceivedInvalidPacket = false;
}

ElaXIO_UdpHeader::ElaXIO_UdpHeader(const ElaXIO_UdpHeader& aSrc)
    : PakHeader(aSrc),
      mXIO_Version(aSrc.mXIO_Version),
      mReceivedInvalidPacket(aSrc.mReceivedInvalidPacket)
{
}

PakHeader* ElaXIO_UdpHeader::Clone() const
{
    return new ElaXIO_UdpHeader(*this);
}

void ElaXIO_UdpHeader::WriteHeader(GenBuffer& aIO, int aPacketID, int aPacketLength)
{
    aIO.putValue(mXIO_Version);
    aIO.putValue(static_cast<int32_t>(aPacketLength - sizeof(header)));
    aIO.putValue(static_cast<int32_t>(aPacketID));
}

bool ElaXIO_UdpHeader::ReadHeader(GenBuffer& aIO, int& aPacketID, int& aPacketLength)
{
    uint32_t version;
    int32_t length, id;
    aIO.getValue(version);
    aIO.getValue(length);
    aPacketLength = length + sizeof(header);
    aIO.getValue(id);
    aPacketID = id;
    bool valid = (version == mXIO_Version);
    if (!valid && !mReceivedInvalidPacket)
    {
        mReceivedInvalidPacket = true;
        std::cout << "xio_interface: Received PDU with invalid version." << "XIO versions may not match." << std::endl;
    }
    return valid;
}

int ElaXIO_UdpHeader::GetHeaderSize()
{
    return sizeof(header);
}
