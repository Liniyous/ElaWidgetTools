#ifndef PAKTCP_IO_H
#define PAKTCP_IO_H

#include "ElaPacketIO_Export.h"

#include "PakDefaultHeader.h"
#include "PakSocketIO.h"

class GenTCP_Connection;
class PakPacket;
class PakProcessor;
class PakO;
class PakI;

//! Sends packets via TCP.
//! A packet's size is only limited by the size of the GenIO receive buffer
class ELA_PACKETIO_EXPORT PakTCP_IO : public PakSocketIO
{
public:
    static const int cLARGE_WAIT_TIME = 100000000;
    PakTCP_IO(GenTCP_Connection* aConnectionPtr, PakProcessor* aProcessor, PakHeader* aHeaderType = new PakDefaultHeader);
    ~PakTCP_IO() override;

    bool Send(const PakPacket& aPkt) override;

    bool Send(const PakPacket& aPkt, int aWaitTimeMicroSeconds);

    bool Send(char* aBuffer, int aSize, int aPacketId, int aWaitTimeMicroSeconds = cLARGE_WAIT_TIME);

    bool ReceiveHeader(int& aPacketId, int& aPacketLength, int aWaitTimeMicroSeconds) override;

    bool Receive(PakPacket& aPkt) override;

    PakPacket* ReceiveNew();

    bool Receive(char* aBuffer, int aSize);

    bool IsConnected();

    void IgnorePacket() override;

    GenTCP_Connection& GetConnection() { return *mConnectionPtr; }

    void SetMaximumPacketSize(int aSize) { mMaximumPacketSize = aSize; }

    void BeginManualFlush() { ++mManualFlushCount; }

    void EndManualFlush()
    {
        --mManualFlushCount;
        if (mManualFlushCount <= 0)
        {
            mManualFlushCount = 0;
            Flush();
        }
    }

    bool Flush(int aWaitTimeInMicroSec = 100000000);

    void SetConnection(GenTCP_Connection* aConnectionPtr) { mConnectionPtr = aConnectionPtr; }

    PakProcessor* GetPakProcessor() const { return mPakProcessorPtr; }

    GenSockets::GenSocket* GetRecvSocket() const override;

    GenSockets::GenSocket* GetSendSocket() const override;

protected:
    bool PacketReadyToRead(int aWaitTimeMicroSeconds);

    void ReadTCP();
    bool ReadMoreTCP();
    bool ReadToBoundaryTCP();

    PakProcessor* mPakProcessorPtr;
    GenTCP_Connection* mConnectionPtr;
    GenBuffer mBufO;
    GenBuffer mBufI;
    PakO* mSerializeWriter;
    PakI* mSerializeReader;
    bool mIsHeaderRead;
    int mLastId;
    bool mHasReadHeader;
    bool mPacketReadyToRead;
    int mHeaderPacketId;
    int mHeaderPacketLength;

private:
    // prevent copying
    PakTCP_IO(const PakTCP_IO&);
    PakTCP_IO& operator=(const PakTCP_IO&);
    int mHeaderSize;
    int mManualFlushCount;
    int mSendBufferSize;
    int mMaximumPacketSize;
};
#endif
