#include "GenIConvertLitEndianInt.h"

#include <cstddef> // for size_t

#ifdef _WIN32
#pragma warning(disable : 4311)
#endif

//  This Class inherits from GenIOBuf

// This class is used to get integers from a little endian buffer.
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


GenIConvertLitEndianInt::GenIConvertLitEndianInt() {}


// Virtual
GenIConvertLitEndianInt::~GenIConvertLitEndianInt() {}


// *****************************
//
// The Get's
//
// *****************************

void GenIConvertLitEndianInt::GetFromBuf(const unsigned char* aCurGet, unsigned short int& aVal) const
{
   aVal = aCurGet[0] | aCurGet[1] << 8;
}

void GenIConvertLitEndianInt::GetFromBuf(const unsigned char* aCurGet, short int& aVal) const
{
   aVal = aCurGet[0] | aCurGet[1] << 8;
}

void GenIConvertLitEndianInt::GetFromBuf(const unsigned char* aCurGet, unsigned int& aVal) const
{
   // Transfer data in one assignment if on correct boundary, else do it byte by byte
   if (!(reinterpret_cast<size_t>(aCurGet) & 0x3))
   {
#ifdef GENIO_LIT_ENDIAN
      aVal = *reinterpret_cast<const unsigned int*>(aCurGet);
#else
      aVal = *reinterpret_cast<const unsigned int*>(aCurGet) >> 24 |
             *reinterpret_cast<const unsigned int*>(aCurGet) >> 8 & 0x0000FF00 |
             *reinterpret_cast<const unsigned int*>(aCurGet) << 8 & 0x00FF0000 |
             *reinterpret_cast<const unsigned int*>(aCurGet) << 24;
#endif
   }
   else
   {
      aVal = aCurGet[0] | aCurGet[1] << 8 | aCurGet[2] << 16 | aCurGet[3] << 24;
   }
}

void GenIConvertLitEndianInt::GetFromBuf(const unsigned char* aCurGet, int& aVal) const
{
   // Transfer data in one assignment if on correct boundary, else do it byte by byte
   if (!(reinterpret_cast<size_t>(aCurGet) & 0x3))
   {
#ifdef GENIO_LIT_ENDIAN
      aVal = *reinterpret_cast<const int*>(aCurGet);
#else
      *reinterpret_cast<unsigned int*>(&aVal) = *reinterpret_cast<const unsigned int*>(aCurGet) >> 24 |
                                                *reinterpret_cast<const unsigned int*>(aCurGet) >> 8 & 0x0000FF00 |
                                                *reinterpret_cast<const unsigned int*>(aCurGet) << 8 & 0x00FF0000 |
                                                *reinterpret_cast<const unsigned int*>(aCurGet) << 24;
#endif
   }
   else
   {
      *reinterpret_cast<unsigned int*>(&aVal) = aCurGet[0] | aCurGet[1] << 8 | aCurGet[2] << 16 | aCurGet[3] << 24;
   }
}

