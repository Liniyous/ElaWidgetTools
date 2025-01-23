#ifndef GEN_BUF_H
#define GEN_BUF_H

#include "ElaPacketIO_Export.h"

class ELA_PACKETIO_EXPORT GenBuf
{
public:
    enum SeekDirType
    {
        FromBeg = 0x01,
        FromCur = 0x02,
        FromDataEnd = 0x04,
        FromPhyEnd = 0x10
    };

    enum BufType
    {
        BigEndian = 0x01,
        LittleEndian = 0x02,
        LittleEndianVax = 0x03,
        Native = 0x04,
        Null = 0x05
    };

    enum IOStatus
    {
        NoError = 0,
        UndefinedError = -1,
        NotEnoughData = -2,   // Tried to get data from an empty buffer
        TooMuchData = -3,     // Tried to put data to a full buffer
        AllocationError = -4, // The "new" failed
        MismatchedSize = -5,  // Mismatched size between stated and actual data
        FixFailed = -100      // First attemp to fix failure did not work
    };

    virtual ~GenBuf();

    unsigned long GetTotalBytes() const;
    unsigned long GetNumValidBytes() const;
    unsigned long GetNumUnusedBytes() const;

    // Returns a positive number if successful else
    // returns a zero if there is an error
    int SetNumValidBytes(unsigned long aNumValidBytes);

    // putValue all pointers back to the beginning
    void Reset();

    int IncreaseSizeByAtLeast(unsigned long aSizeIncrease);

protected:
    // The default constructor used to be private but derived classes wouldn't
    // compile under Microsoft VC++ 7.1 even though they used one of the other
    // protected constructors (JAJ 29-Sep-2004)
    GenBuf();

    // Constructor will create its own buffer of size "aSize"
    GenBuf(unsigned long aSize, unsigned int aAllowGrowthOnPuts = 0);

    // Constructor will use the user supplied buffer "aUserBuffer"
    GenBuf(unsigned char* aUserBuffer, unsigned long aSizeInBytes, unsigned long aNumValidBytes = 0);

    // Change buffer currently being used, set current byte offset to zero,
    // and delete the buffer if it was initially created by this class
    void SetLocalBuffer(unsigned char* aUserBuffer, unsigned long aSizeInBytes, unsigned long aNumValidBytes = 0);

    unsigned char* GetLocalBuffer() const;

    int SetOffset(unsigned char** aCur, long aOffsetBytes, SeekDirType aSeekDir);
    int EnsureGetSpace(unsigned long aVariableSize) const;
    int EnsurePutSpace(unsigned long aVariableSize);
    void IncrementGetIndex(unsigned long aVariableSize);
    void IncrementPutIndex(unsigned long aVariableSize);
    unsigned char* GetGetIndex() const;
    unsigned char* GetPutIndex() const;

    unsigned char* mBeg;
    unsigned char* mCurGet;
    unsigned char* mCurPut;
    unsigned char* mDatEnd; // Really points to the first address
    // just past the end of the data
    unsigned char* mEnd; // Really points to the first address
                         // just outside of the buffer
private:
    // No copying of this class or its derived classes allowed
    GenBuf& operator=(const GenBuf& aRhs);
    GenBuf(const GenBuf& aSrc);

    unsigned int mClassOwnsBuffer;
    unsigned int mAllowGrowth;
};

inline unsigned char* GenBuf::GetLocalBuffer() const
{
    return mBeg;
}

inline unsigned long GenBuf::GetNumValidBytes() const
{
    return static_cast<unsigned long>(mDatEnd - mBeg);
}

inline unsigned long GenBuf::GetNumUnusedBytes() const
{
    return static_cast<unsigned long>(mEnd - mDatEnd);
}

inline unsigned long GenBuf::GetTotalBytes() const
{
    return static_cast<unsigned long>(mEnd - mBeg);
}

inline void GenBuf::Reset()
{
    mCurGet = mBeg;
    mCurPut = mBeg;
    mDatEnd = mBeg;
}

// Utility functions for Gets and Puts
inline int GenBuf::EnsureGetSpace(unsigned long aVariableSize) const
{
    return mCurGet + aVariableSize <= mDatEnd ? 1 : NotEnoughData;
}

inline int GenBuf::EnsurePutSpace(unsigned long aVariableSize)
{
    if (mCurPut + aVariableSize <= mEnd)
    {
        return 1;
    }
    else
    {
        // Let putValue buffer grow if we own the buffer
        return IncreaseSizeByAtLeast(aVariableSize);
    }
}

inline void GenBuf::IncrementGetIndex(unsigned long aVariableSize)
{
    mCurGet += aVariableSize;
}

inline void GenBuf::IncrementPutIndex(unsigned long aVariableSize)
{
    mCurPut += aVariableSize;

    if (mDatEnd < mCurPut)
    {
        mDatEnd = mCurPut;
    }
}

inline int GenBuf::SetNumValidBytes(unsigned long aNumValidBytes)
{
    mDatEnd = mBeg + aNumValidBytes;

    if (mDatEnd > mEnd)
    {
        mDatEnd = mEnd;
        return 0;
    }
    else
    {
        return 1;
    }
}

inline unsigned char* GenBuf::GetGetIndex() const
{
    return mCurGet;
}

inline unsigned char* GenBuf::GetPutIndex() const
{
    return mCurPut;
}

#endif
