#ifndef GENSOCKETSELECTOR_H
#define GENSOCKETSELECTOR_H

#include "ElaPacketIO_Export.h"

#include "GenSocketSet.h"

namespace GenSockets
{
class GenSocket;
class ELA_PACKETIO_EXPORT GenSocketSelector : public GenSocketSet
{
public:
    enum WaitEvent
    {
        cREAD = 1,
        cWRITE = 2,
        cEXCEPTION = 4,
        cALL = cREAD | cWRITE | cEXCEPTION
    };

    enum SelectResult
    {
        //! Wait time has expired without any events
        cTIMEOUT,
        //! One or more socket has been signaled.
        cREADY,
        //! An error has occurred
        cERROR
    };

    static const float cBLOCK_FOREVER;
    GenSocketSelector();
    ~GenSocketSelector();
    SelectResult Select(GenSocketSet& aSignalledSocketSet, float aWaitTime = cBLOCK_FOREVER, int aSignalledEvent = cREAD);
};

} // namespace GenSockets
#endif
