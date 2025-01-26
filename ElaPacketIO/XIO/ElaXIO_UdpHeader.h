#ifndef ELAXIO_UDPHEADER_H
#define ELAXIO_UDPHEADER_H

#include "ElaPacketIO_Export.h"

#include "PakHeader.h"
#include "PakIntTypes.h"

class ELA_PACKETIO_EXPORT ElaXIO_UdpHeader : public PakHeader
{
public:
    explicit ElaXIO_UdpHeader(int aVersionID);
    ElaXIO_UdpHeader(const ElaXIO_UdpHeader& aSrc);
    ~ElaXIO_UdpHeader() override = default;

    PakHeader* Clone() const override;

    void WriteHeader(GenBuffer& aIO, int aPacketID, int aPacketLength) override;

    bool ReadHeader(GenBuffer& aIO, int& aPacketID, int& aPacketLength) override;

    int GetHeaderSize() override;

protected:
    struct header {
        uint32_t mXIO_Version;
        int32_t mLength;
        int32_t mID;
    };
    uint32_t mXIO_Version;
    bool mReceivedInvalidPacket;
};

#endif
