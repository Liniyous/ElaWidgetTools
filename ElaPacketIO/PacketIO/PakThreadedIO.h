#ifndef PAKTHREADEDIO_H
#define PAKTHREADEDIO_H

#include "ElaPacketIO_Export.h"

#include <mutex>
#include <vector>

#include "PakPacket.h"
#include "PakSocketReactor.h"
#include "UtCallback.h"
#include "UtSemaphore.h"
#include "UtThread.h"
class PakTCP_IO;
class PakUDP_IO;
class PakSocketIO;

//! Handles receiving packets on 1 or more sockets.
//! Being a UtThread, Start() must be called to initiate the thread.
//! Stop() must be called prior to Join() to signal the thread to quit.
//!
//!@note Currently, the send() methods are pass-through to a blocking send call.
//!      In the future, this may be changed to do threaded sends.
class ELA_PACKETIO_EXPORT PakThreadedIO : public UtThread
{
public:
    typedef std::vector<PakPacket*> PacketList;

    PakThreadedIO();

    ~PakThreadedIO() override;

    void AddIO(PakSocketIO* aIOPtr, PakConnection* aConnectionPtr = nullptr);

    void RemoveIO(PakSocketIO* aIOPtr);

    void Send(PakSocketIO* aIOPtr, PakPacket& aPacket);

    void Send(const std::vector<PakSocketIO*>& aIO_List, PakPacket& aPacket);

    void SendToAll(PakPacket& aPacket);

    //! This Callback list is invoked during Process() after a connection is broken.
    UtCallbackListN<void(PakSocketIO*, PakConnection*)> Disconnected;

    void Process();

    void Extract(PacketList& aPacketList);

    void Stop();

    void Run() override;

private:
    void ProcessRemovedHandlers();

    void RemoveIO_P(PakSocketIO* aIOPtr, bool aNotifyUser);

    class Handler
    {
    public:
        typedef std::vector<PakPacket*> PacketList;
        Handler(PakThreadedIO* aParentPtr, PakSocketIO* aIOPtr, PakConnection* aConnectionPtr);
        void Handle();
        ~Handler();
        void ProcessPackets();
        void ExtractPackets(PacketList& aPackets);
        PakSocketIO* GetIO() const { return mIOPtr; }
        PakConnection* GetConnection() const { return mConnectionPtr; }

    private:
        PakConnection* mConnectionPtr;
        PakThreadedIO* mParentPtr;
        PakSocketIO* mIOPtr;
        bool mIsTCP;
        UtSemaphore mQueueAccess;
        PacketList mReceiveQueue;
    };

public:
    friend class PakThreadedIO::Handler;

private:
    void Pause();
    void Resume();
    typedef std::vector<Handler*> HandlerList;
    PakSocketReactor mReactor;

    volatile bool mStopping;
    UtSemaphore mHandlerAccess;
    std::recursive_mutex mReactorLock;
    //! List of handlers that are removed, and need to notify user
    HandlerList mRemovedHandlers;
    //! List of handlers that are waiting to be removed from reactor
    HandlerList mDeadHandlers;
    //! List of active handlers
    HandlerList mHandlers;
};

#endif
