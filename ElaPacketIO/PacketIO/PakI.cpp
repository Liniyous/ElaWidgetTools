#include "PakI.h"

PakI::PakI(GenBuffer* aBufferPtr)
    : mBufferPtr(aBufferPtr)
{
}

// virtual
//! Serializes a fixed-sized string.
//! @param aString The text to serialize
//! @param aBytes The number of bytes the string will fill.
void PakI::SerializeString(std::string& aString, int aBytes)
{
    aString.clear();
    static const int cBUFF_SIZE = 1025;
    char buffer[cBUFF_SIZE];
    int length = aBytes;
    while (length > 0)
    {
        int readLength = cBUFF_SIZE - 1;
        if (length < readLength)
        {
            readLength = length;
        }
        mBufferPtr->GetRaw(buffer, readLength);
        buffer[readLength] = 0;
        aString += buffer;
        length -= readLength;
    }
}
