// GenOBufXfer - This class holds all the put logic for a GenIO Buffer
//               Note the virtual inheritance for the multiple inheritance
//               in GenIOBuf.
//               This class is intended to be a base class for GenBufOUser,
//               GenBufOManaged, or GenBufIO

#ifndef GEN_BUF_XFER_O_H
#define GEN_BUF_XFER_O_H

#include "ElaPacketIO_Export.h"

#include <cstring>
#include <string>
#include <vector>

#include "GenBuf.h"
#include "GenIODefs.h"
#include "GenOConvert.h"

class ELA_PACKETIO_EXPORT GenBufXferO : virtual public GenBuf
{
public:
    ~GenBufXferO() override;

    // getValue offset from beginning or end of the data.  Offset from current
    // location is always zero
    long GetPutOffset(GenBuf::SeekDirType aSeekDir = GenBuf::FromBeg) const;

    // Returns a positive number if successful else
    // returns a zero if there is an error
    int SetPutOffset(long aOffsetBytes, GenBuf::SeekDirType aSeekDir);

    // The putValue's
    // Goofy microsoft compiler will not let function templates be defined
    // outside the class definition, so they are defined here.

    // Functions return number bytes added to the buffer if successful else
    // they return an error flag
    template <class T>
    int Put(T aValue)
    {
        int status = EnsurePutSpace(sizeof(T));
        if (status > 0)
        {
            mGenOConvert->PutToBuf(GetPutIndex(), aValue);
            IncrementPutIndex(sizeof(T));
            return sizeof(T);
        }
        else
        {
            return status;
        }
    }

    int Put(unsigned char aValue);
    int Put(char aValue);
    int Put(signed char aValue);

    // Null terminated string function, will return number bytes added to
    // the buffer including null zero if successful else it will return
    // a error flag
    int Put(const char* aValue, unsigned long& aLength);

    template <class T, class A>
    int PutVector(const std::vector<T, A>& aVec)
    {
        unsigned int size = static_cast<unsigned int>(aVec.size());
        unsigned int numBytes = size * sizeof(T);
        int status = EnsurePutSpace(numBytes);
        if (status > 0)
        {
            for (unsigned int j = 0; j < size; ++j)
            {
                mGenOConvert->PutToBuf(GetPutIndex(), aVec[j]);
                IncrementPutIndex(sizeof(T));
            }
            return static_cast<int>(numBytes);
        }
        else
        {
            return status;
        }
    }

#if defined(_WIN32) && _MSC_VER < 1300
    int PutVector(const std::vector<unsigned char>& aVec) { return PutCharVector(aVec); }

    int PutVector(const std::vector<char>& aVec) { return PutCharVector(aVec); }

    int PutVector(const std::vector<signed char>& aVec) { return PutCharVector(aVec); }
#else
    template <class A>
    int PutVector(const std::vector<unsigned char, A>& aVec);
    template <class A>
    int PutVector(const std::vector<char, A>& aVec);
    template <class A>
    int PutVector(const std::vector<signed char, A>& aVec);
#endif
    int PutString(const std::string& aString);

    template <class T>
    int PutArray(const T aValue[], unsigned int aArrayLen)
    {
        unsigned int numBytes = aArrayLen * sizeof(T);
        int status = EnsurePutSpace(numBytes);
        if (status > 0)
        {
            for (unsigned int j = 0; j < aArrayLen; ++j)
            {
                mGenOConvert->PutToBuf(GetPutIndex(), aValue[j]);
                IncrementPutIndex(sizeof(T));
            }
            return static_cast<int>(numBytes);
        }
        else
        {
            return status;
        }
    }

    int PutArray(const unsigned char aArray[], unsigned int aArrayLen);
    int PutArray(const char aArray[], unsigned int aArrayLen);
    int PutArray(const signed char aArray[], unsigned int aArrayLen);

    // Function to put raw data to buffer. No byte swapping is performed.
    // It returns number bytes added to the buffer if successful else
    // an error flag is returned.
    int PutRaw(const void* aDataPtr, unsigned long aDataSizeBytes);

