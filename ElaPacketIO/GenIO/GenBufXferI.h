// GenBufXferI - This class holds all the get logic for a GenIO Buffer.
//               Note the virtual inheritance for the multiple inheritance
//               in GenBuf.
//               This class is intended to be a base class for GenBufIUser,
//               GenBufIManaged, or GenBufIO

#ifndef GEN_BUF_XFER_I_H
#define GEN_BUF_XFER_I_H

#include "ElaPacketIO_Export.h"

#include <string>
#include <vector>

#include "GenBuf.h"
#include "GenIConvert.h"
#include "GenIODefs.h"

class ELA_PACKETIO_EXPORT GenBufXferI : virtual public GenBuf
{
public:
    ~GenBufXferI() override;

    // getValue offset from beginning or end of the data.  Offset from current
    // location is always zero
    long GetGetOffset(GenBuf::SeekDirType aSeekDir = GenBuf::FromBeg) const;

    // Returns a positive number if successful else
    // returns a zero if there is an error
    int SetGetOffset(long aOffsetBytes, GenBuf::SeekDirType aSeekDir);

    // Goofy microsoft compiler will not let function templates be defined
    // outside the class definition, so they are defined here.

    // Functions return number bytes removed from buffer if successful else
    // they return a "NotEnoughData"
    template <class T>
    int Get(T& aValue)
    {
        int status = EnsureGetSpace(sizeof(T));
        if (status > 0)
        {
            mGenIConvert->GetFromBuf(GetGetIndex(), aValue);
            IncrementGetIndex(sizeof(T));
            return sizeof(T);
        }
        else
        {
            return status;
        }
    }

    int Get(unsigned char& aValue);
    int Get(char& aValue);
    int Get(signed char& aValue);

    // Null terminated string function  CAUTION!  User is responsible for
    // providing enough space when calling this getValue method.
    // Returned count value includes terminating null zero
    int Get(char* aValue, unsigned long& aBytesSeen);

    template <class T, class A>
    int GetVector(std::vector<T, A>& aVec)
    {
        unsigned int size = static_cast<unsigned int>(aVec.size());
        unsigned int numBytes = size * sizeof(T);
        int status = EnsureGetSpace(numBytes);
        if (status > 0)
        {
            for (unsigned int j = 0; j < size; ++j)
            {
                mGenIConvert->GetFromBuf(GetGetIndex(), aVec[j]);
                IncrementGetIndex(sizeof(T));
            }
            return static_cast<int>(numBytes);
        }
        else
        {
            return status;
        }
    }

#if defined(_WIN32) && _MSC_VER < 1300
    int GetVector(std::vector<unsigned char>& aVec) { return GetCharVector(aVec); }

    int GetVector(std::vector<char>& aVec) { return GetCharVector(aVec); }

    int GetVector(std::vector<signed char>& aVec) { return GetCharVector(aVec); }
#else
    template <class A>
    int GetVector(std::vector<unsigned char, A>& aVec);
    template <class A>
    int GetVector(std::vector<char, A>& aVec);
    template <class A>
    int GetVector(std::vector<signed char, A>& aVec);
#endif

    int GetString(std::string& aString, int aSize);

    template <class T>
    int GetArray(T aValue[], unsigned int aArrayLen)
    {
        unsigned int numBytes = sizeof(T) * aArrayLen;
        int status = EnsureGetSpace(numBytes);
        if (status > 0)
        {
            for (unsigned int j = 0; j < aArrayLen; ++j)
            {
                mGenIConvert->GetFromBuf(GetGetIndex(), aValue[j]);
                IncrementGetIndex(sizeof(T));
            }
            return numBytes;
        }
        else
        {
            return status;
        }
    }

    int GetArray(unsigned char aArray[], unsigned int aArrayLen);
    int GetArray(char aArray[], unsigned int aArrayLen);
    int GetArray(signed char aArray[], unsigned int aArrayLen);

    // Function to getValue raw data from buffer. No byte swapping is performed.
    // It returns number bytes gotten from the buffer if successful else
    // an error flag is returned.
    int GetRaw(void* aDataPtr, unsigned long aDataSizeBytes);

