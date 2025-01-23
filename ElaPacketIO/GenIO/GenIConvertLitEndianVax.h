#ifndef GENI_CONVERT_LIT_ENDIAN_VAX_H
#define GENI_CONVERT_LIT_ENDIAN_VAX_H

#include "ElaPacketIO_Export.h"

// This gets data out of a little Endian buffer

#include "GenIConvertLitEndianInt.h"

#if defined(sgi) && (_COMPILER_VERSION >= 720)
#pragma set woff 1681, 1682
#endif

class ELA_PACKETIO_EXPORT GenIConvertLitEndianVax : public GenIConvertLitEndianInt
{
public:
    GenIConvertLitEndianVax();

    ~GenIConvertLitEndianVax() override;

    // Functions for moving data from a buffer to a variable.
    // The data is not assumed to be aligned in the buffer.

    void GetFromBuf(const unsigned char* aCurGet, float& aValue) const override;
    void GetFromBuf(const unsigned char* aCurGet, double& aValue) const override;
};

#if defined(sgi) && (_COMPILER_VERSION >= 720)
#pragma reset woff 1681, 1682
#endif

#endif
