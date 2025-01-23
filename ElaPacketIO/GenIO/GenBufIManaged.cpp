#include "GenBufIManaged.h"

GenBufIManaged::GenBufIManaged(BufType aBufType, unsigned long aSize, bool aAllowGrowth)
    : GenBuf(aSize, aAllowGrowth), GenBufXferI(aBufType)
{
}

GenBufIManaged::~GenBufIManaged() {}
