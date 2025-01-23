#include "UtCallback.h"

void UtCallbackLink::Merge(UtCallbackList* aOtherCallbackList)
{
    mCallbackListPtr = aOtherCallbackList;
}

void UtCallbackLink::Disconnect(UtCallback* aCallbackPtr)
{
    mCallbackListPtr->Disconnect(aCallbackPtr);
}

void UtCallback::Disconnect()
{
    if (mCallbackLinkPtr != nullptr)
    {
        mCallbackLinkPtr->Disconnect(this);
        mCallbackLinkPtr = nullptr;
    }
}

void UtCallback::Block(bool aBlock /*= true*/)
{
    if (mIsBlocked != aBlock && mCallbackLinkPtr != nullptr)
    {
        mCallbackLinkPtr->Block(this, aBlock);
        mIsBlocked = aBlock;
    }
}

UtCallback::~UtCallback()
{
    Disconnect();
}

void UtCallbackList::MergeP(UtCallbackList& aOtherCallbackList)
{
    ListType& otherCbList = aOtherCallbackList.mCallbackList;
    ListType& otherBlockedCbList = aOtherCallbackList.mBlockedCallbackList;

    for (IterType i = otherCbList.begin(); i != otherCbList.end(); ++i)
    {
        UtCallback* callback = *i;
        callback->Merge(this);
    }

    for (IterType i = otherBlockedCbList.begin(); i != otherBlockedCbList.end(); ++i)
    {
        UtCallback* callback = *i;
        callback->Merge(this);
    }

    mCallbackList.splice(mCallbackList.end(), otherCbList);
    mBlockedCallbackList.splice(mBlockedCallbackList.end(), otherBlockedCbList);
}

void UtCallbackList::ConnectP(UtCallback* aCallbackPtr)
{
    // If the callback is currently connected to a list then disconnect it from that list.
    aCallbackPtr->Disconnect();

    // Connect the callback if not already in the list.
    if (std::find(mCallbackList.begin(), mCallbackList.end(), aCallbackPtr) == mCallbackList.end())
    {
        aCallbackPtr->mCallbackLinkPtr = new CallbackLink(this);
        if (aCallbackPtr->IsBlocked())
        {
            mBlockedCallbackList.push_back(aCallbackPtr);
        }
        else
        {
            mCallbackList.push_back(aCallbackPtr);
        }
    }
}

void UtCallbackList::DisconnectAll()
{
    // Tell each connected subscriber that they have been disconnected.
    {
        for (IterType it = mCallbackList.begin(); it != mCallbackList.end(); ++it)
        {
            delete (*it)->mCallbackLinkPtr;
            (*it)->mCallbackLinkPtr = nullptr;
        }
    }
    mCallbackList.clear();
    {
        for (IterType it = mBlockedCallbackList.begin(); it != mBlockedCallbackList.end(); ++it)
        {
            delete (*it)->mCallbackLinkPtr;
            (*it)->mCallbackLinkPtr = nullptr;
        }
    }
    mBlockedCallbackList.clear();
}

void UtCallbackList::Disconnect(UtCallback* aCallbackPtr)
{
    bool found = true;
    IterType it = std::find(mCallbackList.begin(), mCallbackList.end(), aCallbackPtr);
    if (it == mCallbackList.end())
    {
        it = std::find(mBlockedCallbackList.begin(), mBlockedCallbackList.end(), aCallbackPtr);
        if (it == mBlockedCallbackList.end())
        {
            found = false;
        }
        else
        {
            mBlockedCallbackList.erase(it);
        }
    }
    else
    {
        mCallbackList.erase(it);
    }
    if (found)
    {
        // the subscriber it is no longer connected.
        delete aCallbackPtr->mCallbackLinkPtr;
        aCallbackPtr->mCallbackLinkPtr = nullptr;
    }
}

UtCallbackList::~UtCallbackList()
{
    DisconnectAll();
}

UtCallbackList::UtCallbackList(const UtCallbackList& /*aSrc*/)
    : mCallbackList(), mBlockedCallbackList()
{
}

UtCallbackList::UtCallbackList()
    : mCallbackList(), mBlockedCallbackList()
{
}

void UtCallbackLink::Block(UtCallback* aCallbackPtr, bool aBlock)
{
    if (aBlock)
    {
        mCallbackListPtr->mCallbackList.remove(aCallbackPtr);
        mCallbackListPtr->mBlockedCallbackList.push_back(aCallbackPtr);
    }
    else
    {
        mCallbackListPtr->mBlockedCallbackList.remove(aCallbackPtr);
        mCallbackListPtr->mCallbackList.push_back(aCallbackPtr);
    }
}
