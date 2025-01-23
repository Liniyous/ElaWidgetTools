#ifndef GENI_CONVERT_LIT_ENDIAN_INT_H
#define GENI_CONVERT_LIT_ENDIAN_INT_H

#include "ElaPacketIO_Export.h"

// This gets data out of a Big Endian buffer

#include "GenIConvert.h"

#if defined(sgi) && (_COMPILER_VERSION >= 720)
#pragma set woff 1681, 1682
#endif

class ELA_PACKETIO_EXPORT GenIConvertLitEndianInt : public GenIConvert
{
public:
    GenIConvertLitEndianInt();

    ~GenIConvertLitEndianInt() override;

    // Functions for moving data from a buffer to a variable.
    // The data is not assumed to be aligned in the buffer.

    void GetFromBuf(const unsigned char* aCurGet, unsigned short& aValue) const override;
    void GetFromBuf(const unsigned char* aCurGet, short& aValue) const override;
    void GetFromBuf(const unsigned char* aCurGet, unsigned int& aValue) const override;
    void GetFromBuf(const unsigned char* aCurGet, int& aValue) const override;
    void GetFromBuf(const unsigned char* aCurGet, unsigned long& aValue) const override;
    void GetFromBuf(const unsigned char* aCurGet, long& aValue) const override;
    void GetFromBuf(const unsigned char* aCurGet, GENIO_UINT64& aValue) const override;
    void GetFromBuf(const unsigned char* aCurGet, GENIO_INT64& aValue) const override;
};

#if defined(sgi) && (_COMPILER_VERSION >= 720)
#pragma reset woff 1681, 1682
#endif

#endif
