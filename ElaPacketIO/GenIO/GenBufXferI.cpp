#include "GenBufXferI.h"

#include <algorithm>
#include <cstring>

#include "GenIConvertBigEndian.h"
#include "GenIConvertLitEndian.h"
#include "GenIConvertLitEndianVax.h"

// This class inherits from GenBuf

GenBufXferI::GenBufXferI(BufType aBufType, bool aAllowGrowth)
    : GenBuf(0, (unsigned int)(aAllowGrowth ? 1 : 0))
{
    BuildGenIConvert(aBufType);
}

// Virtual
GenBufXferI::~GenBufXferI()
{
    delete mGenIConvert;
}

// Returned count value includes terminating null zero
int GenBufXferI::Get(char* aValue, unsigned long& aBytesSeen)
{
    unsigned char* ptr = mCurGet;

    while (ptr < mDatEnd)
    {
        if (*ptr == '\0')
        {
            aBytesSeen = static_cast<long>(ptr - mCurGet + 1);
            break;
        }
        ++ptr;
    }

    // If not enough data was found, clean up and return
    if (ptr == mDatEnd)
    {
        aBytesSeen = static_cast<long>(mDatEnd - mCurGet);

        // Force aBytesSeen to be at least one, because grow logic doesn't
        // work well when asked to increase size by zero
        if (aBytesSeen == 0)
        {
            aBytesSeen = 1;
        }
        return NotEnoughData;
    }

    // If we got here things are good, so copy the characters.
    memcpy(aValue, mCurGet, aBytesSeen);
    IncrementGetIndex(aBytesSeen);

    return static_cast<int>(aBytesSeen);
}

int GenBufXferI::GetRaw(void* aDataPtr, unsigned long aDataSizeBytes)
{
    int status = EnsureGetSpace(aDataSizeBytes);
    if (status > 0)
    {
        std::copy(reinterpret_cast<char*>(mCurGet),
                  reinterpret_cast<char*>(mCurGet) + aDataSizeBytes,
                  reinterpret_cast<char*>(aDataPtr));

        IncrementGetIndex(aDataSizeBytes);
        return static_cast<int>(aDataSizeBytes);
    }
    else
    {
        return status;
    }
}

int GenBufXferI::GetBufferIfValid(const void*& aBufferPtr, unsigned long aDataSizeBytes)
{
    int status = EnsureGetSpace(aDataSizeBytes);
    if (status > 0)
    {
        aBufferPtr = mCurGet;
        IncrementGetIndex(aDataSizeBytes);
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

void GenBufXferI::BuildGenIConvert(BufType aBufType)
{
    switch (aBufType)
    {
    case BigEndian:
        mGenIConvert = new GenIConvertBigEndian;
        break;

    case LittleEndian:
        mGenIConvert = new GenIConvertLitEndian;
        break;

    case LittleEndianVax:
        mGenIConvert = new GenIConvertLitEndianVax;
        break;

    case Native:
        // Note order of "#if" is important.  A Vax
        // is a GENIO_VAX_G_FLOAT and a GENIO_LIT_ENDIAN.
#if defined(GENIO_VAX_G_FLOAT)
        mGenIConvert = new GenIConvertLitEndianVax;
#elif defined(GENIO_LIT_ENDIAN)
        mGenIConvert = new GenIConvertLitEndian;
#elif defined(GENIO_BIG_ENDIAN)
        mGenIConvert = new GenIConvertBigEndian;
#endif
        break;

    case Null:
        mGenIConvert = nullptr;
        break;

    default:
        // Better never get here
        mGenIConvert = nullptr;
        break;
    }
}

int GenBufXferI::GetCharArray(void* aArray, unsigned int aArrayLen)
{
    int status = EnsureGetSpace(aArrayLen);
    if (status > 0)
    {
        std::copy_n(mCurGet,                              // Buffer to copied from
                    aArrayLen,                            // Size of data to be copied
                    static_cast<unsigned char*>(aArray)); // New buffer
        IncrementGetIndex(aArrayLen);
        return static_cast<int>(aArrayLen);
    }

    return status;
}
