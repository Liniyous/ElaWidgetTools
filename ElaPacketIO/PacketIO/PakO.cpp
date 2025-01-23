#include "PakO.h"

PakO::PakO(GenBuffer* aBufferPtr)
    : mBufferPtr(aBufferPtr)
{
}

// virtual
//! Serializes a fixed-sized string.
//! @param aString The text to serialize
//! @param aBytes The number of bytes the string will fill.
void PakO::SerializeString(std::string& aString, int aBytes)
{
    //_byteswap_ulong()
    int len = static_cast<int>(aString.length());
    if (len < aBytes)
    {
        mBufferPtr->PutRaw(aString.c_str(), len);
        while (len < aBytes)
        {
            mBufferPtr->putValue('\0');
            ++len;
        }
    }
    else
    {
        mBufferPtr->PutRaw(aString.c_str(), aBytes - 1);
        mBufferPtr->putValue('\0');
    }
}
