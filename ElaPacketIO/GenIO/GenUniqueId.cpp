#include "GenUniqueId.h"

#include <cassert>
#include <cstring>
#include <ctime>
#include <sstream>

#include "GenHostName.h"
#include "GenIP.h"
#include "GenSocketManager.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

GenUniqueId::GenUniqueId()
    : mIsCreated(false)
{
}

//! Create a GenUniqueId with 3 integer components
GenUniqueId::GenUniqueId(unsigned int aA, unsigned int aB, unsigned int aC)
    : mIsCreated(true)
{
    mID[0].mValue = aA;
    mID[1].mValue = aB;
    mID[2].mValue = aC;
}

//! Copy constructor
GenUniqueId::GenUniqueId(const GenUniqueId& aSrc)
    : mIsCreated(true)
{
    aSrc.Validate();
    mID[0].mValue = aSrc.mID[0].mValue;
    mID[1].mValue = aSrc.mID[1].mValue;
    mID[2].mValue = aSrc.mID[2].mValue;
}

//! Returns a pointer to a 12 byte array representing the unique ID
const char* GenUniqueId::GetData() const
{
    Validate();
    return mID[0].mBytes;
}

//! Returns a 32 bit word from the unique ID.
//! @param aWord The index of the 32 bit word. Must be between 0 and 2
unsigned int GenUniqueId::GetData(int aWord) const
{
    assert(aWord < 3);
    Validate();
    return mID[aWord].mValue;
}

//! Less than operator
bool GenUniqueId::operator<(const GenUniqueId& aRHS) const
{
    Validate();
    aRHS.Validate();
    typedef std::pair<unsigned int, unsigned int> Pair;
    typedef std::pair<std::pair<unsigned int, unsigned int>, unsigned int> Triplet;
    return Triplet(Pair(mID[0].mValue, mID[1].mValue), mID[2].mValue) < Triplet(Pair(aRHS.mID[0].mValue, aRHS.mID[1].mValue), aRHS.mID[2].mValue);
}

//! Assignment
GenUniqueId& GenUniqueId::operator=(const GenUniqueId& aSrc)
{
    aSrc.Validate();
    mID[0].mValue = aSrc.mID[0].mValue;
    mID[1].mValue = aSrc.mID[1].mValue;
    mID[2].mValue = aSrc.mID[2].mValue;
    mIsCreated = true;
    return *this;
}

//! Equivalence operator
bool GenUniqueId::operator==(const GenUniqueId& aRHS) const
{
    return mID[0].mValue == aRHS.mID[0].mValue && mID[1].mValue == aRHS.mID[1].mValue && mID[2].mValue == aRHS.mID[2].mValue;
}

//! Generates the unique ID value
void GenUniqueId::Create() const
{
    const unsigned int cPRIME = 234251123;
    static unsigned int counter = 0;
    counter += cPRIME;
    GenSockets::GenIP ip = GenSockets::GenSocketManager::GetIP(0);
    mID[0].mBytes[0] = ip.GetAddressPart(0);
    mID[0].mBytes[1] = ip.GetAddressPart(1);
    mID[0].mBytes[2] = ip.GetAddressPart(2);
    mID[0].mBytes[3] = ip.GetAddressPart(3);

    unsigned int threadId;
#ifdef _WIN32
    threadId = (unsigned int)GetCurrentThreadId();
#else
    threadId = (unsigned int)getpid();
#endif
    threadId = threadId ^ counter;
    mID[1].mValue = threadId;
    mID[2].mValue = (unsigned int)time(nullptr);
    mIsCreated = true;
}

std::string GenUniqueId::ToString()
{
    Validate();
    std::stringstream ss;
    ss << std::hex << "{" << mID[0].mValue << '-' << mID[1].mValue << '-' << mID[2].mValue << '}';
    return ss.str();
}
