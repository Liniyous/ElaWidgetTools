#ifndef PAKSOCKETREACTOR_H
#define PAKSOCKETREACTOR_H

#include "ElaPacketIO_Export.h"

#include <map>
#include <vector>

#include "UtBinder.h"
#include "UtCallback.h"

namespace GenSockets
{
class GenSocket;
}

class PakSocketReactorImpl;

/** PakSocketReactor allows linking callbacks to sockets.  The callbacks
 are invoked when the sockets have data ready to read.
*/
class ELA_PACKETIO_EXPORT PakSocketReactor
{
public:
    PakSocketReactor();
    ~PakSocketReactor();

    typedef UtCallbackN<void()> CallbackType;

    //! Adds a socket read handler.  When the socket is able to read,
    //! the supplied callback will be invoked.  This may be called
    //! while Run() or HandleEvents() is executing.
    void Connect(GenSockets::GenSocket* aSocket, const std::function<void()>& aFunc)
    {
        mCallbacks[aSocket] = new UtCallbackN<void()>(aFunc);
        FinishConnect(aSocket);
    }

    //! Adds a socket read handler.  When the socket is able to read,
    //! the supplied callback will be invoked.  This may be called
    //! while Run() or HandleEvents() is executing.
    template <typename C>
    void Connect(GenSockets::GenSocket* aSocket, void (C::*aFuncPtr)(), C* aThisPtr)
    {
        Connect(aSocket, UtStd::Bind(aFuncPtr, aThisPtr));
    }

    void Disconnect(GenSockets::GenSocket* aSocket);

    enum EventType
    {
        cREAD = 1,
        cWRITE = 2,
        cEXCEPTION = 4,
        cALL = cREAD | cWRITE | cEXCEPTION
    };

    void HandleEvents(double aWaitTime = 0.0, int aEventType = cREAD);

    void Run(int aEventType = cREAD);

    void Stop();

    //! Allows this to be used as a singleton.  Returns the singleton instance.
    //! @note PakSocketRector may also be instanced.
    static PakSocketReactor& GetInstance();

private:
    void FinishConnect(GenSockets::GenSocket* aSocket);

    void RunSelect(double aWaitTime, int aEventType);

    void CompleteDisconnects();

    void Notify();

    void ProcessSignals();

    void HandleNotify();

    bool RemoveErrorSockets();

    typedef std::vector<GenSockets::GenSocket*> SocketList;
    typedef std::map<GenSockets::GenSocket*, UtCallbackN<void()>*> CallbackMap;

    volatile bool mIsRunning;
    volatile bool mIsStopping;
    PakSocketReactorImpl* mImpl;
    SocketList mDeadSockets;
    CallbackMap mCallbacks;
};

#endif
