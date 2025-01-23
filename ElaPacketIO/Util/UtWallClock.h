#ifndef UTWALLCLOCK_H
#define UTWALLCLOCK_H

/**
   WallClock Time Functions
   This class provides methods to determine the amount of wall clock time that has elapsed.
*/

#ifdef _WIN32
// Nothing to include for now
#else
#include <sys/time.h>
#endif

#include "ElaPacketIO_Export.h"
#include <iostream>
class ELA_PACKETIO_EXPORT UtWallClock
{
public:
    UtWallClock();
    ~UtWallClock() = default;

    static std::string GetWallClockTime();

    //! Choices for the underlying timer.
    enum TimingMethod
    {
        //! Select the default timing method.
        cDEFAULT,
        //! QueryPerformanceCounter() available on Windows.  Falls back to cSYSTEM_TIME when not available
        cPERFORMANCE_COUNTER,
        //! GetSystemTime() on Windows, gettimeofday() on linux.
        cSYSTEM_TIME,
        //! GetTickCount() ( GetTickCount64() when available ) on Windows.
        cTICK_COUNT
    };

    UtWallClock(TimingMethod aTimingMethod);

    /**
      Return the elapsed wall clock time in seconds since object creation or
      the last call to ResetClock(), or the last call to GetCycleTimeSec
   */
    double GetCycleTime();

    // getValue the reference tine
#ifdef _WIN32
    double GetBaseRef() const;
#else
    timeval GetBaseRef() const;
#endif
    // 返回自对象创建以来经过的时间
    double GetClock() const;

    // 返回操作系统当前运行时间
    double GetRawClock() const;

    /**
      Reset the base time of the wall clock.
      Subsequent calls to GetWallClock() will return the number of seconds
      that have elapsed since this was called.
   */

    void ResetClock();

    //! Sets the underlying timer for UtWallClock.  Calls ResetClock() after changing the timing method.
    void SetTimingMethod(TimingMethod aMethod);

    TimingMethod GetTimingMethod() const { return mTimingMethod; }

private:
    TimingMethod mTimingMethod{cDEFAULT};
    /**
      The value of the raw wall clock at object creation or the last call to Rest
   */
#ifdef _WIN32
    double mSecondsPerTick{0.0};
    double mBaseRef{0.0};
    double mBaseCycleRef{0.0};
    // When using GetTickCount(), this stores an offset to correct for time wrapping to zero.
    mutable double mTimeCorrection{0.0};
#else
    timeval mBaseRef;
    timeval mBaseCycleRef;
#endif
};

#ifdef _WIN32
inline double UtWallClock::GetBaseRef() const
{
    return mBaseRef;
}
#else
inline timeval UtWallClock::GetBaseRef() const
{
    return mBaseRef;
}
#endif
#endif
