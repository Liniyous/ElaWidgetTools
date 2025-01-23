#ifndef GENNETIO_H
#define GENNETIO_H

#include "ElaPacketIO_Export.h"

#include <string>

#include "GenIO.h"
class GenBufOManaged;
class GenBufIManaged;
class GenUDP_IO;

/** The base class for Socket based communication
 */
class ELA_PACKETIO_EXPORT GenNetIO : public GenIO
{
public:
    GenNetIO(const char* aRecvBufName, const char* aSendBufName);

    static GenNetIO* Create(const std::string& aForeignHost, int aForeignPort, int aLocalPort, int aOptions);

    static GenNetIO* Create(const std::string& aForeignHost, int aForeignPort, int aOptions);

    static GenNetIO* Create(int aLocalPort, int aOptions);

    enum Options
    {
        cOPT_TCP = 1,
        cOPT_UDP = 2
    };

    GenBufIManaged* GetRecvBuffer() const { return mRecvBuf; }
    GenBufOManaged* GetSendBuffer() const { return mSendBuf; }

    //! On Dedicated Connections: Returns information about the connected address
    //! On non-dedicated connections: Returns information about the sender of the last received packet.
    //! @param aSrcInetAddr The IP packed into an integer
    //! @param aSrcPort The port incoming messages are received through.
    //! @return 'true' if the sender information was retrieved successfully
    virtual bool GetSenderId(unsigned int& aSrcInetAddr, unsigned short int& aSrcPort) const = 0;

    //! Returns the port this NetIO is assigned to receive on.
    //! @return A positive value:     The receiving port,
    //!         A non-positive value: The socket is not bound to a port.
    virtual int GetLocalPort() const = 0;

protected:
    GenBufIManaged* mRecvBuf;
    GenBufOManaged* mSendBuf;

private:
    // prevent copying
    GenNetIO& operator=(const GenNetIO&);
    GenNetIO(const GenNetIO&);

    static GenUDP_IO* CreateUDP(const std::string& aForeignHost, int aForeignPort, int aLocalPort = -1);
};

#endif
