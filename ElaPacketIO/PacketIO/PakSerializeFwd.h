#ifndef PAKSERIALIZEFWD_H
#define PAKSERIALIZEFWD_H

#include <cstdint>
#include <string>

#include "PakI.h"
#include "PakIntTypes.h"
#include "PakO.h"
namespace PakSerialization
{
template <typename T>
class PakSerializePtr
{
public:
    PakSerializePtr(T*& aPtr)
        : mPtr(&aPtr)
    {
    }
    T** mPtr;
};

template <typename T>
class PakSerializePolyPtr
{
public:
    PakSerializePolyPtr(T*& aPtr)
        : mPtr(&aPtr)
    {
    }
    T** mPtr;
};

template <typename T>
class PakSerializeArray
{
public:
    PakSerializeArray(T* aArrayPtr, int aSize)
        : mArrayPtr(aArrayPtr), mSize(aSize)
    {
    }
    T* mArrayPtr;
    int mSize;
};

template <typename T>
class PakSerializeEnum
{
public:
    PakSerializeEnum(T& aEnum)
        : mEnumPtr(&aEnum)
    {
    }
    T* mEnumPtr;
};

class PakSerializeIgnoreData
{
public:
    PakSerializeIgnoreData(int aBytes)
        : mBytes(aBytes)
    {
    }
    int mBytes;
};

class PakSerializeRawData
{
public:
    PakSerializeRawData(void* aData, int aBytes)
        : mData(aData), mBytes(aBytes)
    {
    }
    void* mData;
    int mBytes;
};

class PakSerializeFixedString
{
public:
    PakSerializeFixedString(std::string& aString, int aMaxSize)
        : mStringPtr(&aString), mMaxSize(aMaxSize)
    {
    }
    std::string* mStringPtr;
    int mMaxSize;
};

template <typename T>
inline const PakSerializePolyPtr<T> Polymorphic(T*& aPtr)
{
    return PakSerializePolyPtr<T>(aPtr);
}

template <typename T>
inline const PakSerializeArray<T> Array(T* aPtr, int aSize)
{
    return PakSerializeArray<T>(aPtr, aSize);
}

template <typename T>
inline const PakSerializeEnum<T> Enum(T& aEnum)
{
    return PakSerializeEnum<T>(aEnum);
}

inline const PakSerializeIgnoreData IgnoreBytes(int aBytes)
{
    return PakSerializeIgnoreData(aBytes);
}

inline const PakSerializeRawData RawData(void* aData, int aBytes)
{
    return PakSerializeRawData(aData, aBytes);
}

inline const PakSerializeRawData RawData(const void* aData, int aBytes)
{
    return PakSerializeRawData(const_cast<void*>(aData), aBytes);
}

inline const PakSerializeFixedString FixedString(std::string& aString, int aMaxSize)
{
    return PakSerializeFixedString(aString, aMaxSize);
}

template <typename AR, typename T, bool IS_OUTPUT>
struct PakSplitSerialize {
    static void Go(AR& aAr, T& aValue) { aValue.Load(aAr); }
};

template <typename AR, typename T>
struct PakSplitSerialize<AR, T, true> {
    static void Go(AR& aAr, T& aValue) { aValue.Save(aAr); }
};
} // namespace PakSerialization
#endif
