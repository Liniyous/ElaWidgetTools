#include "GenTCP_Server.h"

#include <algorithm>
#include <cstddef> // for size_t
#include <iostream>

#include "GenSocket.h"
#include "GenTCP_IO.h"

GenTCP_Server::GenTCP_Server()
    : mServerSocketPtr(nullptr), mOwnsConnections(true)
{
}

GenTCP_Server::~GenTCP_Server()
{
    if (mOwnsConnections)
    {
        CloseAllConnections();
    }
    delete mServerSocketPtr;
}

//! Closes any connections made using Accept()
//! @note Only valid if the OwnsConnections is true
void GenTCP_Server::CloseAllConnections()
{
    if (mOwnsConnections)
    {
        for (size_t i = 0; i < mConnectionList.size(); ++i)
        {
            delete mConnectionList[i];
        }
        mConnectionList.clear();
    }
}

//! Closes a connection belonging to this server.
//! @param aConnectionPtr Pointer to a connection that was created
//!                       using Accept()
//! @note Only valid if the OwnsConnections is true
void GenTCP_Server::CloseConnection(GenTCP_IO* aConnectionPtr)
{
    if (mOwnsConnections)
    {
        IoList::iterator i = std::find(mConnectionList.begin(), mConnectionList.end(), aConnectionPtr);
        mConnectionList.erase(i);
        delete aConnectionPtr;
    }
}

//! Initializes the server and begins listening for
//! connection requests
bool GenTCP_Server::Init(int aPortNumber)
{
    bool lReturn = false;
    mServerSocketPtr = new GenSockets::GenSocket(GenSockets::GenSocket::cTCP_SOCKET);
    if (mServerSocketPtr->Bind(aPortNumber))
    {
        mServerSocketPtr->Listen();
        lReturn = true;
    }
    else
    {
        std::cout << "Cannot bind socket to port. " << "Port: " << aPortNumber << std::endl;
    }
    return lReturn;
}

//! Returns 'true' the this server class is configured
//! to manage the connections.  Returns 'false' if the
//! connections are managed externally.
bool GenTCP_Server::OwnsConnections()
{
    return mOwnsConnections;
}

//! Sets the OwnsConnections() value
void GenTCP_Server::SetOwnsConnections(bool aTF)
{
    mOwnsConnections = aTF;
}

//! If a connection requests is waiting or is received in the
//! specified wait time, a connection is made to that computer.
//! @param aWaitTimeInMicroSeconds If no connection request is available
//!        GenTCP_Server will wait this number of milliseconds.
//! @return A pointer to the new connection.
GenTCP_IO* GenTCP_Server::Accept(int aWaitTimeInMicroSeconds)
{
    GenTCP_IO* lReturn = nullptr;
    if (mServerSocketPtr)
    {
        std::unique_ptr<GenSockets::GenSocket> sockPtr{mServerSocketPtr->Accept(aWaitTimeInMicroSeconds * 1.0E-6F)};
        if (sockPtr)
        {
            lReturn = new GenTCP_IO();
            lReturn->Init(std::move(sockPtr));
            mConnectionList.push_back(lReturn);
        }
    }
    return lReturn;
}
