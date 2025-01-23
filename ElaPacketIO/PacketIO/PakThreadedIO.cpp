#include "PakThreadedIO.h"

#include <iostream>

#include "GenInternetSocketAddress.h"
#include "GenTCP_IO.h"
#include "GenUDP_IO.h"
#include "PakProcessor.h"
#include "PakTCP_IO.h"
#include "PakUDP_IO.h"

// Split into reactor thread, and send thread...

PakThreadedIO::PakThreadedIO()
    : mStopping(false), mHandlerAccess(1)
{
}

PakThreadedIO::~PakThreadedIO()
{
    for (size_t i = 0; i < mHandlers.size(); ++i)
    {
        Handler* handlerPtr = mHandlers[i];
        delete handlerPtr;
    }
}

//! Precondition: PakTCP_IO is connected
//! Handles receiving and sending for a PakTCP_IO.  PakThreadedIO gains control
//! of the IO class, and send or receive methods should only be called through this class.
//! @param aIOPtr The PakTCP_IO to handle.
//! @param aConnectionPtr Sets incoming packets' getSender() value to this connection
void PakThreadedIO::AddIO(PakSocketIO* aIOPtr, PakConnection* aConnectionPtr /*= 0*/)
{
    Pause();
    Handler* handler = new Handler(this, aIOPtr, aConnectionPtr);
    mReactor.Connect(aIOPtr->GetRecvSocket(), &Handler::Handle, handler);
    mHandlers.push_back(handler);
    Resume();
}

//! Stops handling sending/receiving for a PakTCP_IO.
//! @param aIOPtr The PakTCP_IO to stop handling.
void PakThreadedIO::RemoveIO(PakSocketIO* aIOPtr)
{
    Pause();
    RemoveIO_P(aIOPtr, true); // Don't notify the user
    ProcessRemovedHandlers();
    Resume();
}

//! Pauses the reactor thread
void PakThreadedIO::Pause()
{
    std::lock_guard<std::recursive_mutex> lock(mReactorLock);
    mReactor.Stop();
    mHandlerAccess.Acquire();
}

void PakThreadedIO::Resume()
{
    mHandlerAccess.Release();
}

void PakThreadedIO::RemoveIO_P(PakSocketIO* aIOPtr, bool aNotifyUser)
{
    for (size_t i = 0; i < mHandlers.size(); ++i)
    {
        PakSocketIO* io = mHandlers[i]->GetIO();
        if (io == aIOPtr)
        {
            mReactor.Disconnect(io->GetRecvSocket());
            Handler* handlerPtr = mHandlers[i];
            mHandlers.erase(mHandlers.begin() + i);
            if (aNotifyUser)
            {
                mRemovedHandlers.push_back(handlerPtr);
            }
            else
            {
                delete handlerPtr;
            }
            break;
        }
    }
}

//! Sends a message using a PakTCP_IO.  PakThreadedIO will handle the deletion
//! of the packet.  Currently just sends in a blocking mode.
void PakThreadedIO::Send(PakSocketIO* aIOPtr, PakPacket& aPacket)
{
    aIOPtr->Send(aPacket);
}

//! Sends a message to multiple recipients.  PakThreadedIO will handle the deletion
//! of the packet.  Currently just sends in a blocking mode.
void PakThreadedIO::Send(const std::vector<PakSocketIO*>& aIO_List, PakPacket& aPacket)
{
    for (size_t i = 0; i < aIO_List.size(); ++i)
    {
        aIO_List[i]->Send(aPacket);
    }
}

//! Sends a message to all connections handled by PakThreadedIO
//! Currently just sends in a blocking mode
void PakThreadedIO::SendToAll(PakPacket& aPacket)
{
    for (size_t i = 0; i < mHandlers.size(); ++i)
    {
        mHandlers[i]->GetIO()->Send(aPacket);
    }
}

//! Invokes callbacks associated with received packets
void PakThreadedIO::Process()
{
    for (size_t i = 0; i < mHandlers.size(); ++i)
    {
        mHandlers[i]->ProcessPackets();
    }
    if (!mRemovedHandlers.empty())
    {
        Pause();
        ProcessRemovedHandlers();
        Resume();
    }
}

//! Extract a list of received packets
void PakThreadedIO::Extract(PacketList& aPacketList)
{
    for (size_t i = 0; i < mHandlers.size(); ++i)
    {
        PacketList packets;
        mHandlers[i]->ExtractPackets(packets);
        aPacketList.insert(aPacketList.end(), packets.begin(), packets.end());
    }
    if (!mRemovedHandlers.empty())
    {
        Pause();
        ProcessRemovedHandlers();
        Resume();
    }
}

