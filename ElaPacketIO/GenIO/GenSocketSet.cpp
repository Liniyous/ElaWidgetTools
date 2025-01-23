#include "GenSocketSet.h"

#include <algorithm>

namespace GenSockets
{

void GenSocketSet::AddSocket(GenSocket* aSocket)
{
    mSockets.push_back(aSocket);
}

void GenSocketSet::RemoveSocket(GenSocket* aSocket)
{
    mSockets.resize(std::remove(mSockets.begin(), mSockets.end(), aSocket) - mSockets.begin());
}

} // namespace GenSockets
