#include "PakSocketIO.h"

#include "PakHeader.h"

PakSocketIO::~PakSocketIO()
{
    delete mPacketHeaderType;
}

//! Overwrites the ID and length field in the packet header.
//! @param aIO A GenIO filled with the packet.
//! @param aPacketID The packet's ID.
void PakSocketIO::SetPacketHeader(GenBuffer& aBuffer, int aPacketID, int aPacketLength)
{
    if (mPacketHeaderType)
    {
        mPacketHeaderType->WriteHeader(aBuffer, aPacketID, aPacketLength);
    }
}

bool PakSocketIO::GetPacketHeader(GenBuffer& aBuffer, int& aPacketID, int& aPacketLength, bool& aIsInvalid)
{
    bool lReturn = true;
    if (mPacketHeaderType != nullptr)
    {
        size_t hsize = (size_t)mPacketHeaderType->GetHeaderSize();
        if (aBuffer.GetPutPos() - aBuffer.GetGetPos() >= hsize)
        {
            aIsInvalid = mPacketHeaderType->ReadHeader(aBuffer, aPacketID, aPacketLength);
        }
        else
        {
            lReturn = false;
            aIsInvalid = false;
        }
    }
    else
    {
        aPacketID = 0;
        aPacketLength = 0;
        aIsInvalid = true;
    }
    return lReturn;
}

int PakSocketIO::GetHeaderSize()
{
    int hSize = 0;
    if (mPacketHeaderType)
    {
        hSize = mPacketHeaderType->GetHeaderSize();
    }
    return hSize;
}
