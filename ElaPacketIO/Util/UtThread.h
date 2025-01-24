#ifndef UTTHREAD_H
#define UTTHREAD_H

#define MAX_THREADS 64

#include "ElaPacketIO_Export.h"

#include <exception>
#include <thread>
#include <vector>

class ELA_PACKETIO_EXPORT UtThread
{
public:
    using UtThreads = std::vector<UtThread*>;

    virtual ~UtThread();

    virtual void Run() = 0;
    bool Start();
    bool Join();
    static bool JoinAll(UtThreads& aThreads);
    const std::thread& GetThreadData() const { return *mThreadPtr; };
    std::exception_ptr GetException() const { return mException; }
    static unsigned int GetCoreCount();

protected:
    UtThread();
    UtThread(const UtThread& aSrc) = delete;
    UtThread& operator=(const UtThread& aRhs) = delete;

    std::exception_ptr mException;

private:
    std::thread* mThreadPtr;
};

class UtThreadAdapter : public UtThread
{
public:
    UtThreadAdapter(void (*aFunctionPtr)())
        : mFunctionPtr(aFunctionPtr)
    {
    }

    void Run() override { (*mFunctionPtr)(); }

private:
    void (*mFunctionPtr)();
};

#endif
