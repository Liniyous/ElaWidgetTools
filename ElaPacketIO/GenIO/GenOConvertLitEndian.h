// GenOConvert - An abstract base class for the GenO byte swapping routines

#ifndef GENO_CONVERT_LIT_ENDIAN_H
#define GENO_CONVERT_LIT_ENDIAN_H

#include "ElaPacketIO_Export.h"

#include "GenOConvertLitEndianInt.h"

#if defined(sgi) && (_COMPILER_VERSION >= 720)
#pragma set woff 1681, 1682
#endif

class ELA_PACKETIO_EXPORT GenOConvertLitEndian : public GenOConvertLitEndianInt
{
public:
    // Constructor
    GenOConvertLitEndian();

    ~GenOConvertLitEndian() override;

    // Functions for moving data into a buffer from a variable.
    // The data is not assumed to be aligned in the buffer.
    void PutToBuf(unsigned char* aCurPut, float aValue) const override;
    void PutToBuf(unsigned char* aCurPut, double aValue) const override;
};

#if defined(sgi) && (_COMPILER_VERSION >= 720)
#pragma reset woff 1681, 1682
#endif

#endif
