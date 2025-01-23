#include "ElaXIO_Connection.h"

#include <iostream>

#include "ElaXIO_Interface.h"
#include "ElaXIO_PacketRegistry.h"
#include "PakTCP_IO.h"
#include "PakUDP_IO.h"

ElaXIO_Connection::ElaXIO_Connection(ElaXIO_Interface* aInterfacePtr, PakSocketIO* aIOPtr)
    : mInterfacePtr(aInterfacePtr), mLinkedConnectionPtr(nullptr), mIOPtr(aIOPtr), mIsServer(false), mIsInitialized(false), mDisconnecting(false), mHasClockTranslation(false), mLastTimeStamp(0.0)
{
    static int sUniqueConnectionId = 1;
    mConnectionId = sUniqueConnectionId++;

    mTCP_IO_Ptr = dynamic_cast<PakTCP_IO*>(mIOPtr);
    mUDP_IO_Ptr = dynamic_cast<PakUDP_IO*>(mIOPtr);
}

ElaXIO_Connection::~ElaXIO_Connection()
{
    delete mIOPtr;
}

void ElaXIO_Connection::send(ElaXIO_Packet& aPkt)
{
    mInterfacePtr->send(aPkt, this);
}