    // Function to put raw data to buffer. No byte swapping is performed.
    // No data is moved, but the "put" pointers are moved.
    // It returns number bytes to be added to the buffer if successful else
    // an error flag is returned.
    int GetBufferIfValid(void*& aBufferPtr, unsigned long aDataSizeBytes);

protected:
    // Constructor will create a GenOConvert object
    explicit GenBufXferO(BufType aBufType);

private:
    // This function is needed for the PutVectors that are characters
    template <class T, class A>
    int PutCharVector(const std::vector<T, A>& aVec);

    // Use protected constructor
    GenBufXferO();

    int PutCharArray(const void* aArray, unsigned int aArrayLen);
    void BuildGenOConvert(BufType aBufType);

    GenOConvert* mGenOConvert;
};

inline long GenBufXferO::GetPutOffset(GenBuf::SeekDirType aSeekDir) const
{
    if (aSeekDir == FromBeg)
    {
        return static_cast<long>(mCurPut - mBeg);
    }
    else if (aSeekDir == FromPhyEnd)
    {
        return static_cast<long>(mEnd - mCurPut);
    }
    else if (aSeekDir == FromDataEnd)
    {
        return static_cast<long>(mDatEnd - mCurPut);
    }
    else
    {
        // From current is always zero
        return 0;
    }
}

inline int GenBufXferO::SetPutOffset(long aOffsetBytes, SeekDirType aSeekDir)
{
    return SetOffset(&mCurPut, aOffsetBytes, aSeekDir);
}

//
// Puts
//

inline int GenBufXferO::PutString(const std::string& aString)
{
    unsigned int size = static_cast<unsigned int>(aString.size());
    int status = EnsurePutSpace(size);
    if (status > 0)
    {
        memcpy(mCurPut, aString.c_str(), size);
        IncrementPutIndex(size);
        return static_cast<int>(size);
    }
    else
    {
        return status;
    }
}

inline int GenBufXferO::Put(unsigned char aValue)
{
    int status = EnsurePutSpace(1);
    if (status > 0)
    {
        *reinterpret_cast<unsigned char*>(mCurPut) = aValue;
        IncrementPutIndex(1);
        return 1;
    }
    else
    {
        return status;
    }
}

inline int GenBufXferO::Put(char aValue)
{
    int status = EnsurePutSpace(1);
    if (status > 0)
    {
        *reinterpret_cast<char*>(mCurPut) = aValue;
        IncrementPutIndex(1);
        return 1;
    }
    else
    {
        return status;
    }
}

inline int GenBufXferO::Put(signed char aValue)
{
    int status = EnsurePutSpace(1);
    if (status > 0)
    {
        *reinterpret_cast<signed char*>(mCurPut) = aValue;
        IncrementPutIndex(1);
        return 1;
    }
    else
    {
        return status;
    }
}

template <class T, class A>
inline int GenBufXferO::PutCharVector(const std::vector<T, A>& aVec)
{
    unsigned int size = static_cast<unsigned int>(aVec.size());
    int status = EnsurePutSpace(size);
    if (status > 0)
    {
        memcpy(mCurPut, &aVec[0], size);
        IncrementPutIndex(size);
        return static_cast<int>(size);
    }
    else
    {
        return status;
    }
}

#if !defined(_WIN32) || (defined(_WIN32) && _MSC_VER >= 1300)
template <class A>
inline int GenBufXferO::PutVector(const std::vector<unsigned char, A>& aVec)
{
    return PutCharVector(aVec);
}

template <class A>
inline int GenBufXferO::PutVector(const std::vector<char, A>& aVec)
{
    return PutCharVector(aVec);
}

template <class A>
inline int GenBufXferO::PutVector(const std::vector<signed char, A>& aVec)
{
    return PutCharVector(aVec);
}
#endif

inline int GenBufXferO::PutArray(const unsigned char aArray[], unsigned int aArrayLen)
{
    return PutCharArray(aArray, aArrayLen);
}

inline int GenBufXferO::PutArray(const char aArray[], unsigned int aArrayLen)
{
    return PutCharArray(aArray, aArrayLen);
}

inline int GenBufXferO::PutArray(const signed char aArray[], unsigned int aArrayLen)
{
    return PutCharArray(aArray, aArrayLen);
}

#endif
