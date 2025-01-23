#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#include "GenOConvertBigEndian.h"

#include <cstddef> // for size_t

#ifdef _WIN32
#pragma warning(disable : 4311)
#endif

//  This Class inherits from GenOConvert

// This class is used to put data into a big endian buffer.
// The documentation below was stolen and from convproc.c and
// slightly modified.

//   *******************  Type Conversion Details  ********************
//
//        *************** Integers **************
//   Integers are converted between Big and Little Endian format simply
//   by performing a complete swap of the bytes in the integer.  For example,
//   a 32-bit integer with bytes ABCD would be swapped to DCBA.
//
//        ********* Big Endian <-> Little Endian IEEE floating point *****
//   The formats of these two are the same, only the bytes are swapped.
//   Conversion is the same as Big/Little Endian integers:
//   Swap 4 bytes for a Single Precision, 8 bytes for a double precision.
//
//        ********* IEEE to VAX floating point ***********
//
//   VAX F float is excess 128 with an implied 0.1 fraction bit, and IEEE
//   single precision is excess 127 with an implied 1.0 fraction bit.
//   Therefore, the conversion from IEEE to VAX F involves adding two
//   to the exponent and rearranging the bytes.  Since the lowest bit of
//   exponent spills into the high fraction byte, adding 2 to the entire
//   exponent is more easily accomplished by adding 1 to the upper 7 bits
//   of exponent which conveniently all appear in one byte.  We don't have
//   to worry about carry to other bytes since all 7 bits are in one byte.
//
//   In the following diagrams: S is the sign bit, E is the exponent field,
//   a is the most significant part of the fraction, b - g are the remaining
//   fractional parts in decreasing significance. A is the address.
//
//   The IEEE Single Precision Big Endian (SPBE) number is:
//      A+0      A+1      A+2      A+3
//   +--------+--------+--------+--------+
//   |SEEEEEEE|Eaaaaaaa|bbbbbbbb|cccccccc|
//   +--------+--------+--------+--------+
//
//   The IEEE Single Precsion Little Endian (SPLE) number is:
//      A+3      A+2      A+1      A+0
//   +--------+--------+--------+--------+
//   |SEEEEEEE|Eaaaaaaa|bbbbbbbb|cccccccc|
//   +--------+--------+--------+--------+
//
//   The VAX F_float looks like this:
//      A+3      A+2      A+1      A+0
//   +--------+--------+--------+--------+
//   |bbbbbbbb|cccccccc|SEEEEE+1|Eaaaaaaa|
//   +--------+--------+--------+--------+
//
//   VAX D IS NOT SUPPORTED
//   VAX D floating point format is identical to F floating except for an
//   additional 32 lower significance fractional bits. IEEE double precision
//   format is drastically different than single precision so the conversion
//   is not so straightforward.
//   IEEE Double Precision:
//     11 bit exponent (excess 1023), 52 fractional bits with implied 1.xxx
//   VAX D_floating:
//     8 bit exponent (excess 128), 55 fractional bits with implied 0.1xxx
//
//   VAX G_float is excess 1024 with an implied 0.1 fraction bit, and IEEE
//   double precision is excess 1023 with an implied 1.0 fraction bit.
//   Therefore, the conversion from IEEE to VAX G involves adding two to
//   the exponent and rearranging the bytes.
//
//   The IEEE Double Precision Big Endian (DPBE) is:
//      A+0      A+1      A+2      A+3      A+4      A+5      A+6      A+7
//   +--------+--------+--------+--------+--------+--------+--------+--------+
//   |SEEEEEEE|EEEEaaaa|bbbbbbbb|cccccccc|dddddddd|eeeeeeee|ffffffff|gggggggg|
//   +--------+--------+--------+--------+--------+--------+--------+--------+
//
//   The IEEE Double Precision Little Endian (DPLE) is:
//      A+7      A+6      A+5      A+4      A+3      A+2      A+1      A+0
//   +--------+--------+--------+--------+--------+--------+--------+--------+
//   |SEEEEEEE|EEEEaaaa|bbbbbbbb|cccccccc|dddddddd|eeeeeeee|ffffffff|gggggggg|
//   +--------+--------+--------+--------+--------+--------+--------+--------+
//
//   The VAX G_float looks like this:
//      A+7      A+6      A+5      A+4      A+3      A+2      A+1      A+0
//   +--------+--------+--------+--------+--------+--------+--------+--------+
//   |ffffffff|gggggggg|dddddddd|eeeeeeee|bbbbbbbb|cccccccc|SEEEEEEE|EE+2aaaa|
//   +--------+--------+--------+--------+--------+--------+--------+--------+
//
//   Denormalized IEEE numbers will not be converted accurately by these
//   routines.  But, they will convert to some other very small number
//   which is close enough for our practical purposes.  Very large IEEE
//   numbers close to the upper range limit could overflow the range of
//   VAX numbers.  We don't use numbers that large in our applications
//   so we don't worry about it here.
//
//        **************//VAX to IEEE floating point **************
//
//   Conversion from VAX to IEEE type is just the opposite of the
//   IEEE to VAX algorithms described above.
//
//   VAX to IEEE can underflow, can't overflow
//
// MODULE ERROR RANGE: 980 - 984

