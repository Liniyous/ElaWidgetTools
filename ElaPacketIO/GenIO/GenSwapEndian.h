#ifndef GENSWAPENDIAN_H
#define GENSWAPENDIAN_H

#include "ElaPacketIO_Export.h"

#include <algorithm>

#include "GenIODefs.h"

namespace GenSwapEndian_Detail
{
// Can only swap 1,2,4,8 byte values
template <size_t BITS>
struct Swapper {
};
template <>
struct Swapper<1> {
    void Swap(char*) {}
};
template <>
struct Swapper<2> {
    static void Swap(char* aBytes) { std::swap(aBytes[0], aBytes[1]); }
};
template <>
struct Swapper<4> {
    static void Swap(char* aBytes)
    {
        std::swap(aBytes[0], aBytes[3]);
        std::swap(aBytes[1], aBytes[2]);
    }
};
template <>
struct Swapper<8> {
    static void Swap(char* aBytes)
    {
        std::swap(aBytes[0], aBytes[7]);
        std::swap(aBytes[1], aBytes[6]);
        std::swap(aBytes[2], aBytes[5]);
        std::swap(aBytes[3], aBytes[4]);
    }
};
} // namespace GenSwapEndian_Detail

//! Provides swapping big->little / little->big endian byte-swapping in
//! a convenient, machine independent manner
//! Usage:  GenSwapEndian::SwapBigNative() & var1 & var2 & var3;
class ELA_PACKETIO_EXPORT GenSwapEndian
{
public:
    class GenEndianSwapper
    {
    public:
        template <typename T>
        GenEndianSwapper& operator&(T& aVal)
        {
            GenSwapEndian_Detail::Swapper<sizeof(T)>::Swap((char*)&aVal);
            return *this;
        }
        template <typename T>
        GenEndianSwapper& Array(T* aVal, size_t aSize)
        {
            for (size_t i = 0; i < aSize; ++i)
            {
                (*this) & aVal[i];
            }
            return *this;
        }
    };
    class GenEndianSwapper_NoChange
    {
    public:
        template <typename T>
        GenEndianSwapper_NoChange& operator&(T& /*aVal*/)
        {
            return *this;
        }
        template <typename T>
        GenEndianSwapper_NoChange& Array(T* /*aVal*/, int /*aSize*/)
        {
            return *this;
        }
    };
#ifdef GENIO_LIT_ENDIAN
    typedef GenEndianSwapper GenBigEndianSwapper;
    typedef GenEndianSwapper_NoChange GenLittleEndianSwapper;
    static const bool cLITTLE_ENDIAN = true;
    static const bool cBIG_ENDIAN = false;
#endif
#ifdef GENIO_BIG_ENDIAN
    typedef GenEndianSwapper_NoChange GenBigEndianSwapper;
    typedef GenEndianSwapper GenLittleEndianSwapper;
    static const bool cLITTLE_ENDIAN = false;
    static const bool cBIG_ENDIAN = true;
#endif
    //! Return an object that swaps big-endian to Native byte order using the '&' operator
    //! (this is a no-op if big endian is the native byte order)
    static GenBigEndianSwapper SwapBigNative() { return GenBigEndianSwapper(); }
    //! Return an object that swaps little-endian to Native byte order using the '&' operator
    //! (this is a no-op if little endian is the native byte order)
    static GenLittleEndianSwapper SwapLittleNative() { return GenLittleEndianSwapper(); }
};

#endif
