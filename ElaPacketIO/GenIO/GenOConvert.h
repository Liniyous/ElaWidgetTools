// GenOConvert - An abstract base class for the GenO byte swapping routines

#ifndef GENO_CONVERT_H
#define GENO_CONVERT_H

#include "ElaPacketIO_Export.h"

#include "GenIODefs.h"

class ELA_PACKETIO_EXPORT GenOConvert
{
public:
    virtual ~GenOConvert();

    // Virtual putValue functions to be overridden by derived class
    virtual void PutToBuf(unsigned char* aCurPut, unsigned short aValue) const = 0;
    virtual void PutToBuf(unsigned char* aCurPut, short aValue) const = 0;
    virtual void PutToBuf(unsigned char* aCurPut, unsigned int aValue) const = 0;
    virtual void PutToBuf(unsigned char* aCurPut, int aValue) const = 0;
    virtual void PutToBuf(unsigned char* aCurPut, unsigned long aValue) const = 0;
    virtual void PutToBuf(unsigned char* aCurPut, long aValue) const = 0;
    virtual void PutToBuf(unsigned char* aCurPut, GENIO_UINT64 aValue) const = 0;
    virtual void PutToBuf(unsigned char* aCurPut, GENIO_INT64 aValue) const = 0;
    virtual void PutToBuf(unsigned char* aCurPut, float aValue) const = 0;
    virtual void PutToBuf(unsigned char* aCurPut, double aValue) const = 0;

protected:
    // Constructor
    GenOConvert();

private:
    // No coping of this class or its derived classes allowed
    GenOConvert& operator=(const GenOConvert& aRhs);
    GenOConvert(const GenOConvert& aSrc);
};

#endif