void GenIConvertLitEndianInt::GetFromBuf(const unsigned char* aCurGet, unsigned long int& aVal) const
{
#ifdef GENIO_LONG64
   // Transfer data in one assignment if on correct boundary, else do it byte by byte
   if (!(reinterpret_cast<unsigned long>(aCurGet) & 0x7))
   {
#ifdef GENIO_LIT_ENDIAN
      aVal = *reinterpret_cast<const unsigned long*>(aCurGet);
#else
      aVal = *reinterpret_cast<const unsigned long*>(aCurGet) >> 56 |
             *reinterpret_cast<const unsigned long*>(aCurGet) >> 40 & UI64(0x000000000000FF00) |
             *reinterpret_cast<const unsigned long*>(aCurGet) >> 24 & UI64(0x0000000000FF0000) |
             *reinterpret_cast<const unsigned long*>(aCurGet) >> 8 & UI64(0x00000000FF000000) |
             *reinterpret_cast<const unsigned long*>(aCurGet) << 8 & UI64(0x000000FF00000000) |
             *reinterpret_cast<const unsigned long*>(aCurGet) << 24 & UI64(0x0000FF0000000000) |
             *reinterpret_cast<const unsigned long*>(aCurGet) << 40 & UI64(0x00FF000000000000) |
             *reinterpret_cast<const unsigned long*>(aCurGet) << 56;
#endif
   }
   else
   {
      aVal = static_cast<unsigned long>(aCurGet[0]) | static_cast<unsigned long>(aCurGet[1]) << 8 |
             static_cast<unsigned long>(aCurGet[2]) << 16 | static_cast<unsigned long>(aCurGet[3]) << 24 |
             static_cast<unsigned long>(aCurGet[4]) << 32 | static_cast<unsigned long>(aCurGet[5]) << 40 |
             static_cast<unsigned long>(aCurGet[6]) << 48 | static_cast<unsigned long>(aCurGet[7]) << 56;
   }
#else
   // Transfer data in one assignment if on correct boundary, else do it byte by byte
   if (!(reinterpret_cast<size_t>(aCurGet) & 0x3))
   {
#ifdef GENIO_LIT_ENDIAN
      aVal = *reinterpret_cast<const unsigned long*>(aCurGet);
#else
      aVal = *reinterpret_cast<const unsigned long*>(aCurGet) >> 24 |
             *reinterpret_cast<const unsigned long*>(aCurGet) >> 8 & 0x0000FF00 |
             *reinterpret_cast<const unsigned long*>(aCurGet) << 8 & 0x00FF0000 |
             *reinterpret_cast<const unsigned long*>(aCurGet) << 24;
#endif
   }
   else
   {
      aVal = aCurGet[0] | aCurGet[1] << 8 | aCurGet[2] << 16 | aCurGet[3] << 24;
   }
#endif
}

void GenIConvertLitEndianInt::GetFromBuf(const unsigned char* aCurGet, long int& aVal) const
{
#ifdef GENIO_LONG64
   // Transfer data in one assignment if on correct boundary, else do it byte by byte
   if (!(reinterpret_cast<unsigned long>(aCurGet) & 0x7))
   {
#ifdef GENIO_LIT_ENDIAN
      aVal = *reinterpret_cast<const long*>(aCurGet);
#else
      *reinterpret_cast<unsigned long*>(&aVal) =
         *reinterpret_cast<const unsigned long*>(aCurGet) >> 56 |
         *reinterpret_cast<const unsigned long*>(aCurGet) >> 40 & UI64(0x000000000000FF00) |
         *reinterpret_cast<const unsigned long*>(aCurGet) >> 24 & UI64(0x0000000000FF0000) |
         *reinterpret_cast<const unsigned long*>(aCurGet) >> 8 & UI64(0x00000000FF000000) |
         *reinterpret_cast<const unsigned long*>(aCurGet) << 8 & UI64(0x000000FF00000000) |
         *reinterpret_cast<const unsigned long*>(aCurGet) << 24 & UI64(0x0000FF0000000000) |
         *reinterpret_cast<const unsigned long*>(aCurGet) << 40 & UI64(0x00FF000000000000) |
         *reinterpret_cast<const unsigned long*>(aCurGet) << 56;
#endif
   }
   else
   {
      *reinterpret_cast<unsigned long*>(&aVal) =
         static_cast<unsigned long>(aCurGet[0]) | static_cast<unsigned long>(aCurGet[1]) << 8 |
         static_cast<unsigned long>(aCurGet[2]) << 16 | static_cast<unsigned long>(aCurGet[3]) << 24 |
         static_cast<unsigned long>(aCurGet[4]) << 32 | static_cast<unsigned long>(aCurGet[5]) << 40 |
         static_cast<unsigned long>(aCurGet[6]) << 48 | static_cast<unsigned long>(aCurGet[7]) << 56;
   }
#else
   // Transfer data in one assignment if on correct boundary, else do it byte by byte
   if (!(reinterpret_cast<size_t>(aCurGet) & 0x3))
   {
#ifdef GENIO_LIT_ENDIAN
      aVal = *reinterpret_cast<const long*>(aCurGet);
#else
      *reinterpret_cast<unsigned long*>(&aVal) = *reinterpret_cast<const unsigned long*>(aCurGet) >> 24 |
                                                 *reinterpret_cast<const unsigned long*>(aCurGet) >> 8 & 0x0000FF00 |
                                                 *reinterpret_cast<const unsigned long*>(aCurGet) << 8 & 0x00FF0000 |
                                                 *reinterpret_cast<const unsigned long*>(aCurGet) << 24;
#endif
   }
   else
   {
      *reinterpret_cast<unsigned long*>(&aVal) = aCurGet[0] | aCurGet[1] << 8 | aCurGet[2] << 16 | aCurGet[3] << 24;
   }
#endif
}

