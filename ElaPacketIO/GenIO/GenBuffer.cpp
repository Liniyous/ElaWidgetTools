#include "GenBuffer.h"

#include "GenSwapEndian.h"

//! Enables translation of calls to putValue() and getValue() to/from big endian
void GenBuffer::SetBigEndian()
{
    EnableByteSwap(GenSwapEndian::cLITTLE_ENDIAN);
}
//! Configures putValue() and getValue() to use native byte ordering
void GenBuffer::SetNativeByteOrder()
{
    EnableByteSwap(false);
}
//! Enlarges the buffer to a new size
void GenBuffer::GrowBy(size_t aBytes)
{
    size_t newSize = aBytes + mBytes + 1;
    // Always grow at minimum by 50%
    if ((aBytes + 1) < (mBytes / 2))
    {
        newSize = mBytes + mBytes / 2;
    }
    char* newData = new char[newSize];
    std::copy(mBuffer, mBuffer + mBytes, newData);
    delete[] mBuffer;
    mBuffer = newData;
    mBytes = newSize;
}

//! Move a set of bytes within the buffer
void GenBuffer::Move(size_t aBeginOffset, size_t aEndOffset, size_t aNewBegin)
{
    //! Same range - no copying required
    if (aNewBegin == aBeginOffset)
    {
        return;
    }
    else if (aBeginOffset > aNewBegin)
    {
        std::copy(mBuffer + aBeginOffset, mBuffer + aEndOffset, mBuffer + aNewBegin);
    }
    else
    {
        std::copy_backward(mBuffer + aBeginOffset, mBuffer + aEndOffset, mBuffer + aNewBegin + aEndOffset - aBeginOffset);
    }
}
