#ifndef GENBUFFER_H
#define GENBUFFER_H

#include "ElaPacketIO_Export.h"

#include <algorithm>
#include <cstring>

class ELA_PACKETIO_EXPORT GenBuffer
{
public:
    //! Construct the buffer with a growable buffer of size aBytes
    GenBuffer(int aBytes)
        : mBuffer(nullptr), mCanGrow(false)
    {
        SetBigEndian();
        SetBuffer(new char[aBytes], aBytes, true);
    }

    //! Construct the buffer with a user buffer, see SetBuffer()
    GenBuffer(char* aData, int aBytes, bool aCanGrow = false)
        : mBuffer(nullptr), mCanGrow(false)
    {
        SetBigEndian();
        SetBuffer(aData, aBytes, aCanGrow);
    }

    static const int cDEFAULT_BUFFER_SIZE = 1472;

    //! Construct a copy of aSrc
    GenBuffer(const GenBuffer& aSrc)
        : mBuffer(new char[aSrc.mBytes]), mBytes(aSrc.mBytes), mGetPos(aSrc.mGetPos), mPutPos(0), mCanGrow(true), mSwapBytes(aSrc.mSwapBytes)
    {
        if (mPutPos > mGetPos)
        {
            PutRaw(aSrc.mBuffer, aSrc.mPutPos);
        }
    }

    //! Construct the buffer with a default size, and allow growth
    GenBuffer()
        : mBuffer(nullptr), mCanGrow(false)
    {
        SetBigEndian();
        SetBuffer(new char[cDEFAULT_BUFFER_SIZE], cDEFAULT_BUFFER_SIZE);
        mCanGrow = true;
    }

    ~GenBuffer() { Dispose(); }

    //! Disable or Enable byte swapping
    void EnableByteSwap(bool aSwapBytes) { mSwapBytes = aSwapBytes; }
    //! Enable byte swapping if the local machine is little-endian
    void SetBigEndian();
    //! Disable byte swapping
    void SetNativeByteOrder();

    void Dispose()
    {
        if (mCanGrow)
        {
            delete[] mBuffer;
        }
        mBuffer = nullptr;
    }

    //! Use a user-created buffer.
    //! @param aData Pointer to the buffer
    //! @param aBytes Length of the buffer in bytes
    //! @param aCanGrow If 'true', GenBuffer takes ownership of the buffer and allows resizing
    //!                 If 'false', GenBuffer will not delete the buffer or resize.
    void SetBuffer(char* aData, size_t aBytes, bool aCanGrow = false)
    {
        Dispose();
        mBuffer = aData;
        mBytes = aBytes;
        mPutPos = 0;
        mGetPos = 0;
        mCanGrow = aCanGrow;
    }
    //! Set the current offset used to put data
    void SetPutPos(size_t aPos) { mPutPos = aPos; }
    //! getValue the current offset used to put data
    size_t& GetPutPos() { return mPutPos; }
    //! Set the current offset used to put data
    size_t GetPutPos() const { return mPutPos; }
    //! Set the current offset used to get data
    void SetGetPos(size_t aPos) { mGetPos = aPos; }
    //! getValue the current offset used to get data
    size_t& GetGetPos() { return mGetPos; }
    //! getValue the current offset used to get data
    size_t GetGetPos() const { return mGetPos; }

    //! Returns a pointer to the internal buffer
    char* GetBuffer() { return mBuffer; }
    //! Returns a pointer to the internal buffer
    const char* GetBuffer() const { return mBuffer; }

    //! Returns the size of the internal buffer
    size_t GetBytes() const { return mBytes; }

    //! Writes a sequence of aBytes to the buffer.
    void PutRaw(const char* aBuffer, size_t aBytes)
    {
        CheckPutSpace(aBytes);
        memcpy(mBuffer + mPutPos, aBuffer, aBytes);
        mPutPos += aBytes;
    }
    //! Reads a sequence of aBytes to the buffer.
    void GetRaw(char* aBuffer, size_t aBytes)
    {
        memcpy(aBuffer, mBuffer + mGetPos, aBytes);
        mGetPos += aBytes;
    }

    //! Set the put/get positions to the start of the buffer
    void Reset() { mGetPos = mPutPos = 0; }

    void Move(size_t aBeginOffset, size_t aEndOffset, size_t aNewBegin);

    //! Grow the buffer so it is at least as large as aNewSize
    void Grow(size_t aNewSize)
    {
        if (aNewSize >= mBytes)
        {
            GrowBy(aNewSize - mBytes);
        }
    }
    //! Grow the buffer by an increment of at least aBytes
    void GrowBy(size_t aBytes);
    //! Return the difference between the write position and the read position
    size_t GetValidBytes() const { return mPutPos - mGetPos; }
    //! Swap the internal buffers between this and aRhs
    void SwapBuffer(GenBuffer& aRhs)
    {
        std::swap(mBuffer, aRhs.mBuffer);
        std::swap(mBytes, aRhs.mBytes);
        std::swap(mGetPos, aRhs.mGetPos);
        std::swap(mPutPos, aRhs.mPutPos);
        std::swap(mCanGrow, aRhs.mCanGrow);
    }

    void* ReleaseBuffer()
    {
        void* bufferPtr = mBuffer;
        mBuffer = nullptr;
        mBytes = 0;
        return bufferPtr;
    }

    //! putValue a basic type into the buffer.  This should only be used for basic types (char, int, float).
    //! This will perform a byte-swap if that option is enabled
    template <typename T>
    void putValue(const T& aVal)
    {
        PutP<sizeof(T)>(reinterpret_cast<const char*>(&aVal));
    }
    //! getValue a basic type from the buffer.  This should only be used for basic types (char, int, float)
    //! This will perform a byte-swap if that option is enabled
    template <typename T>
    void getValue(T& aVal)
    {
        GetP<sizeof(T)>(reinterpret_cast<char*>(&aVal));
    }

    void CheckPutSpace(size_t aPutSize)
    {
        if (mCanGrow && (aPutSize + mPutPos >= mBytes))
        {
            GrowBy(aPutSize);
        }
    }

protected:
    template <int BYTES>
    void PutP(const char* aBuffer)
    {
        CheckPutSpace(BYTES);
        if (mSwapBytes)
        {
            for (size_t i = 0; i < BYTES; ++i)
            {
                *(mBuffer + mPutPos + i) = aBuffer[BYTES - 1 - i];
            }
        }
        else
        {
            memcpy(mBuffer + mPutPos, aBuffer, BYTES);
        }
        mPutPos += BYTES;
    }

    template <int BYTES>
    void GetP(char* aBuffer)
    {
        if (mSwapBytes)
        {
            char* bufPtr = mBuffer + mGetPos;
            for (size_t i = 0; i < BYTES; ++i)
            {
                *(aBuffer + i) = bufPtr[BYTES - 1 - i];
            }
        }
        else
        {
            memcpy(aBuffer, mBuffer + mGetPos, BYTES);
        }
        mGetPos += BYTES;
    }

    char* mBuffer;
    size_t mBytes;
    size_t mGetPos;
    size_t mPutPos;
    bool mCanGrow;
    bool mSwapBytes;
};

#endif
