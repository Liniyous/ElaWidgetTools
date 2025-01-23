#include "PakSerializeFwd.h"
#include "PakSerializeTraits.h"
#ifndef PAKSERIALIZE_H
#define PAKSERIALIZE_H
#include "ElaPacketIO_Export.h"

namespace PakSerialization
{
template <typename AR, typename T>
inline void Serialize(AR& aAr, T& aValue);
template <class AR, typename T>
inline void Serialize(AR& aAr, T*& aPtr);

template <typename T>
inline void Load(PakI& aAr, T& aValue);
template <typename T>
inline void Save(PakO& aAr, T& aValue);

//! This may be overridden for types that do not have default constructors
template <typename T>
inline T* CreateType()
{
    return new T;
}

inline ELA_PACKETIO_EXPORT void Serialize(PakI& aAr, PakSerializeRawData& aData)
{
    aAr.GetBuffer()->GetRaw((char*)aData.mData, aData.mBytes);
}
inline ELA_PACKETIO_EXPORT void Serialize(PakO& aAr, PakSerializeRawData& aData)
{
    aAr.GetBuffer()->PutRaw((char*)aData.mData, aData.mBytes);
}
inline ELA_PACKETIO_EXPORT void Serialize(PakI& aAr, PakSerializeFixedString& aData)
{
    aAr.SerializeString(*aData.mStringPtr, aData.mMaxSize);
}
inline ELA_PACKETIO_EXPORT void Serialize(PakO& aAr, PakSerializeFixedString& aData)
{
    aAr.SerializeString(*aData.mStringPtr, aData.mMaxSize);
}
} // End Namespace PakSerialization

template <typename T>
inline PakI& operator&(PakI& aAr, T& aValue);
template <typename T>
inline PakI& operator>>(PakI& aAr, T& aValue);

template <typename T>
inline PakO& operator&(PakO& aAr, T& aValue);
template <typename T>
inline PakO& operator&(PakO& aAr, const T& aValue);
template <typename T>
inline PakO& operator<<(PakO& aAr, T& aValue);
template <typename T>
inline PakO& operator<<(PakO& aAr, const T& aValue);

// Define serialization for some basic types and containers

namespace PakSerialization
{
// Provides a non-const typedef of any type
template <typename T>
struct Non_Const {
    typedef T Type;
};
template <typename T>
struct Non_Const<const T> {
    typedef T Type;
};

// Serialize a pointer
template <typename T>
inline void Load(PakI& aAr, PakSerialization::PakSerializePtr<T>& aPtr)
{
    bool isValid;
    aAr & isValid;
    if (isValid)
    {
        typedef typename Non_Const<T>::Type NC_Type;
        NC_Type* typePtr = CreateType<NC_Type>();
        aAr&* typePtr;
        *aPtr.mPtr = typePtr;
    }
    else
    {
        *aPtr.mPtr = nullptr;
    }
}

template <typename T>
inline void Save(PakO& aAr, PakSerialization::PakSerializePtr<T>& aPtr)
{
    bool isValid = (*aPtr.mPtr != nullptr);
    aAr & isValid;
    if (isValid)
    {
        aAr&(typename Non_Const<T>::Type&)**aPtr.mPtr;
    }
}
} // End Namespace PakSerialization

#include "PakSerializeTypes.h"

namespace PakSerialization
{
template <class AR, typename T>
void Serialize(AR& aAr, PakSerializeArray<T>& aArray)
{
    for (int i = 0; i < aArray.mSize; ++i)
    {
        aAr & aArray.mArrayPtr[i];
    }
}

template <typename T>
inline void Load(PakI& aAr, PakSerialization::PakSerializeEnum<T>& aEnum)
{
    int32_t value(0);
    aAr & value;
    *aEnum.mEnumPtr = static_cast<T>(value);
}

template <typename T>
inline void Save(PakO& aAr, PakSerialization::PakSerializeEnum<T>& aEnum)
{
    int32_t value = static_cast<int32_t>(*aEnum.mEnumPtr);
    aAr & value;
}
template <class AR>
inline void Serialize(AR& aAr, PakSerializeIgnoreData& aIgnore)
{
    aAr.IgnoreBytes(aIgnore.mBytes);
}
} // namespace PakSerialization

#endif
