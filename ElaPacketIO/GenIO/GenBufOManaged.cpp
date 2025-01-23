#include "GenBufOManaged.h"

GenBufOManaged::GenBufOManaged(BufType aBufType, unsigned long aSize, unsigned int aAllowGrowthOnPuts)
   : GenBuf(aSize, aAllowGrowthOnPuts)
   , GenBufXferO(aBufType)
{
}


GenBufOManaged::~GenBufOManaged() {}
