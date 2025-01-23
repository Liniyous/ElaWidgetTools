// GenOConvert - An abstract base class for the GenO byte swapping routines

#ifndef GENO_CONVERT_BIG_ENDIAN_H
#define GENO_CONVERT_BIG_ENDIAN_H

#include "ElaPacketIO_Export.h"

#include "GenOConvert.h"

class ELA_PACKETIO_EXPORT GenOConvertBigEndian : public GenOConvert
{
public:
    // Constructor
    GenOConvertBigEndian();

    ~GenOConvertBigEndian() override;

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
    void PutToBuf(unsigned char* aCurPut, float aValue) const override;
    void PutToBuf(unsigned char* aCurPut, double aValue) const override;
};

#endif
