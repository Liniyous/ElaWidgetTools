#ifndef UTSEMAPHORE_H
#define UTSEMAPHORE_H

#include "ElaPacketIO_Export.h"

class UtSemaphoreImpl;

class ELA_PACKETIO_EXPORT UtSemaphore
{
public:
    UtSemaphore(int aInitialCount);
    UtSemaphore(const UtSemaphore& aSrc) = delete;
    UtSemaphore& operator=(const UtSemaphore&) = delete;
    ~UtSemaphore();

    void Acquire();
    void Release(unsigned int aCount = 1);
    bool TryAcquire();

private:
    UtSemaphoreImpl* mImplPtr;
};

#endif
