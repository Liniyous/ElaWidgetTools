#ifndef GEN_BUF_I_MANAGED_H
#define GEN_BUF_I_MANAGED_H

#include "ElaPacketIO_Export.h"

#include "GenBufXferI.h"

// GenBufIManaged - This class is used to get data from a input buffer that this
// class owns.

class ELA_PACKETIO_EXPORT GenBufIManaged : public GenBufXferI
{
public:
    // Constructor will prepare class for a buffer of type "aBufType"
    GenBufIManaged(BufType aBufType, unsigned long aSize, bool aAllowGrowth = false);

    ~GenBufIManaged() override;

    // getValue a access to the buffer.  User is responsible to not overflow the
    // buffer
    unsigned char* GetBuffer();
};

inline unsigned char* GenBufIManaged::GetBuffer()
{
    return GetLocalBuffer();
}

#endif
