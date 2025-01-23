#include "GenSocketConnection.h"

#include "GenSocket.h"

GenSocketConnection::GenSocketConnection(GenSockets::GenSocket* aSocket)
    : mSocket(aSocket)
{
}

//! Returns 'true' if the IO is connected
bool GenSocketConnection::IsConnected() const
{
    return mSocket->IsConnected();
}

// virtual
//! Returns 'true' if the IO is non-blocking
bool GenSocketConnection::IsNonBlocking() const
{
    return (mSocket->GetSocketOptions() & GenSockets::GenSocket::cNON_BLOCKING) != 0;
}

// virtual
//! Sets the blocking flag.
//! @param aTF 'true' makes the IO non-blocking.  'false' makes the IO blocking.
void GenSocketConnection::SetIsNonBlocking(bool aTF)
{
    if (aTF)
    {
        mSocket->AddSocketOptions(GenSockets::GenSocket::cNON_BLOCKING);
    }
    else
    {
        mSocket->RemoveSocketOptions(GenSockets::GenSocket::cNON_BLOCKING);
    }
}

// virtual
int GenSocketConnection::GetLocalPort() const
{
    int lReturn = 0;
    lReturn = mSocket->GetBoundPort();
    return lReturn;
}
