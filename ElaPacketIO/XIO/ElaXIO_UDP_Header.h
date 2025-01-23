#ifndef ELAXIO_UDP_HEADER_H
#define ELAXIO_UDP_HEADER_H

#include "ElaPacketIO_Export.h"

#include "PakHeader.h"
#include "PakIntTypes.h"

class ELA_PACKETIO_EXPORT ElaXIO_UDP_Header : public PakHeader
{
public:
    explicit ElaXIO_UDP_Header(int aVersionID);
    ElaXIO_UDP_Header(const ElaXIO_UDP_Header& aSrc);
    ~ElaXIO_UDP_Header() override = default;

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