GenOConvertBigEndian::GenOConvertBigEndian() {}

GenOConvertBigEndian::~GenOConvertBigEndian() {}

// *****************************
//
// The putValue's
//
// *****************************

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, unsigned short int aVal) const
{
    aCurPut[0] = static_cast<unsigned char>(aVal >> 8);
    aCurPut[1] = static_cast<unsigned char>(aVal & 0xFF);
}

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, short int aVal) const
{
    aCurPut[0] = static_cast<unsigned char>(*reinterpret_cast<unsigned short*>(&aVal) >> 8);
    aCurPut[1] = static_cast<unsigned char>(*reinterpret_cast<unsigned short*>(&aVal) & 0xFF);
}

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, unsigned int aVal) const
{
    // Transfer data in one assignment if on correct boundary, else do it byte by byte
    if (!(reinterpret_cast<size_t>(aCurPut) & 0x3))
    {
#ifdef GENIO_BIG_ENDIAN
        *reinterpret_cast<unsigned int*>(aCurPut) = aVal;
#else
        *reinterpret_cast<unsigned int*>(aCurPut) =
            (aVal >> 24) | (aVal >> 8 & 0x0000FF00) | (aVal << 8 & 0x00FF0000) | (aVal << 24);
#endif
    }
    else
    {
        aCurPut[0] = static_cast<unsigned char>(aVal >> 24);
        aCurPut[1] = static_cast<unsigned char>(aVal >> 16 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(aVal >> 8 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(aVal & 0xFF);
    }
}

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, int aVal) const
{
    // Transfer data in one assignment if on correct boundary, else do it byte by byte
    if (!(reinterpret_cast<size_t>(aCurPut) & 0x3))
    {
#ifdef GENIO_BIG_ENDIAN
        *reinterpret_cast<int*>(aCurPut) = aVal;
#else
        *reinterpret_cast<unsigned int*>(aCurPut) =
            (*reinterpret_cast<unsigned int*>(&aVal) >> 24) | (*reinterpret_cast<unsigned int*>(&aVal) >> 8 & 0x0000FF00) | (*reinterpret_cast<unsigned int*>(&aVal) << 8 & 0x00FF0000) | (*reinterpret_cast<unsigned int*>(&aVal) << 24);
#endif
    }
    else
    {
        aCurPut[0] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) >> 24);
        aCurPut[1] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) >> 16 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) >> 8 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) & 0xFF);
    }
}

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, unsigned long int aVal) const
{
#ifdef GENIO_LONG64
    if (!(reinterpret_cast<unsigned long>(aCurPut) & 0x7))
    {
#ifdef GENIO_BIG_ENDIAN
        *reinterpret_cast<unsigned long*>(aCurPut) = aVal;
#else
        *reinterpret_cast<unsigned long*>(aCurPut) =
            (aVal >> 56) | (aVal >> 40 & UI64(0x000000000000FF00)) | (aVal >> 24 & UI64(0x0000000000FF0000)) | (aVal >> 8 & UI64(0x00000000FF000000)) | (aVal << 8 & UI64(0x000000FF00000000)) | (aVal << 24 & UI64(0x0000FF0000000000)) | (aVal << 40 & UI64(0x00FF000000000000)) | (aVal << 56);
#endif
    }
    else
    {
        aCurPut[0] = static_cast<unsigned char>(aVal >> 56);
        aCurPut[1] = static_cast<unsigned char>(aVal >> 48 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(aVal >> 40 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(aVal >> 32 & 0xFF);
        aCurPut[4] = static_cast<unsigned char>(aVal >> 24 & 0xFF);
        aCurPut[5] = static_cast<unsigned char>(aVal >> 16 & 0xFF);
        aCurPut[6] = static_cast<unsigned char>(aVal >> 8 & 0xFF);
        aCurPut[7] = static_cast<unsigned char>(aVal & 0xFF);
    }
#else
    // Transfer data in one assignment if on correct boundary, else do it byte by byte
    if (!(reinterpret_cast<size_t>(aCurPut) & 0x3))
    {
#ifdef GENIO_BIG_ENDIAN
        *reinterpret_cast<unsigned long*>(aCurPut) = aVal;
#else
        *reinterpret_cast<unsigned long*>(aCurPut) =
            (aVal >> 24) | (aVal >> 8 & 0x0000FF00) | (aVal << 8 & 0x00FF0000) | (aVal << 24);
#endif
    }
    else
    {
        aCurPut[0] = static_cast<unsigned char>(aVal >> 24);
        aCurPut[1] = static_cast<unsigned char>(aVal >> 16 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(aVal >> 8 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(aVal & 0xFF);
    }
#endif
}

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, long int aVal) const
{
#ifdef GENIO_LONG64
    if (!(reinterpret_cast<unsigned long>(aCurPut) & 0x7))
    {
#ifdef GENIO_BIG_ENDIAN
        *reinterpret_cast<long*>(aCurPut) = aVal;
#else
        *reinterpret_cast<unsigned long*>(aCurPut) =
            (*reinterpret_cast<unsigned long*>(&aVal) >> 56) | (*reinterpret_cast<unsigned long*>(&aVal) >> 40 & UI64(0x000000000000FF00)) | (*reinterpret_cast<unsigned long*>(&aVal) >> 24 & UI64(0x0000000000FF0000)) | (*reinterpret_cast<unsigned long*>(&aVal) >> 8 & UI64(0x00000000FF000000)) | (*reinterpret_cast<unsigned long*>(&aVal) << 8 & UI64(0x000000FF00000000)) | (*reinterpret_cast<unsigned long*>(&aVal) << 24 & UI64(0x0000FF0000000000)) | (*reinterpret_cast<unsigned long*>(&aVal) << 40 & UI64(0x00FF000000000000)) | (*reinterpret_cast<unsigned long*>(&aVal) << 56);
#endif
    }
    else
    {
        aCurPut[0] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 56);
        aCurPut[1] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 48 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 40 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 32 & 0xFF);
        aCurPut[4] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 24 & 0xFF);
        aCurPut[5] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 16 & 0xFF);
        aCurPut[6] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 8 & 0xFF);
        aCurPut[7] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) & 0xFF);
    }
