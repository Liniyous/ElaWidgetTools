// GenOConvert - An abstract base class for the GenO byte swapping routines

#ifndef GENO_CONVERT_LIT_ENDIAN_INT_H
#define GENO_CONVERT_LIT_ENDIAN_INT_H

#include "ElaPacketIO_Export.h"

#include "GenOConvert.h"

#if defined(sgi) && (_COMPILER_VERSION >= 720)
#pragma set woff 1681, 1682
#endif

class ELA_PACKETIO_EXPORT GenOConvertLitEndianInt : public GenOConvert
{
public:
    // Constructor
    GenOConvertLitEndianInt();

    ~GenOConvertLitEndianInt() override;

    // Functions for moving data into a buffer from a variable.
    // The data is not assumed to be aligned in the buffer.
    void PutToBuf(unsigned char* aCurPut, unsigned short aValue) const override;
    void PutToBuf(unsigned char* aCurPut, short aValue) const override;
    void PutToBuf(unsigned char* aCurPut, unsigned int aValue) const override;
    void PutToBuf(unsigned char* aCurPut, int aValue) const override;
    void PutToBuf(unsigned char* aCurPut, unsigned long aValue) const override;
    void PutToBuf(unsigned char* aCurPut, long aValue) const override;
    void PutToBuf(unsigned char* aCurPut, GENIO_UINT64 aValue) const override;
    void PutToBuf(unsigned char* aCurPut, GENIO_INT64 aValue) const override;
};

#if defined(sgi) && (_COMPILER_VERSION >= 720)
#pragma reset woff 1681, 1682
#endif

#endif
