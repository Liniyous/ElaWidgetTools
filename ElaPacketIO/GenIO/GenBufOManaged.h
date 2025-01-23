// GenBufOManaged - This class is used to put data into a output buffer that this
//                  class owns.

#ifndef GEN_BUF_O_MANAGED_H
#define GEN_BUF_O_MANAGED_H

#include "ElaPacketIO_Export.h"

#include "GenBufXferO.h"

class ELA_PACKETIO_EXPORT GenBufOManaged : public GenBufXferO
{
public:
    // Constructor will prepare class for a buffer of type "aBufType"
    GenBufOManaged(BufType aBufType, unsigned long aSize, unsigned int aAllowGrowthOnPuts = 0);

    ~GenBufOManaged() override;

    // Change buffer currently being used.
    // This function is not typically called and should be used with great care.
    void ChangeBuffer(unsigned char* aUserBuffer, unsigned long aSizeInBytes, unsigned long aNumValidBytes);

    // getValue a access to the buffer.
    const unsigned char* GetBuffer();
};

inline void GenBufOManaged::ChangeBuffer(unsigned char* aUserBuffer, unsigned long aSizeInBytes, unsigned long aNumValidBytes)
{
    SetLocalBuffer(aUserBuffer, aSizeInBytes, aNumValidBytes);
}

inline const unsigned char* GenBufOManaged::GetBuffer()
{
    return GetLocalBuffer();
}

#endif
