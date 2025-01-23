#ifndef GENI_CONVERT_BIG_ENDIAN_H
#define GENI_CONVERT_BIG_ENDIAN_H

// This gets data out of a Big Endian buffer

#include "ElaPacketIO_Export.h"

#include "GenIConvert.h"

class ELA_PACKETIO_EXPORT GenIConvertBigEndian : public GenIConvert
{
public:
    GenIConvertBigEndian();

    ~GenIConvertBigEndian() override;

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
    void GetFromBuf(const unsigned char* aCurGet, float& aValue) const override;
    void GetFromBuf(const unsigned char* aCurGet, double& aValue) const override;
};

#endif