    // Function to getValue raw data from buffer. No byte swapping is performed.
    // No data is moved, but the "get" pointers are moved.
    // It returns number bytes to be gotten form the buffer if successful else
    // an error flag is returned.
    int GetBufferIfValid(const void*& aBufferPtr, unsigned long aDataSizeBytes);

protected:
    // Constructor will create a GenIConvert object
    explicit GenBufXferI(BufType aBufType, bool aAllowGrowth = false);

private:
    // This function is needed for the GetVectors that are characters
    template <class T, class A>
    int GetCharVector(std::vector<T, A>& aVec);

    // Use protected constructor
    GenBufXferI();

    int GetCharArray(void* aArray, unsigned int aArrayLen);
    void BuildGenIConvert(BufType aBufType);

    GenIConvert* mGenIConvert;
};

inline long GenBufXferI::GetGetOffset(GenBuf::SeekDirType aSeekDir) const
{
    if (aSeekDir == FromDataEnd)
    {
        return static_cast<long>(mDatEnd - mCurGet);
    }
    else if (aSeekDir == FromBeg)
    {
        return static_cast<long>(mCurGet - mBeg);
    }
    else if (aSeekDir == FromPhyEnd)
    {
        return static_cast<long>(mEnd - mCurGet);
    }
    else
    {
        // From current is always zero
        return 0;
    }
}

inline int GenBufXferI::SetGetOffset(long aOffsetBytes, SeekDirType aSeekDir)
{
    return SetOffset(&mCurGet, aOffsetBytes, aSeekDir);
}

//
// Gets
inline int GenBufXferI::GetString(std::string& aString, int aSize)
{
    int status = EnsureGetSpace(aSize);
    if (status > 0)
    {
        aString.assign(reinterpret_cast<char*>(mCurGet), static_cast<std::string::size_type>(aSize));
        IncrementGetIndex(aSize);
        return static_cast<int>(aSize);
    }
    else
    {
        return status;
    }
}

inline int GenBufXferI::Get(unsigned char& aValue)
{
    int status = EnsureGetSpace(1);
    if (status > 0)
    {
        aValue = *reinterpret_cast<unsigned char*>(mCurGet);
        IncrementGetIndex(1);
        return 1;
    }
    else
    {
        return status;
    }
}

inline int GenBufXferI::Get(char& aValue)
{
    int status = EnsureGetSpace(1);
    if (status > 0)
    {
        aValue = *reinterpret_cast<char*>(mCurGet);
        IncrementGetIndex(1);
        return 1;
    }
    else
    {
        return status;
    }
}

inline int GenBufXferI::Get(signed char& aValue)
{
    int status = EnsureGetSpace(1);
    if (status > 0)
    {
        aValue = *reinterpret_cast<signed char*>(mCurGet);
        IncrementGetIndex(1);
        return 1;
    }
    else
    {
        return status;
    }
}

// Need to define this function here so it can be used below
template <class T, class A>
inline int GenBufXferI::GetCharVector(std::vector<T, A>& aVec)
{
    unsigned int size = static_cast<unsigned int>(aVec.size());
    int status = EnsureGetSpace(size);
    if (status > 0)
    {
        memcpy(&aVec[0], mCurGet, size);
        IncrementGetIndex(size);
        return static_cast<int>(size);
    }
    else
    {
        return status;
    }
}

#if !defined(_WIN32) || (defined(_WIN32) && _MSC_VER >= 1300)
template <class A>
inline int GenBufXferI::GetVector(std::vector<unsigned char, A>& aVec)
{
    return GetCharVector(aVec);
}

template <class A>
inline int GenBufXferI::GetVector(std::vector<char, A>& aVec)
{
    return GetCharVector(aVec);
}

template <class A>
inline int GenBufXferI::GetVector(std::vector<signed char, A>& aVec)
{
    return GetCharVector(aVec);
}
#endif

inline int GenBufXferI::GetArray(unsigned char aArray[], unsigned int aArrayLen)
{
    return GetCharArray(aArray, aArrayLen);
}

inline int GenBufXferI::GetArray(char aArray[], unsigned int aArrayLen)
{
    return GetCharArray(aArray, aArrayLen);
}

inline int GenBufXferI::GetArray(signed char aArray[], unsigned int aArrayLen)
{
    return GetCharArray(aArray, aArrayLen);
}

#endif
