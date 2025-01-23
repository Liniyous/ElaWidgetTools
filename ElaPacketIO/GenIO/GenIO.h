#ifndef GENIO_H
#define GENIO_H

#include "ElaPacketIO_Export.h"

#include "GenI.h"
#include "GenO.h"

class ELA_PACKETIO_EXPORT GenIO : public GenI, public GenO
{
public:
    GenIO(const char* aRecvBufName, const char* aSendBufName);
    ~GenIO() override;

private:
};

#endif
