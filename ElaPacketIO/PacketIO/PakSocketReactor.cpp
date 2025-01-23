#include "PakSocketReactor.h"

#include <iostream>
#include <map>

#include "GenSocket.h"
#include "GenSocketSelector.h"
#include "UtBinder.h"
#include "UtWallClock.h"
// Define this stuff here to avoid a bunch of includes in the header
class PakSocketReactorImpl
{
public:
    PakSocketReactorImpl() {}

    ~PakSocketReactorImpl()
    {
        delete mNotifyReceiver;
        delete mNotifySender;
    }
    GenSockets::GenSocketSelector mSocketSelector;
    GenSockets::GenSocketSet mSelectedSockets;

    GenSockets::GenSocket* mNotifyReceiver;
    GenSockets::GenSocket* mNotifySender;
};

PakSocketReactor::PakSocketReactor()
    : mIsRunning(false), mImpl(new PakSocketReactorImpl)
{
    GenSockets::GenSocket::CreateSocketPair(mImpl->mNotifyReceiver, mImpl->mNotifySender);
    mImpl->mSocketSelector.AddSocket(mImpl->mNotifyReceiver);
    mCallbacks[mImpl->mNotifyReceiver] = new UtCallbackN<void()>(UtStd::Bind(&PakSocketReactor::HandleNotify, this));
}

PakSocketReactor::~PakSocketReactor()
{
    for (CallbackMap::iterator i = mCallbacks.begin(); i != mCallbacks.end(); ++i)
    {
        delete i->second;
    }
    delete mImpl;
}

PakSocketReactor& PakSocketReactor::GetInstance()
{
    static PakSocketReactor i;
    return i;
}

void PakSocketReactor::FinishConnect(GenSockets::GenSocket* aSocket)
{
    mImpl->mSocketSelector.AddSocket(aSocket);
    if (mIsRunning)
    {
        Notify();
    }
}

//! Removes a socket read handler.
void PakSocketReactor::Disconnect(GenSockets::GenSocket* aSocket)
{
    mDeadSockets.push_back(aSocket);
    if (mIsRunning)
    {
        Notify();
    }
    else
    {
        CompleteDisconnects();
    }
}

void PakSocketReactor::CompleteDisconnects()
{
    for (size_t i = 0; i < mDeadSockets.size(); ++i)
    {
        mImpl->mSocketSelector.RemoveSocket(mDeadSockets[i]);
        CallbackMap::iterator iter = mCallbacks.find(mDeadSockets[i]);
        if (iter != mCallbacks.end())
        {
            delete iter->second;
            mCallbacks.erase(iter);
        }
    }
    mDeadSockets.clear();
}

//! This is the same as Run(), except it returns after aWaitTime
void PakSocketReactor::HandleEvents(double aWaitTime, int aEventType)
{
    if (aWaitTime > 0)
    {
        mIsStopping = false;
        const double CloseEnoughToZero = .001;
        double timeRemaining = aWaitTime;
        UtWallClock clockTimer;
        do
        {
            RunSelect(timeRemaining, aEventType);
            ProcessSignals();
            timeRemaining -= clockTimer.GetClock();
        } while (timeRemaining > CloseEnoughToZero && !mIsStopping);
    }
    else
    {
        RunSelect(0, aEventType);
        ProcessSignals();
    }
}

//! Waits for events, and processes them
//! Call Stop() from an external thread or event handler to stop
//! this call.
void PakSocketReactor::Run(int aEventType)
{
    mIsRunning = true;
    mIsStopping = false;
    while (!mIsStopping)
    {
        CompleteDisconnects();
        RunSelect(GenSockets::GenSocketSelector::cBLOCK_FOREVER, aEventType);
        ProcessSignals();
    }
    mIsRunning = false;
}

//! Stops event handling
void PakSocketReactor::Stop()
{
    if (mIsRunning)
    {
        mIsStopping = true;
        Notify();
    }
}

void PakSocketReactor::RunSelect(double aWaitTime, int aEventType)
{
    mImpl->mSelectedSockets.Clear();
    if (!mImpl->mSocketSelector.IsEmpty())
    {
        if (GenSockets::GenSocketSelector::cERROR == mImpl->mSocketSelector.Select(mImpl->mSelectedSockets, ((float)aWaitTime), aEventType))
        {
            if (!RemoveErrorSockets())
            {
                std::cout << "Unknown error on GenSocketSelector::Select()." << std::endl;
            }
        }
    }
}

//! Invokes any callbacks associated with the 'signalled' sockets.
void PakSocketReactor::ProcessSignals()
{
    unsigned int selectedSockets = mImpl->mSelectedSockets.GetSocketCount();
    for (unsigned int i = 0; i < selectedSockets; ++i)
    {
        GenSockets::GenSocket* selectedSocket = mImpl->mSelectedSockets.GetSocketEntry(i);
        (*mCallbacks[selectedSocket])();
    }
}

//! Wakes up the Select() call
void PakSocketReactor::Notify()
{
    char data = 0;
    // send 1 byte to wake up the select() call
    mImpl->mNotifySender->Send(&data, 1, 0);
}

//! Clear buffer if Notify() was called.
void PakSocketReactor::HandleNotify()
{
    int data = 0;
    mImpl->mNotifyReceiver->Receive((char*)&data, sizeof(int), 0);
}

bool PakSocketReactor::RemoveErrorSockets()
{
    bool removedSocket = false;
    unsigned int socketCount = mImpl->mSocketSelector.GetSocketCount();
    for (unsigned int i = 0; i < socketCount; ++i)
    {
        GenSockets::GenSocket* sockPtr = mImpl->mSocketSelector.GetSocketEntry(i);
        if (sockPtr->QuerySocketError() != GenSockets::GenSocket::cNO_ERROR)
        {
            mImpl->mSocketSelector.RemoveSocket(sockPtr);
            removedSocket = true;
            RemoveErrorSockets();
            break;
        }
    }
    return removedSocket;
}
