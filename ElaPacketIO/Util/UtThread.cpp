#include "UtThread.h"

UtThread::UtThread()
    : mThreadPtr(nullptr)
{
}

// virtual
UtThread::~UtThread()
{
    delete mThreadPtr;
}

bool UtThread::Start()
{
    if (mThreadPtr == nullptr)
    {
        // Only create a new thread if it has not already been done
        // We are not waiting for or terminating the existing thread
        // as that may lead to undesirable behavior.
        mException = nullptr;
        mThreadPtr = new std::thread(
            [this] {
                try
                {
                    Run();
                }
                catch (...)
                {
                    mException = std::current_exception();
                }
            });
    }
    return (mThreadPtr != nullptr);
}

bool UtThread::Join()
{
    if (mThreadPtr->joinable())
    {
        mThreadPtr->join();
    }
    return (!mThreadPtr->joinable());
}

bool UtThread::JoinAll(UtThread::UtThreads& aThreads)
{
    bool ok(true); // default to true
    size_t threadCount(aThreads.size());
    if (threadCount > 0)
    {
        for (size_t i = 0; i < (MAX_THREADS + 1); ++i)
        {
            if (i < threadCount)
            {
                if (aThreads[i]->GetThreadData().joinable())
                {
                    aThreads[i]->Join();
                }
                ok &= (!aThreads[i]->GetThreadData().joinable());
            }
        }
    }
    return ok;
}

unsigned int UtThread::GetCoreCount()
{
    return std::thread::hardware_concurrency();
}

// ----------------------------------------------------------------------------
//                              Unit test case
//
// Unix: CC (or g++) -DUNIT_TEST UtThread.cpp -lpthread
// Windows: cl -GX -DUNIT_TEST UtThread.cpp
// ----------------------------------------------------------------------------

#ifdef UNIT_TEST

#include <iostream>
#include <mutex>

#include "UtThread.hpp"

std::recursive_mutex mutex;
int sharedCount = 0;

void ThreadFunc()
{
    for (unsigned int i = 0; i < 500000; ++i)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);
        ++sharedCount;
    }
}

class ThreadClass : public UtThread
{
public:
    void Run()
    {
        // Just call ThreadFunc for simplicity.  This really could be unique code.
        ThreadFunc();
    }
};

int main(int argc, char* argv[])
{
    ThreadClass t1;
    ThreadClass t2;
    UtThreadAdapter t3(ThreadFunc);
    t1.Start();
    t2.Start();
    t3.Start();
    t1.Join();
    t2.Join();
    t3.Join();

    auto out = ut::log::info() << "UtThread Count";
    out.AddNote() << "Count:" << sharedCount;
    return 0;
}

#endif
