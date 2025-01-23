#ifndef PakUDP_IO_H
#define PakUDP_IO_H

#include "ElaPacketIO_Export.h"

#include "PakDefaultHeader.h"
#include "PakSocketIO.h"
class GenUDP_Connection;
class PakPacket;
class PakProcessor;
class PakO;
class PakI;
//! Sends and receives packets via UDP.
//! Each PakPacket must fit inside a UDP packet.
class ELA_PACKETIO_EXPORT PakUDP_IO : public PakSocketIO
{
public:
    PakUDP_IO(GenUDP_Connection* aConnection, PakProcessor* aProcessorPtr, PakHeader* aHeaderType = new PakDefaultHeader);
    ~PakUDP_IO() override;

    bool Send(const PakPacket& aPkt) override;

    bool ReceiveHeader(int& aPacketId, int& aPacketLength, int aWaitTimeMicroSeconds) override;

    bool Receive(PakPacket& aPkt) override;

    void IgnorePacket() override;

    GenUDP_Connection& GetConnection() { return *mConnectionPtr; }

    GenSockets::GenSocket* GetRecvSocket() const override;

    GenSockets::GenSocket* GetSendSocket() const override;

    PakPacket* ReceiveNew();

    PakProcessor* GetPakProcessor() const { return mProcessorPtr; }

protected:
    void ReadUDP();
    bool ReadMoreUDP();
    bool ReadToBoundaryUDP();
    GenUDP_Connection* mConnectionPtr;
    PakProcessor* mProcessorPtr;
    GenBuffer mBufO;
    GenBuffer mBufI;
    PakO* mSerializeWriter;
    PakI* mSerializeReader;
    bool mIsHeaderRead;
    int mLastId;
    bool mHasReadHeader;
    int mHeaderPacketId;
    int mHeaderPacketLength;
    char* mEmptyJunk;
    int mHeaderSize;

private:
    void operator=(const PakUDP_IO&); // Not allowed
};

#endif
