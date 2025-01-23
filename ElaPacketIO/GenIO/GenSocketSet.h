#ifndef GENSOCKETSET_H
#define GENSOCKETSET_H

#include "ElaPacketIO_Export.h"

#include <vector>

namespace GenSockets
{
class GenSocket;

//! A container for GenSocket pointers
class ELA_PACKETIO_EXPORT GenSocketSet
{
public:
    //! Adds a socket to the set
    void AddSocket(GenSocket* aSocket);

    //! Remove a socket from the set
    void RemoveSocket(GenSocket* aSocket);

    //! Returns the number of sockets in this set
    unsigned int GetSocketCount() const { return static_cast<unsigned int>(mSockets.size()); }

    //! Returns the socket at the specified index
    GenSocket* GetSocketEntry(unsigned int aIndex) const { return mSockets[aIndex]; }

    //! Removes any sockets in the socket set.
    void Clear() { mSockets.clear(); }

    //! Returns true if there are no sockets in the set
    bool IsEmpty() const { return mSockets.empty(); }

protected:
    std::vector<GenSocket*> mSockets;
};

} // namespace GenSockets
#endif
