#include "PakDefaultHeader.h"

// virtual
PakDefaultHeader::~PakDefaultHeader() {}

// virtual
PakDefaultHeader::PakHeader* PakDefaultHeader::Clone() const
{
    return new PakDefaultHeader;
}

// virtual
void PakDefaultHeader::WriteHeader(GenBuffer& aIO, int aPacketID, int aPacketLength)
{
    aIO.putValue(static_cast<int32_t>(aPacketLength - 8));
    aIO.putValue(static_cast<int32_t>(aPacketID));
}

// virtual
bool PakDefaultHeader::ReadHeader(GenBuffer& aIO, int& aPacketID, int& aPacketLength)
{
    int32_t id, length;
    aIO.getValue(length);
    aPacketLength = length + 8;
    aIO.getValue(id);
    aPacketID = id;
    return true;
}

// virtual
int PakDefaultHeader::GetHeaderSize()
{
    return sizeof(header);
}
