#ifndef UTIMMUTABLELIST_H
#define UTIMMUTABLELIST_H

#include <cassert>
#include <mutex>
#include <vector>

template <typename T>
class UtImmutableListNode
{
public:
    UtImmutableListNode(const T& aValue, UtImmutableListNode<T>* aNextPtr = 0)
        : mCountMutex(), mRefCount(1), mNextPtr(aNextPtr), mValue(aValue)
    {
        if (mNextPtr != 0)
        {
            mNextPtr->AddRef();
        }
    }

    ~UtImmutableListNode()
    {
        if (mNextPtr != 0)
        {
            mNextPtr->Unref();
        }
    }

    UtImmutableListNode<T>* next() { return mNextPtr; }

    void AddRef()
    {
        std::lock_guard<std::recursive_mutex> lock(mCountMutex);
        ++mRefCount;
    }

    void Unref()
    {
        mCountMutex.lock();
        --mRefCount;
        bool doDelete = (mRefCount == 0);
        mCountMutex.unlock();
        if (doDelete)
        {
            delete this;
        }
    }
    const T& Value() const { return mValue; }

private:
    std::recursive_mutex mCountMutex;
    int mRefCount;
    UtImmutableListNode<T>* mNextPtr;
    T mValue;
};

//! A singly-linked immutable list with built-in reference counting
//! This allows multiple lists to share the same tail.
//! copy constructor, empty(), cons(), next(), and front() -- Constant time
//! size(), toVector() -- linear time
//!
//! To inspect elements, either use front() & next() or toVector()
template <typename T>
class UtImmutableList
{
private:
    typedef UtImmutableListNode<T> Node;

    explicit UtImmutableList(Node* aNodePtr)
        : mNodePtr(aNodePtr)
    {
        if (mNodePtr != 0)
        {
            mNodePtr->AddRef();
        }
    }

public:
    UtImmutableList()
        : mNodePtr(0)
    {
    }

    UtImmutableList(const UtImmutableList& aSrc)
        : mNodePtr(aSrc.mNodePtr)
    {
        if (mNodePtr != 0)
        {
            mNodePtr->AddRef();
        }
    }

    UtImmutableList(const T& aValue, Node* aNextPtr = 0) { mNodePtr = new Node(aValue, aNextPtr); }

    ~UtImmutableList()
    {
        if (mNodePtr != 0)
        {
            mNodePtr->Unref();
        }
    }

    UtImmutableList<T>& operator=(const UtImmutableList<T>& aRhs)
    {
        if (mNodePtr != aRhs.mNodePtr)
        {
            if (mNodePtr != 0)
            {
                mNodePtr->Unref();
            }
            mNodePtr = aRhs.mNodePtr;
            if (aRhs.mNodePtr != 0)
            {
                mNodePtr->AddRef();
            }
        }
        return *this;
    }

    int size() const
    {
        int count = 0;
        Node* nodePtr = mNodePtr;
        while (nodePtr != 0)
        {
            ++count;
            nodePtr = nodePtr->next();
        }
        return count;
    }

    bool empty() const { return mNodePtr == 0; }

    const T& front() const // car
    {
        assert(!empty());
        return mNodePtr->Value();
    }

    const UtImmutableList<T> next() const // cdr
    {
        assert(!empty());
        return UtImmutableList(mNodePtr->next());
    }

    UtImmutableList<T> join(const T& aValue) const // cons
    {
        return UtImmutableList(aValue, mNodePtr);
    }

    std::vector<T> toVector() const
    {
        std::vector<T> values;
        Node* nodePtr = mNodePtr;
        while (nodePtr != 0)
        {
            values.push_back(nodePtr->Value());
            nodePtr = nodePtr->next();
        }
        return values;
    }

    static UtImmutableList<T> fromVector(const std::vector<T>& aValues)
    {
        if (aValues.empty())
            return UtImmutableList<T>();
        Node* n = 0;
        for (int i = -1 + (int)aValues.size(); i >= 0; --i)
        {
            Node* prev = n;
            n = new Node(aValues[i], n);
            if (prev)
            {
                prev->Unref();
            }
        }
        UtImmutableList rVal(n);
        n->Unref();
        return rVal;
    }

private:
    Node* _private_GetNode() const { return mNodePtr; }

    Node* mNodePtr;
};

#endif
