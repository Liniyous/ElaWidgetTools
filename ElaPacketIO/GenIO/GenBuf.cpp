#include "GenBuf.h"

#include <cstring>

#ifdef _WIN32
#pragma warning(disable : 4244)
#endif

GenBuf::GenBuf(unsigned long aSize, unsigned int aAllowGrowthOnPuts)
    : mClassOwnsBuffer(1), mAllowGrowth(aAllowGrowthOnPuts)
{
    if (aSize)
    {
        unsigned long numDoubles = (aSize + 7) / 8;

        // Force mBeg to start on a double word boundary
        mBeg = reinterpret_cast<unsigned char*>(new double[numDoubles]);
        mCurPut = mBeg;
        mCurGet = mBeg;
        mDatEnd = mBeg;
        mEnd = mBeg + aSize;
    }
    else
    {
        mBeg = nullptr;
        mCurPut = nullptr;
        mCurGet = nullptr;
        mDatEnd = nullptr;
        mEnd = nullptr;
    }
}

GenBuf::GenBuf(unsigned char* aUserBuffer, unsigned long aSizeInBytes, unsigned long aNumValidBytes)
    : mBeg(aUserBuffer), mCurGet(aUserBuffer), mCurPut(aUserBuffer), mClassOwnsBuffer(0), mAllowGrowth(0)

{
    mDatEnd = mBeg + aNumValidBytes;
    mEnd = mBeg + aSizeInBytes;
}

// Virtual
GenBuf::~GenBuf()
{
    if (mClassOwnsBuffer)
    {
        delete[] mBeg;
    }
}

// Change buffer currently being used, set current byte offset to zero,
// and delete the buffer if it was initially created by this class
void GenBuf::SetLocalBuffer(unsigned char* aUserBuffer, unsigned long aSizeInBytes, unsigned long aNumValidBytes)
{
    if (mClassOwnsBuffer)
    {
        mClassOwnsBuffer = 0;
        delete[] mBeg;
    }

    mBeg = aUserBuffer;
    mCurPut = aUserBuffer;
    mCurGet = aUserBuffer;
    mDatEnd = aUserBuffer + aNumValidBytes;
    mEnd = aUserBuffer + aSizeInBytes;
}

//
// Protected Functions
//

int GenBuf::SetOffset(unsigned char** aCur, long aOffsetBytes, SeekDirType aSeekDir)
{
    if (aSeekDir == FromBeg)
    {
        *aCur = mBeg + aOffsetBytes;
    }
    else if (aSeekDir == FromCur)
    {
        *aCur = *aCur + aOffsetBytes;
    }
    else if (aSeekDir == FromDataEnd)
    {
        *aCur = mDatEnd - aOffsetBytes;
    }
    else
    {
        *aCur = mEnd - aOffsetBytes;
    }

    if (*aCur > mDatEnd)
    {
        *aCur = mDatEnd;
        return 0;
    }
    else if (*aCur < mBeg)
    {
        *aCur = mBeg;
        return 0;
    }
    else
    {
        return 1;
    }
}

int GenBuf::IncreaseSizeByAtLeast(unsigned long aSizeIncrease)
{
    if (mClassOwnsBuffer && mAllowGrowth)
    {
        unsigned long size = mEnd - mBeg;
        if (size > aSizeIncrease)
        {
            // Double buffer size rather than just adding a little
            aSizeIncrease = size;
        }

        if (mBeg && aSizeIncrease)
        {
            unsigned long oldSize = mDatEnd - mBeg;
            unsigned long size_temp = mEnd - mBeg + aSizeIncrease;
            unsigned long numDoubles = (size_temp + 7) / 8;
            unsigned char* begOld = mBeg;

            // Force mBeg to start on a double word boudary
            mBeg = reinterpret_cast<unsigned char*>(new double[numDoubles]);

            if (mBeg)
            {
                mCurPut = mBeg + static_cast<unsigned long>(mCurPut - begOld);
                mCurGet = mBeg + static_cast<unsigned long>(mCurGet - begOld);
                mDatEnd = mBeg + static_cast<unsigned long>(mDatEnd - begOld);
                mEnd = mBeg + numDoubles * 8;

                // Copy the data
                memcpy(mBeg, begOld, oldSize);

                // getValue rid of the old data
                delete[] begOld;

                return 1;
            }
            else
            {
                mBeg = begOld;
                return AllocationError;
            }
        }
        else if (aSizeIncrease)
        {
            unsigned long numDoubles = (aSizeIncrease + 7) / 8;

            // Force mBeg to start on a double word boudary
            mBeg = reinterpret_cast<unsigned char*>(new double[numDoubles]);

            if (mBeg)
            {
                mCurPut = mBeg;
                mCurGet = mBeg;
                mDatEnd = mBeg;
                mEnd = mBeg + numDoubles * 8;

                return 1;
            }
            else
            {
                return AllocationError;
            }
        }
        else
        {
            return UndefinedError;
        }
    }
    else
    {
        return TooMuchData;
    }
}
