#ifndef PAKBUFI_ARCHIVE_H
#define PAKBUFI_ARCHIVE_H

#include "ElaPacketIO_Export.h"

#include <string>

#include "GenBuffer.h"

class PakBufO_Archive;

//! Serializes data from a GenI
class ELA_PACKETIO_EXPORT PakI
{
public:
    static const bool cIS_OUTPUT = false;
    typedef PakBufO_Archive OutputArchive;
    typedef PakI InputArchive;

    PakI(GenBuffer* aBufferPtr);

    GenBuffer* GetBuffer() { return mBufferPtr; }

    void IgnoreBytes(int aNumBytes) { mBufferPtr->SetGetPos(mBufferPtr->GetGetPos() + aNumBytes); }

    template <typename T>
    void Serialize(T& aVal)
    {
        mBufferPtr->getValue(aVal);
    }

    void SerializeBuffer(char* aBuffer, int aSize) { mBufferPtr->GetRaw(aBuffer, aSize); }

    void SerializeString(std::string& aString, int aBytes);

protected:
    GenBuffer* mBufferPtr;
};

#endif
