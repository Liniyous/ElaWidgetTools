#ifndef PAKSERIALIZETYPES_H
#define PAKSERIALIZETYPES_H

#include "ElaPacketIO_Export.h"

#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "GenBuffer.h"
#include "PakI.h"
#include "PakO.h"
#include "PakSerialize.h"
#include "UtImmutableList.h"

// Defines serialization methods for some standard types
// std::(string, vector, set, map, pair, list), UtSharedPtr, GenMemIO
namespace PakSerialization
{
template <typename T>
inline void Load(PakI& aAr, T&);
template <typename T>
inline void Save(PakO& aAr, T&);

// Boolean values are sent as a char
template <>
inline void ELA_PACKETIO_EXPORT Load(PakI& aAr, bool& aValue)
{
    char val(0);
    aAr.Serialize(val);
    aValue = (val != 0);
}
template <>
inline void ELA_PACKETIO_EXPORT Save(PakO& aAr, bool& aValue)
{
    char val = aValue ? 1 : 0;
    aAr.Serialize(val);
}

// std::pair
template <typename AR, typename S, typename T>
inline void Serialize(AR& aAr, std::pair<S, T>& aValue)
{
    aAr & aValue.first & aValue.second;
}

// Helper function for serializing container values
template <typename AR, typename T>
inline void SaveContainerValues(AR& aAr, const T& aContainer)
{
    typename T::const_iterator iter = aContainer.begin();
    for (; iter != aContainer.end(); ++iter)
    {
        aAr& const_cast<typename T::reference>(*iter);
    }
}

// std::set
template <typename T>
inline void Load(PakI& aAr, std::set<T>& aValue)
{
    uint32_t size;
    aValue.clear();
    aAr & size;
    T value;
    for (int i = 0; i < size; ++i)
    {
        aAr & value;
        aValue.insert(value);
    }
}
template <typename T>
inline void Save(PakO& aAr, std::set<T>& aValue)
{
    uint32_t size = aValue.size();
    aAr & size;
    SaveContainerValues(aAr, aValue);
}

// std::string
void ELA_PACKETIO_EXPORT Save(PakO& aAr, std::string& aValue);
void ELA_PACKETIO_EXPORT Load(PakI& aAr, std::string& aValue);

// std::list
template <typename T, typename ALLOC>
inline void Save(PakO& aAr, std::list<T, ALLOC>& aValue)
{
    uint32_t size = (uint32_t)aValue.size();
    aAr & size;
    SaveContainerValues(aAr, aValue);
}

template <typename T, typename ALLOC>
inline void Load(PakI& aAr, std::list<T, ALLOC>& aValue)
{
    uint32_t size;
    aValue.clear();
    aAr & size;
    T value;
    for (unsigned i = 0; i < size; ++i)
    {
        aAr & value;
        aValue.push_back(value);
    }
}

// std::vector
template <typename T, typename ALLOC>
inline void Save(PakO& aAr, std::vector<T, ALLOC>& aValue)
{
    uint32_t size = (uint32_t)aValue.size();
    aAr & size;
    SaveContainerValues(aAr, aValue);
}

template <typename T, typename ALLOC>
inline void Load(PakI& aAr, std::vector<T, ALLOC>& aValue)
{
    uint32_t size;
    aAr & size;
    aValue.resize(size);
    for (unsigned i = 0; i < size; ++i)
    {
        aAr& aValue[i];
    }
}

// std::map
template <typename S, typename T, typename CMP, typename ALLOC>
inline void Load(PakI& aAr, std::map<S, T, CMP, ALLOC>& aValue)
{
    uint32_t size;
    aValue.clear();
    aAr & size;
    S key;
    T value;
    for (unsigned i = 0; i < size; ++i)
    {
        aAr & key & value;
        aValue.insert(std::make_pair(key, value));
    }
}

template <typename S, typename T, typename CMP, typename ALLOC>
inline void Save(PakO& aAr, std::map<S, T, CMP, ALLOC>& aValue)
{
    uint32_t size = (uint32_t)aValue.size();
    aAr & size;
    SaveContainerValues(aAr, aValue);
}

// std::shared_ptr<>
template <typename T>
inline void Load(PakI& aAr, std::shared_ptr<T>& aSharedPtr)
{
    T* ptr(nullptr);
    aAr & ptr;
    if (ptr != nullptr)
    {
        aSharedPtr = std::shared_ptr<T>(ptr);
    }
}

template <typename T>
inline void Save(PakO& aAr, std::shared_ptr<T>& aSharedPtr)
{
    T* ptr(nullptr);
    if (aSharedPtr)
    {
        ptr = aSharedPtr.get();
    }
    aAr & ptr;
}

// std::unique_ptr<>
template <typename T>
inline void Load(PakI& aAr, std::unique_ptr<T>& aUniquePtr)
{
    T* ptr(nullptr);
    aAr & ptr;
    aUniquePtr.reset(ptr);
}

template <typename T>
inline void Save(PakO& aAr, std::unique_ptr<T>& aUniquePtr)
{
    aAr & aUniquePtr.get();
}

// UtImmutableList
template <typename T>
inline void Save(PakO& aAr, UtImmutableList<T>& aValue)
{
    std::vector<T> vec = aValue.toVector();
    aAr & vec;
}

template <typename T>
inline void Load(PakI& aAr, UtImmutableList<T>& aValue)
{
    std::vector<T> values;
    aAr & values;
    aValue = UtImmutableList<T>::fromVector(values);
}

} // End namespace PakSerialization

#endif
