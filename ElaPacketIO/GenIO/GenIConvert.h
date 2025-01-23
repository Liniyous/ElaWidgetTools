#ifndef GENI_CONVERT_H
#define GENI_CONVERT_H

#include "ElaPacketIO_Export.h"

#include "GenIODefs.h"

class ELA_PACKETIO_EXPORT GenIConvert
{
public:
    virtual ~GenIConvert();

    // Virtual getValue functions to be overridden by derived class
    virtual void GetFromBuf(const unsigned char* aCurGet, unsigned short& aValue) const = 0;
    virtual void GetFromBuf(const unsigned char* aCurGet, short& aValue) const = 0;
    virtual void GetFromBuf(const unsigned char* aCurGet, unsigned int& aValue) const = 0;
    virtual void GetFromBuf(const unsigned char* aCurGet, int& aValue) const = 0;
    virtual void GetFromBuf(const unsigned char* aCurGet, unsigned long& aValue) const = 0;
    virtual void GetFromBuf(const unsigned char* aCurGet, long& aValue) const = 0;
    virtual void GetFromBuf(const unsigned char* aCurGet, GENIO_UINT64& aValue) const = 0;
    virtual void GetFromBuf(const unsigned char* aCurGet, GENIO_INT64& aValue) const = 0;
    virtual void GetFromBuf(const unsigned char* aCurGet, float& aValue) const = 0;
    virtual void GetFromBuf(const unsigned char* aCurGet, double& aValue) const = 0;

protected:
    // Constructor
    GenIConvert();

private:
    // No coping of this class or its derived classes allowed
    GenIConvert& operator=(const GenIConvert& aRhs);
    GenIConvert(const GenIConvert& aSrc);
};

#endif