void GenIConvertLitEndianInt::GetFromBuf(const unsigned char* aCurGet, GENIO_UINT64& aVal) const
{
   // Transfer data in one assignment if on correct boundary, else do it byte by byte
   if (!(reinterpret_cast<size_t>(aCurGet) & 0x7))
   {
#if defined GENIO_LIT_ENDIAN
      aVal = *reinterpret_cast<const GENIO_UINT64*>(aCurGet);
#else
      *reinterpret_cast<GENIO_UINT64*>(&aVal) =
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) >> 56 |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) >> 40 & UI64(0x000000000000FF00) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) >> 24 & UI64(0x0000000000FF0000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) >> 8 & UI64(0x00000000FF000000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) << 8 & UI64(0x000000FF00000000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) << 24 & UI64(0x0000FF0000000000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) << 40 & UI64(0x00FF000000000000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) << 56;
#endif
   }
   else
   {
      *reinterpret_cast<GENIO_UINT64*>(&aVal) =
         static_cast<GENIO_UINT64>(aCurGet[0]) | static_cast<GENIO_UINT64>(aCurGet[1]) << 8 |
         static_cast<GENIO_UINT64>(aCurGet[2]) << 16 | static_cast<GENIO_UINT64>(aCurGet[3]) << 24 |
         static_cast<GENIO_UINT64>(aCurGet[4]) << 32 | static_cast<GENIO_UINT64>(aCurGet[5]) << 40 |
         static_cast<GENIO_UINT64>(aCurGet[6]) << 48 | static_cast<GENIO_UINT64>(aCurGet[7]) << 56;
   }
}

void GenIConvertLitEndianInt::GetFromBuf(const unsigned char* aCurGet, GENIO_INT64& aVal) const
{
   // Transfer data in one assignment if on correct boundary, else do it byte by byte
   if (!(reinterpret_cast<size_t>(aCurGet) & 0x7))
   {
#if defined GENIO_LIT_ENDIAN
      aVal = *reinterpret_cast<const GENIO_INT64*>(aCurGet);
#else
      *reinterpret_cast<GENIO_INT64*>(&aVal) =
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) >> 56 |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) >> 40 & UI64(0x000000000000FF00) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) >> 24 & UI64(0x0000000000FF0000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) >> 8 & UI64(0x00000000FF000000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) << 8 & UI64(0x000000FF00000000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) << 24 & UI64(0x0000FF0000000000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) << 40 & UI64(0x00FF000000000000) |
         *reinterpret_cast<const GENIO_UINT64*>(aCurGet) << 56;
#endif
   }
   else
   {
      *reinterpret_cast<GENIO_INT64*>(&aVal) =
         static_cast<GENIO_UINT64>(aCurGet[0]) | static_cast<GENIO_UINT64>(aCurGet[1]) << 8 |
         static_cast<GENIO_UINT64>(aCurGet[2]) << 16 | static_cast<GENIO_UINT64>(aCurGet[3]) << 24 |
         static_cast<GENIO_UINT64>(aCurGet[4]) << 32 | static_cast<GENIO_UINT64>(aCurGet[5]) << 40 |
         static_cast<GENIO_UINT64>(aCurGet[6]) << 48 | static_cast<GENIO_UINT64>(aCurGet[7]) << 56;
   }
}
