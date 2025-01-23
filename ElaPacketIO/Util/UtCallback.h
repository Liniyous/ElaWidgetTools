#ifndef UTCALLBACK_H
#define UTCALLBACK_H

#include "ElaPacketIO_Export.h"

#include <algorithm>
#include <functional>
#include <list>

#include "UtBinder.h"
class UtCallback;
class UtCallbackList;

class ELA_PACKETIO_EXPORT UtCallbackLink
{
public:
    friend class UtCallback;

    UtCallbackLink(UtCallbackList* aCallbackListPtr)
        : mCallbackListPtr(aCallbackListPtr)
    {
    }
    ~UtCallbackLink() = default; // To avoid warnings about virtual methods without virtual destructor
    void Disconnect(UtCallback* aCallbackPtr);
    void Block(UtCallback* aCallbackPtr, bool aBlock);

protected:
    void Merge(UtCallbackList* aOtherCallbackList);
    UtCallbackList* mCallbackListPtr;
};

class UtCallback
{
public:
    friend class UtCallbackList;

    ELA_PACKETIO_EXPORT UtCallback()
        : mCallbackLinkPtr(nullptr), mIsBlocked(false)
    {
    }
    ELA_PACKETIO_EXPORT UtCallback(const UtCallback& /*aSrc*/)
        : mCallbackLinkPtr(nullptr), mIsBlocked(false)
    {
    }
    ELA_PACKETIO_EXPORT virtual ~UtCallback();
    ELA_PACKETIO_EXPORT void Disconnect();
    ELA_PACKETIO_EXPORT void Block(bool aBlock = true);

    //! Unblocks the callback.  See Block().
    void Unblock() { Block(false); }

    //! Returns true if this callback is currently blocked.
    bool IsBlocked() const { return mIsBlocked; }

private:
    void Merge(UtCallbackList* aOtherCallbackList) { mCallbackLinkPtr->Merge(aOtherCallbackList); }

    UtCallback& operator=(const UtCallback&) = delete;

    UtCallbackLink* mCallbackLinkPtr;
    bool mIsBlocked;
};

class ELA_PACKETIO_EXPORT UtCallbackList
{
public:
    friend class UtCallback;
    friend class UtCallbackLink;
    using CallbackLink = UtCallbackLink;

    using cUT_SERIALIZE_IGNORE = bool;

    using ListType = std::list<UtCallback*>;
    using IterType = std::list<UtCallback*>::iterator;

    explicit UtCallbackList();
    explicit UtCallbackList(const UtCallbackList& /*aSrc*/);
    virtual ~UtCallbackList();

    void Disconnect(UtCallback* aCallbackPtr);
    void DisconnectAll();

    bool IsEmpty() const { return mCallbackList.empty(); }

protected:
    void ConnectP(UtCallback* aCallbackPtr);

    void MergeP(UtCallbackList& aOtherCallbackList);

    ListType mCallbackList;

private:
    ListType mBlockedCallbackList;
    UtCallbackList& operator=(const UtCallbackList&) = delete;
};

#include "UtCallbackN.h"

#endif
