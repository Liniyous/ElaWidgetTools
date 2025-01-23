#include "GenBufXferO.h"

#include <cstring>

#include "GenOConvertBigEndian.h"
#include "GenOConvertLitEndian.h"
#include "GenOConvertLitEndianVax.h"

// This class inherits from GenBuf

GenBufXferO::GenBufXferO(BufType aBufType)
    : GenBuf(0)
{
    BuildGenOConvert(aBufType);
}

GenBufXferO::~GenBufXferO()
{
    delete mGenOConvert;
}

// Returned count value includes terminating null zero
int GenBufXferO::Put(const char* aValue, unsigned long& aLength)
{
    const char* ptr = aValue;
    while (*ptr != '\0')
    {
        ++ptr;
    }

    aLength = static_cast<long>(ptr - aValue + 1);
    return PutCharArray(aValue, static_cast<unsigned int>(aLength));
}

int GenBufXferO::PutRaw(const void* aDataPtr, unsigned long aDataSizeBytes)
{
    int status = EnsurePutSpace(aDataSizeBytes);
    if (status > 0)
    {
        memcpy(mCurPut, aDataPtr, aDataSizeBytes);
        IncrementPutIndex(aDataSizeBytes);
        return static_cast<int>(aDataSizeBytes);
    }
    else
    {
        return status;
    }
}

int GenBufXferO::GetBufferIfValid(void*& aBufferPtr, unsigned long aDataSizeBytes)
{
    int status = EnsurePutSpace(aDataSizeBytes);
    if (status > 0)
    {
        aBufferPtr = mCurPut;
        IncrementPutIndex(aDataSizeBytes);
        return static_cast<int>(aDataSizeBytes);
    }
    else
    {
        return status;
    }
}

//
// Private
//

int GenBufXferO::PutCharArray(const void* aArray, unsigned int aArrayLen)
{
    int status = EnsurePutSpace(aArrayLen);
    if (status > 0)
    {
        memcpy(mCurPut, aArray, aArrayLen);
        IncrementPutIndex(aArrayLen);
        return static_cast<int>(aArrayLen);
    }
    else
    {
        return status;
    }
}

void GenBufXferO::BuildGenOConvert(BufType aBufType)
{
    switch (aBufType)
    {
    case BigEndian:
        mGenOConvert = new GenOConvertBigEndian;
        break;

    case LittleEndian:
        mGenOConvert = new GenOConvertLitEndian;
        break;

    case LittleEndianVax:
        mGenOConvert = new GenOConvertLitEndianVax;
        break;

    case Native:
        // Note order of "#if" is important.  A Vax
        // is a GENIO_VAX_G_FLOAT and a GENIO_LIT_ENDIAN.
#if defined(GENIO_VAX_G_FLOAT)
        mGenOConvert = new GenOConvertLitEndianVax;
#elif defined(GENIO_LIT_ENDIAN)
        mGenOConvert = new GenOConvertLitEndian;
#elif defined(GENIO_BIG_ENDIAN)
        mGenOConvert = new GenOConvertBigEndian;
#endif
        break;

    case Null:
        mGenOConvert = nullptr;
        break;

    default:
        // Better never get here
        mGenOConvert = nullptr;
        break;
    }
}
