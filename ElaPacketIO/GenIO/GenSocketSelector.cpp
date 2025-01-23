#include "GenSocketSelector.h"

#include <algorithm>
#include <cstring>
#include <iostream>

#include "GenSocket.h"
#include "GenSocketIncludes.h"

namespace GenSockets
{
const float GenSocketSelector::cBLOCK_FOREVER = 1.0E9;

GenSocketSelector::GenSocketSelector() {}

GenSocketSelector::~GenSocketSelector() {}

namespace
{

void BuildFD_Set(fd_set& aSet, std::vector<GenSocket*>& aSockets)
{
    FD_ZERO(&aSet);
    for (std::vector<GenSocket*>::iterator i = aSockets.begin(); i != aSockets.end(); ++i)
    {
        if ((*i)->GetSocketFileDescriptor() != 0)
        {
            FD_SET((*i)->GetSocketFileDescriptor(), &aSet);
        }
    }
}

} // namespace

//! Determines if a set of sockets are ready to read, write, and/or have exceptions.
//! @param aSignalledSocketSet The socket set filled with sockets that are ready
//! @param aWaitTime The duration to wait in seconds.  0.0 makes this a non-blocking
//!                  operation.  cBLOCK_FOREVER makes this block until a socket is ready.
//! @param aSignalledEvent  A combination of WaitEvent enumeration values.
GenSocketSelector::SelectResult GenSocketSelector::Select(GenSocketSet& aSignalledSocketSet,
                                                          float aWaitTime /* =cBLOCK_FOREVER */,
                                                          int aSignalledEvent /* =cALL */)
{
    aSignalledSocketSet.Clear();

    SelectResult result = cTIMEOUT;

    // Wait time cannot be negative, or select may return an error
    if (aWaitTime < 0.0f)
    {
        aWaitTime = 0.0f;
    }

    // Set up the timeout structure.  if (wait time >= cBLOCK_FOREVER) then wait forever.
    struct timeval* timeoutPtr = nullptr;
    struct timeval timeout;
    if (aWaitTime < cBLOCK_FOREVER)
    {
        timeoutPtr = &timeout;
        int waitSec = static_cast<int>(aWaitTime);
        timeout.tv_sec = waitSec;
        timeout.tv_usec = static_cast<long>((aWaitTime - waitSec) * 1000000.0f);
    }

    fd_set readSet, writeSet, exceptSet;
    fd_set* readPtr = nullptr;
    fd_set* writePtr = nullptr;
    fd_set* exceptPtr = nullptr;

    BuildFD_Set(readSet, mSockets);

    if (aSignalledEvent & cREAD)
    {
        readPtr = &readSet;
    }
    if (aSignalledEvent & cWRITE)
    {
        writePtr = &writeSet;
        memcpy(&writeSet, &readSet, sizeof(readSet));
    }
    if (aSignalledEvent & cEXCEPTION)
    {
        exceptPtr = &exceptSet;
        memcpy(&exceptSet, &readSet, sizeof(readSet));
    }

    int readyCount = select(FD_SETSIZE, readPtr, writePtr, exceptPtr, timeoutPtr);

    if (readyCount > 0)
    {
        for (unsigned int i = 0; i < mSockets.size(); ++i)
        {
            GenSockets::SockFd fd = mSockets[i]->GetSocketFileDescriptor();
            if ((readPtr && FD_ISSET(fd, readPtr)) || (writePtr && FD_ISSET(fd, writePtr)) || (exceptPtr && FD_ISSET(fd, exceptPtr)))
            {
                aSignalledSocketSet.AddSocket(mSockets[i]);
                result = cREADY;
            }
        }
    }
    else if (readyCount < 0)
    {
        result = cERROR;
    }

    return result;
}
} // namespace GenSockets
