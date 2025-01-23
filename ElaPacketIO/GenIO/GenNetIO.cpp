#include "GenNetIO.h"

#include <cstdio>
#include <iostream>

#include "GenHostName.h"
#include "GenTCP_IO.h"
#include "GenUDP_IO.h"

GenNetIO::GenNetIO(const char* aRecvBufName, const char* aSendBufName)
    : GenIO(aRecvBufName, aSendBufName)
{
}

// static
//! Creates a new GenNetIO and Initializes it.
//! Uses bidirectional initialization.
//! @param aForeignHost The foreign host name, or IP.  To use multicast: ex.
//!                     "226.122.12.2 192.168.1.34"  Where the first IP
//!                     is the multicast group, and the second is the interface
//!                     to use.  Broadcast can be used by specifying a broadcast IP.
//! @param aForeignPort The port on the foreign computer(s) that is configured to receive
//! @param aLocalPort   The port to receive messages on.
//! @param aOptions     A combination of flags of type GenNetIO::Options
GenNetIO* GenNetIO::Create(const std::string& aForeignHost, int aForeignPort, int aLocalPort, int aOptions)
{
    bool ok = true;
    GenNetIO* r = nullptr;
    if (aOptions & cOPT_TCP)
    {
        GenTCP_IO* tcp = new GenTCP_IO;
        ok = tcp->Init(aForeignHost, aForeignPort);
        if (ok)
        {
            tcp->SetUseMessageHeaders(true);
        }
        r = tcp;
    }
    else if (aOptions & cOPT_UDP)
    {
        r = CreateUDP(aForeignHost, aForeignPort, aLocalPort);
        ok = (r != nullptr);
    }
    if (!ok)
    {
        delete r;
        r = nullptr;
    }
    return r;
}

// static
//! Creates a new GenNetIO and Initializes it.
//! Uses a send-only initialization.
//! @param aForeignHost The foreign host name, or IP.  To use multicast: ex.
//!                     "226.122.12.2 192.168.1.34"  Where the first IP
//!                     is the multicast group, and the second is the interface
//!                     to use.  Broadcast can be used by specifying a broadcast IP.
//! @param aForeignPort The port on the foreign computer(s) that is configured to receive
//! @param aOptions     A combination of flags of type GenNetIO::Options
GenNetIO* GenNetIO::Create(const std::string& aForeignHost, int aForeignPort, int aOptions)
{
    bool ok = true;
    GenNetIO* r = nullptr;
    if (aOptions & cOPT_TCP)
    {
        GenTCP_IO* tcp = new GenTCP_IO;
        ok = tcp->Init(aForeignHost, aForeignPort);
        if (ok)
        {
            tcp->SetUseMessageHeaders(true);
        }
        r = tcp;
    }
    else if (aOptions & cOPT_UDP)
    {
        r = CreateUDP(aForeignHost, aForeignPort);
        ok = (r != nullptr);
    }
    if (!ok)
    {
        delete r;
        r = nullptr;
    }
    return r;
}

// static
//! Creates a new GenNetIO and Initializes it.
//! Uses a receive only initialization.
//! @param aLocalPort   The port to receive messages on.
//! @param aOptions     A combination of flags of type GenNetIO::Options
GenNetIO* GenNetIO::Create(int aLocalPort, int aOptions)
{
    GenNetIO* r = nullptr;
    bool ok = true;
    if (aOptions & cOPT_TCP)
    {
        GenTCP_IO* tcp = new GenTCP_IO;
        ok = tcp->InitListen(aLocalPort);
        if (ok)
        {
            tcp->SetUseMessageHeaders(true);
            ok = tcp->Accept(1000);
        }
        r = tcp;
    }
    else if (aOptions & cOPT_UDP)
    {
        GenUDP_IO* udp = new GenUDP_IO;
        ok = udp->Init(aLocalPort);
        udp->RememberSenderAddress(true);
        r = udp;
    }
    if (!ok)
    {
        delete r;
        r = nullptr;
    }
    return r;
}

//! Helper function to configure a GenUDP_IO
//! @param aForeignHost The IP or Hostname of the foreign host.
//! @param aForeignPort The Port number to send messages to.
//! @param aLocalPort The port number to receive messages from.
//! @return A pointer to a new initialized GenUDP_IO.  Or a null pointer
//!         if initialization failed.
GenUDP_IO* GenNetIO::CreateUDP(const std::string& aForeignHost, int aForeignPort, int aLocalPort)
{
    bool ok = true;
    GenUDP_IO* udpIO = new GenUDP_IO;
    std::string::size_type spaceLocation = aForeignHost.find(' ');
    std::string foreignAddr;
    std::string interfaceAddr;
    if (spaceLocation != aForeignHost.npos)
    {
        foreignAddr = aForeignHost.substr(0, spaceLocation);
        interfaceAddr = aForeignHost.substr(spaceLocation + 1);
    }
    else
    {
        foreignAddr = aForeignHost;
    }

    if (aLocalPort != -1)
    {
        if (interfaceAddr.empty())
        {
            ok = udpIO->Init(foreignAddr, aForeignPort, aLocalPort);
        }
        else
        {
            ok = udpIO->Init(foreignAddr, interfaceAddr, aForeignPort, aLocalPort);
        }
    }
    else
    {
        if (interfaceAddr.empty())
        {
            ok = udpIO->Init(foreignAddr, aForeignPort);
        }
        else
        {
            ok = udpIO->Init(foreignAddr, interfaceAddr, aForeignPort);
        }
    }

    if (ok)
    {
        GenSockets::GenIP ip(foreignAddr);
        if (aLocalPort != -1 && ip.IsMulticast())
        {
            ok = udpIO->AddMulticastMembership(interfaceAddr, foreignAddr);
        }
    }
    if (!ok)
    {
        delete udpIO;
        udpIO = nullptr;
    }
    return udpIO;
}
