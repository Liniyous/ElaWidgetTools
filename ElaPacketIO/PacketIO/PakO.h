#ifndef PAKBUFO_ARCHIVE_H
#define PAKBUFO_ARCHIVE_H

#include "ElaPacketIO_Export.h"

#include <string>

#include "GenBuffer.h"
class PakI;

//! Serializes data to a GenO
class ELA_PACKETIO_EXPORT PakO
{
public:
    static const bool cIS_OUTPUT = true;
    typedef PakO OutputArchive;
    typedef PakI InputArchive;

    PakO(GenBuffer* aBufferPtr);

    GenBuffer* GetBuffer() { return mBufferPtr; }

    void IgnoreBytes(int aNumBytes)
    {
        mBufferPtr->CheckPutSpace(aNumBytes);
        mBufferPtr->SetPutPos(mBufferPtr->GetPutPos() + aNumBytes);
    }

    template <typename T>
    void Serialize(const T& aVal)
    {
        mBufferPtr->putValue(aVal);
    }

    void SerializeBuffer(char* aBuffer, int aSize) { mBufferPtr->PutRaw(aBuffer, aSize); }

    void SerializeString(std::string& aString, int aBytes);

protected:
    GenBuffer* mBufferPtr;
};

#endif
