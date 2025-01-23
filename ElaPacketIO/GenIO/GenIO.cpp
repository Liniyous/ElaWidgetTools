#include "GenIO.h"

GenIO::GenIO(const char* aRecvBufName, const char* aSendBufName)
    : GenI(aRecvBufName), GenO(aSendBufName)
{
}

GenIO::~GenIO() {}
