#ifndef GENUDP_IO_H
#define GENUDP_IO_H

#include "ElaPacketIO_Export.h"

#include <string>
#include <vector>

#include "GenIP.h"
#include "GenNetIO.h"
#include "GenUDP_Connection.h"

namespace GenSockets
{
class GenInternetSocketAddress;
class GenSocket;
} // namespace GenSockets

/** Provides User Datagram Protocol communication with a GenIO interface

   Supports broadcast and multicast.
*/

class ELA_PACKETIO_EXPORT GenUDP_IO : public GenUDP_Connection, public GenNetIO
{
public:
    GenUDP_IO();

    ~GenUDP_IO() override;

    int Receive(int aWaitTimeInMicroSec) override;
    int Send() override;

    bool GetSenderId(unsigned int& aSrcInetAddr, unsigned short int& aSrcPort) const override
    {
        return GenUDP_Connection::GetSenderId(aSrcInetAddr, aSrcPort);
    }

    int GetLocalPort() const override { return GenUDP_Connection::GetLocalPort(); }
};
#endif