#else
    // Transfer data in one assignment if on correct boundary, else do it byte by byte
    if (!(reinterpret_cast<size_t>(aCurPut) & 0x3))
    {
#ifdef GENIO_BIG_ENDIAN
        *reinterpret_cast<long*>(aCurPut) = aVal;
#else
        *reinterpret_cast<unsigned long*>(aCurPut) =
            (*reinterpret_cast<unsigned long*>(&aVal) >> 24) | (*reinterpret_cast<unsigned long*>(&aVal) >> 8 & 0x0000FF00) | (*reinterpret_cast<unsigned long*>(&aVal) << 8 & 0x00FF0000) | (*reinterpret_cast<unsigned long*>(&aVal) << 24);
#endif
    }
    else
    {
        aCurPut[0] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 24);
        aCurPut[1] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 16 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) >> 8 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(*reinterpret_cast<unsigned long*>(&aVal) & 0xFF);
    }
#endif
}

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, GENIO_UINT64 aVal) const
{
    // Transfer data in one assignment if on correct boundary, else do it byte by byte
    if (!(reinterpret_cast<size_t>(aCurPut) & 0x7))
    {
#if defined GENIO_BIG_ENDIAN
        *reinterpret_cast<GENIO_UINT64*>(aCurPut) = aVal;
#else
        *reinterpret_cast<GENIO_UINT64*>(aCurPut) =
            (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 56) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 40 & UI64(0x000000000000FF00)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 24 & UI64(0x0000000000FF0000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 8 & UI64(0x00000000FF000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 8 & UI64(0x000000FF00000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 24 & UI64(0x0000FF0000000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 40 & UI64(0x00FF000000000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 56);
#endif
    }
    else
    {
        aCurPut[0] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 56 & 0xFF);
        aCurPut[1] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 48 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 40 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 32 & 0xFF);
        aCurPut[4] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 24 & 0xFF);
        aCurPut[5] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 16 & 0xFF);
        aCurPut[6] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 8 & 0xFF);
        aCurPut[7] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) & 0xFF);
    }
}

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, GENIO_INT64 aVal) const
{
    // Transfer data in one assignment if on correct boundary, else do it byte by byte
    if (!(reinterpret_cast<size_t>(aCurPut) & 0x7))
    {
#if defined GENIO_BIG_ENDIAN
        *reinterpret_cast<GENIO_INT64*>(aCurPut) = aVal;
#else
        *reinterpret_cast<GENIO_INT64*>(aCurPut) =
            (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 56) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 40 & UI64(0x000000000000FF00)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 24 & UI64(0x0000000000FF0000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 8 & UI64(0x00000000FF000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 8 & UI64(0x000000FF00000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 24 & UI64(0x0000FF0000000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 40 & UI64(0x00FF000000000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 56);
#endif
    }
    else
    {
        aCurPut[0] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 56 & 0xFF);
        aCurPut[1] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 48 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 40 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 32 & 0xFF);
        aCurPut[4] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 24 & 0xFF);
        aCurPut[5] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 16 & 0xFF);
        aCurPut[6] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 8 & 0xFF);
        aCurPut[7] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) & 0xFF);
    }
}

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, float aVal) const
{
    // Transfer data in one assignment if on correct boundary, else do it byte by byte
    if (!(reinterpret_cast<size_t>(aCurPut) & 0x3))
    {
#if defined GENIO_VAX_G_FLOAT
        // Check for zero
        if (aVal)
        {
            *reinterpret_cast<unsigned int*>(aCurPut) = (*reinterpret_cast<unsigned int*>(&aVal) >> 8 & 0x00FF00FF | *reinterpret_cast<unsigned int*>(&aVal) << 8 & 0xFF00FF00) - 1;
        }
        else
        {
            *reinterpret_cast<unsigned int*>(aCurPut) = 0;
        }
#elif defined GENIO_BIG_ENDIAN
        *reinterpret_cast<float*>(aCurPut) = aVal;
#else
        *reinterpret_cast<unsigned int*>(aCurPut) =
            (*reinterpret_cast<unsigned int*>(&aVal) >> 24) | (*reinterpret_cast<unsigned int*>(&aVal) >> 8 & 0x0000FF00) | (*reinterpret_cast<unsigned int*>(&aVal) << 8 & 0x00FF0000) | (*reinterpret_cast<unsigned int*>(&aVal) << 24);
#endif
    }
    else
    {
#ifdef GENIO_VAX_G_FLOAT
        // Check for zero
        if (aVal)
        {
            aCurPut[0] = static_cast<unsigned char>((*reinterpret_cast<unsigned int*>(&aVal) >> 8 & 0xFF) - 1);
            aCurPut[1] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) & 0xFF);
            aCurPut[2] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) >> 24);
            aCurPut[3] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) >> 16 & 0xFF);
        }
        else
        {
            aCurPut[0] = 0;
            aCurPut[1] = 0;
            aCurPut[2] = 0;
            aCurPut[3] = 0;
        }
