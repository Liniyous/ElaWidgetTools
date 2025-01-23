#ifndef GENSOCKETCONNECTION_H
#define GENSOCKETCONNECTION_H

#include "ElaPacketIO_Export.h"

#include <memory>
#include <string>

#include "GenSocket.h"

//! Base class for a socket connection.  Not much more than a GenSocket.
class ELA_PACKETIO_EXPORT GenSocketConnection
{
public:
    GenSocketConnection(GenSockets::GenSocket* aSocket);

    virtual ~GenSocketConnection() = default;

    virtual int GetLocalPort() const;

    bool IsConnected() const;

    virtual bool IsNonBlocking() const;

    virtual void SetIsNonBlocking(bool aTF);

    std::unique_ptr<GenSockets::GenSocket> mSocket;
};

#endif
