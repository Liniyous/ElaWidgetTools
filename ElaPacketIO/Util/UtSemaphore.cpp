#include "UtSemaphore.h"

#include <cassert>

// Unix implementation
#if defined(__unix__) || defined(__APPLE__)
#include <semaphore.h>

class UtSemaphoreImpl
{
public:
    void Create(int aInitialCount) { sem_init(&mSemHandle, 0, aInitialCount); }

    void Destroy() { sem_destroy(&mSemHandle); }

    void Release(unsigned int aReleaseCount)
    {
        for (unsigned int i = 0; i < aReleaseCount; ++i)
        {
            sem_post(&mSemHandle);
        }
    }

    void Acquire() { sem_wait(&mSemHandle); }

    bool TryAcquire() { return 0 == sem_trywait(&mSemHandle); }

    sem_t mSemHandle;
};
#endif

// Windows Implementation
#if defined(_WIN32)
#include <windows.h>

class UtSemaphoreImpl
{
public:
    // 'enum' is used instead of 'static const int' for compatibility with VC6.
    enum
    {
        cMAX_SEMAPHORE_COUNT = 1000 // 1000 is an arbitrary value
    };

    void Create(int aInitialCount)
    {
        mSemHandle = CreateSemaphore(nullptr, aInitialCount, cMAX_SEMAPHORE_COUNT, nullptr);
    }

    void Destroy() { CloseHandle(mSemHandle); }

    void Release(unsigned int aReleaseCount) { ReleaseSemaphore(mSemHandle, static_cast<long>(aReleaseCount), nullptr); }

    void Acquire()
    {
        unsigned int result = WaitForSingleObject(mSemHandle, INFINITE);
        assert(result == WAIT_OBJECT_0);
    }

    bool TryAcquire()
    {
        unsigned int result = WaitForSingleObject(mSemHandle, 0);
        return result == WAIT_OBJECT_0;
    }

    HANDLE mSemHandle;
};

#endif

UtSemaphore::UtSemaphore(int aInitialCount)
{
    mImplPtr = new UtSemaphoreImpl();
    mImplPtr->Create(aInitialCount);
}

UtSemaphore::~UtSemaphore()
{
    mImplPtr->Destroy();
    delete mImplPtr;
}

void UtSemaphore::Release(unsigned int aCount /* =1 */)
{
    mImplPtr->Release(aCount);
}

void UtSemaphore::Acquire()
{
    mImplPtr->Acquire();
}

bool UtSemaphore::TryAcquire()
{
    return mImplPtr->TryAcquire();
}
