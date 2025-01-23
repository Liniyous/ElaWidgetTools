#ifndef GENTCP_SERVER_H
#define GENTCP_SERVER_H

#include "ElaPacketIO_Export.h"

#include <cstddef> // for size_t
#include <vector>

class GenTCP_IO;

namespace GenSockets
{
class GenSocket;
}

class ELA_PACKETIO_EXPORT GenTCP_Server
{
public:
    GenTCP_Server();

    ~GenTCP_Server();

    bool Init(int aPortNumber);

    GenSockets::GenSocket* GetSocket() const { return mServerSocketPtr; }

    bool OwnsConnections();

    void SetOwnsConnections(bool aTF);

    GenTCP_IO* Accept(int aWaitTimeInMicroSeconds);

    void CloseAllConnections();

    void CloseConnection(GenTCP_IO* aConnectionPtr);

    size_t ConnectionCount() const { return mConnectionList.size(); }

    GenTCP_IO* GetConnection(size_t aIndex) const { return mConnectionList[aIndex]; }

protected:
    typedef std::vector<GenTCP_IO*> IoList;
    GenSockets::GenSocket* mServerSocketPtr;
    IoList mConnectionList;
    bool mOwnsConnections;
};
#endif