//! Precondition: The reactor is paused.
void PakThreadedIO::ProcessRemovedHandlers()
{
    for (size_t i = 0; i < mRemovedHandlers.size(); ++i)
    {
        Handler* handlerPtr = mRemovedHandlers[i];
        Disconnected(handlerPtr->GetIO(), handlerPtr->GetConnection());
        delete handlerPtr;
    }
    mRemovedHandlers.clear();
}

//! Signals the thread to complete.  Join() should return quickly after Stop() is called.
void PakThreadedIO::Stop()
{
    Pause();
    mStopping = true;
    Resume();
}

// virtual
void PakThreadedIO::Run()
{
    while (!mStopping)
    {
        mReactorLock.lock();
        mHandlerAccess.Acquire();
        mReactorLock.unlock();
        if (mStopping)
        {
            break;
        }

        mReactor.Run(PakSocketReactor::cREAD | PakSocketReactor::cEXCEPTION);

        for (size_t i = 0; i < mDeadHandlers.size(); ++i)
        {
            RemoveIO_P(mDeadHandlers[i]->GetIO(), true);
        }
        mDeadHandlers.clear();
        mHandlerAccess.Release();
    }
    mStopping = false;
}

PakThreadedIO::Handler::Handler(PakThreadedIO* aParentPtr, PakSocketIO* aIOPtr, PakConnection* aConnectionPtr)
    : mConnectionPtr(aConnectionPtr), mParentPtr(aParentPtr), mIOPtr(aIOPtr), mQueueAccess(1)
{
    mIsTCP = (dynamic_cast<PakTCP_IO*>(mIOPtr) != nullptr);
}

void PakThreadedIO::Handler::Handle()
{
    PacketList packets;
    PakPacket* pktPtr;
    if (mIsTCP)
    {
        PakTCP_IO* tcpIO = (PakTCP_IO*)mIOPtr;
        while ((pktPtr = tcpIO->ReceiveNew()) != nullptr)
        {
            pktPtr->SetSender(mConnectionPtr);
            packets.push_back(pktPtr);
        }
    }
    else
    {
        PakUDP_IO* udpIO = (PakUDP_IO*)mIOPtr;
        while ((pktPtr = udpIO->ReceiveNew()) != nullptr)
        {
            pktPtr->SetSender(mConnectionPtr);
            const GenSockets::GenInternetSocketAddress& socketAddr = udpIO->GetConnection().GetLastSenderAddress();
            GenSockets::GenIP ip = socketAddr.GetAddress();
            pktPtr->SetOriginatorAddress(ip.GetAddress());
            pktPtr->SetOriginatorPort(socketAddr.GetPort());
            packets.push_back(pktPtr);
        }
    }
    if (!packets.empty())
    {
        mQueueAccess.Acquire();
        mReceiveQueue.insert(mReceiveQueue.end(), packets.begin(), packets.end());
        mQueueAccess.Release();
    }
    if (mIsTCP)
    {
        if (!((PakTCP_IO*)mIOPtr)->IsConnected())
        {
            mParentPtr->mDeadHandlers.push_back(this);
            mParentPtr->mReactor.Stop();
        }
    }
}

PakThreadedIO::Handler::~Handler()
{
    for (size_t i = 0; i < mReceiveQueue.size(); ++i)
    {
        delete mReceiveQueue[i];
    }
}

void PakThreadedIO::Handler::ProcessPackets()
{
    if (mReceiveQueue.empty())
    {
        return;
    }
    PacketList packets;
    mQueueAccess.Acquire();
    packets.swap(mReceiveQueue);
    mQueueAccess.Release();

    if (mIsTCP)
    {
        for (size_t i = 0; i < packets.size(); ++i)
        {
            ((PakTCP_IO*)mIOPtr)->GetPakProcessor()->ProcessPacket(packets[i], true);
        }
    }
    else
    {
        for (size_t i = 0; i < packets.size(); ++i)
        {
            ((PakUDP_IO*)mIOPtr)->GetPakProcessor()->ProcessPacket(packets[i], true);
        }
    }
}

void PakThreadedIO::Handler::ExtractPackets(PacketList& aPackets)
{
    if (mReceiveQueue.empty())
    {
        return;
    }
    mQueueAccess.Acquire();
    mReceiveQueue.swap(aPackets);
    mQueueAccess.Release();
}
