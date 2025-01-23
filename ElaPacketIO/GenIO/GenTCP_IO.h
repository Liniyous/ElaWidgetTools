#ifndef GENTCP_IO_H
#define GENTCP_IO_H

#include "ElaPacketIO_Export.h"

#include <string>

#include "GenNetIO.h"
#include "GenTCP_Connection.h"

namespace GenSockets
{
class GenInternetSocketAddress;
class GenSocket;
} // namespace GenSockets

/** Provides Transmission Control Protocol communication with a GenIO interface.
    An instance of this class represents one connection between two computers.

    Inherently guarantees loss-less delivery of data in the order it is sent.
*/

class ELA_PACKETIO_EXPORT GenTCP_IO : public GenTCP_Connection, public GenNetIO
{
public:
    GenTCP_IO(int aSendBufferSize = 100000, int aRecvBufferSize = 100000);

    ~GenTCP_IO() override;

    int Receive(int aWaitTimeInMicroSec) override;

    virtual int ReceiveContinue(int aWaitTimeInMicroSec);

    virtual int ReceiveContinueN(int aWaitTimeInMicroSec, int aBytesToReceive);

    virtual void IgnoreBytes(int aNumberBytes);

    int Send() override;

    virtual int Send(int aWaitTimeInMicroSec);

    int GetUnsentBytes();

    void RemoveProcessedBufferData();

    unsigned long GetInputBufferSize();

    bool GetSenderId(unsigned int& aSrcInetAddr, unsigned short int& aSrcPort) const override
    {
        return GenTCP_Connection::GetSenderId(aSrcInetAddr, aSrcPort);
    }

    int GetLocalPort() const override { return GenTCP_Connection::GetLocalPort(); }

private:
    // prevent copying
    GenTCP_IO& operator=(const GenTCP_IO&);
    GenTCP_IO(const GenTCP_IO&);

protected:
    int mBufferBytesSent;
};
#endif