#else
        aCurPut[0] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) >> 24);
        aCurPut[1] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) >> 16 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) >> 8 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(*reinterpret_cast<unsigned int*>(&aVal) & 0xFF);
#endif
    }
}

void GenOConvertBigEndian::PutToBuf(unsigned char* aCurPut, double aVal) const
{
    // Transfer data in one assignment if on correct boundary, else do it byte by byte
    if (!(reinterpret_cast<size_t>(aCurPut) & 0x7))
    {
#if defined GENIO_VAX_G_FLOAT
        // Check for zero
        if (aVal)
        {
            // Subtract 2 from exponent before swap
            GENIO_UINT64 tempVal = *reinterpret_cast<GENIO_UINT64*>(&aVal) - 0x20;

            *reinterpret_cast<GENIO_UINT64*>(aCurPut) =
                tempVal >> 8 & UI64(0x00FF00FF00FF00FF) | tempVal << 8 & UI64(0xFF00FF00FF00FF00);
        }
        else
        {
            *reinterpret_cast<GENIO_UINT64*>(aCurPut) = 0;
        }
#elif defined GENIO_BIG_ENDIAN
        *reinterpret_cast<double*>(aCurPut) = aVal;
#else
        *reinterpret_cast<GENIO_UINT64*>(aCurPut) =
            (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 56) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 40 & UI64(0x000000000000FF00)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 24 & UI64(0x0000000000FF0000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 8 & UI64(0x00000000FF000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 8 & UI64(0x000000FF00000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 24 & UI64(0x0000FF0000000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 40 & UI64(0x00FF000000000000)) | (*reinterpret_cast<GENIO_UINT64*>(&aVal) << 56);
#endif
    }
    else
    {
#ifdef GENIO_VAX_G_FLOAT
        // Check for zero
        if (aVal)
        {
            // Subtract 2 from the exponent before swap
            GENIO_UINT64 tempVal = (*reinterpret_cast<GENIO_UINT64*>(&aVal)) - 0x20;
            aCurPut[0] = static_cast<unsigned char>(tempVal >> 8 & 0xFF);
            aCurPut[1] = static_cast<unsigned char>(tempVal & 0xFF);
            aCurPut[2] = static_cast<unsigned char>(tempVal >> 24 & 0xFF);
            aCurPut[3] = static_cast<unsigned char>(tempVal >> 16 & 0xFF);
            aCurPut[4] = static_cast<unsigned char>(tempVal >> 40 & 0xFF);
            aCurPut[5] = static_cast<unsigned char>(tempVal >> 32 & 0xFF);
            aCurPut[6] = static_cast<unsigned char>(tempVal >> 56 & 0xFF);
            aCurPut[7] = static_cast<unsigned char>(tempVal >> 48 & 0xFF);
        }
        else
        {
            aCurPut[0] = 0;
            aCurPut[1] = 0;
            aCurPut[2] = 0;
            aCurPut[3] = 0;
            aCurPut[4] = 0;
            aCurPut[5] = 0;
            aCurPut[6] = 0;
            aCurPut[7] = 0;
        }
#else
        aCurPut[0] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 56 & 0xFF);
        aCurPut[1] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 48 & 0xFF);
        aCurPut[2] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 40 & 0xFF);
        aCurPut[3] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 32 & 0xFF);
        aCurPut[4] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 24 & 0xFF);
        aCurPut[5] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 16 & 0xFF);
        aCurPut[6] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) >> 8 & 0xFF);
        aCurPut[7] = static_cast<unsigned char>(*reinterpret_cast<GENIO_UINT64*>(&aVal) & 0xFF);
#endif
    }
}
