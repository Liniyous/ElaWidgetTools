#include "PakProcessor.h"

#include <cassert>
#include <iostream>

#include "GenIConvertBigEndian.h"
#include "PakHeader.h"
#include "PakI.h"
#include "PakPacket.h"
#include "PakSocketIO.h"
#include "PakUndefinedPacket.h"
PakProcessor::PakProcessor()
{
    mPacketData.assign(1024, (PacketInfo*)nullptr);
}
PakProcessor::~PakProcessor()
{
    for (unsigned int i = 0; i < mPacketData.size(); ++i)
    {
        delete mPacketData[i];
    }
}

// static
//! Read a PakPacket from a PakSocketIO
PakPacket* PakProcessor::ReadPacket(PakSocketIO& aIO)
{
    PakPacket* lReturn = nullptr;
    int id;
    int length;
    if (aIO.ReceiveHeader(id, length, 0))
    {
        PacketInfo* pInfo = mPacketData[id];
        if (pInfo == nullptr)
        {
            std::cout << "Received unknown packet. " << "ID: " << id << std::endl;
            aIO.IgnorePacket();
        }
        else
        {
            if (pInfo->IsUndefinedPacket())
            {
                PakUndefinedPacket* upkt = new PakUndefinedPacket;
                upkt->Initialize(id, length);
                lReturn = upkt;
            }
            else
            {
                lReturn = pInfo->GetNewPacket();
            }

            if (!aIO.Receive(*lReturn))
            {
                delete lReturn;
                lReturn = nullptr;
            }
        }
    }
    return lReturn;
}

PakPacket* PakProcessor::ReadPacket(PakI& aInput, PakHeader& aHeader)
{
    PakPacket* lReturn = nullptr;
    int id, length;
    aHeader.ReadHeader(*aInput.GetBuffer(), id, length);
    PacketInfo* pInfo = mPacketData[id];
    if (pInfo == nullptr)
    {
        std::cout << "Received unknown packet. " << "ID: " << id << std::endl;
        for (int i = 0; i < length; ++i)
        {
            char garbage;
            aInput.GetBuffer()->getValue(garbage);
        }
    }
    else
    {
        if (pInfo->IsUndefinedPacket())
        {
            PakUndefinedPacket* upkt = new PakUndefinedPacket;
            upkt->Initialize(id, length);
            lReturn = upkt;
        }
        else
        {
            lReturn = pInfo->GetNewPacket();
        }
        (*pInfo->mReadFn)(*lReturn, aInput);
    }
    return lReturn;
}

//! Read a packet from a buffer
//! This method is for backward compatibility, use the other ReadPacket()
PakPacket* PakProcessor::ReadPacket(char* aBuffer)
{
    GenIConvertBigEndian convert;
    struct p_header {
        int length;
        int id;
    };
    int id, length;
    convert.GetFromBuf((unsigned char*)aBuffer, length);
    convert.GetFromBuf((unsigned char*)aBuffer + sizeof(int), id);
    PacketInfo* pInfo = mPacketData[id];
    PakPacket* pkt = pInfo->GetNewPacket();
    if (pInfo->IsUndefinedPacket())
    {
        static_cast<PakUndefinedPacket*>(pkt)->Initialize(id, length);
    }
    GenBuffer packetBuffer(aBuffer + sizeof(p_header), length);
    packetBuffer.SetPutPos(length);
    PakI serial(&packetBuffer);
    (*pInfo->mReadFn)(*pkt, serial);
    return pkt;
}

//! Calls any callbacks associated with this packet.
void PakProcessor::ProcessPacket(PakPacket* aPkt, bool aDoCleanup /*=false*/)
{
    PacketInfo* pInfo = mPacketData[aPkt->ID()];
    pInfo->Call(*aPkt);
    int basePacketId = 0;

    PacketInfo* baseInfoPtr = pInfo;
    for (;;)
    {
        basePacketId = baseInfoPtr->GetBasePacketId();
        assert(basePacketId != aPkt->ID());
        if (basePacketId != -1)
        {
            baseInfoPtr = mPacketData[basePacketId];
            baseInfoPtr->Call(*aPkt);
        }
        else
        {
            break;
        }
    }

    if (aDoCleanup)
    {
        aPkt->ReceiveCleanup();
        delete aPkt;
    }
}

void PakProcessor::SubscribeP(int aPacketId, UtCallback* aCallbackPtr, bool aIsSpecific)
{
    PacketInfo* info = mPacketData[aPacketId];
    if (info)
    {
        if (aIsSpecific)
        {
            info->ConnectSpecific(aCallbackPtr);
        }
        else
        {
            info->ConnectGeneric(aCallbackPtr);
        }
    }
    else
    {
        { // RAII block
            std::cout << "Packet is not registered. " << "ID: " << aPacketId << std::endl;
        }
        exit(0);
    }
}

PakProcessor::PacketInfo* PakProcessor::RegisterPacketP(int aPacketId,
                                                        const std::string aPacketName,
                                                        PacketCallbackList* aCallbackListPtr,
                                                        bool aIsUndefined,
                                                        int aPacketBaseId)
{
    if (int(mPacketData.size()) <= aPacketId)
    {
        mPacketData.insert(mPacketData.end(), aPacketId - mPacketData.size() + 1, (PacketInfo*)nullptr);
    }

    PacketInfo* pInfo = mPacketData[aPacketId];
    if (pInfo != nullptr)
    {
        { // RAII block
            std::cout << "Packet already initialized, cannot RegisterPacket again."
                      << " Name: " << aPacketName
                      << " ID: " << aPacketId
                      << std::endl;
        }
        exit(0);
    }

    pInfo = mPacketData[aPacketId] = new PacketInfo(aPacketId, aPacketName, aCallbackListPtr, aIsUndefined);
    pInfo->SetBasePacketId(aPacketBaseId);
    return pInfo;
}

PakProcessor::PacketInfo::PacketInfo(int aPacketId, std::string aPacketName, PacketCallbackList* aCallbackListPtr, bool aIsUndefined)
    : mPacketID(aPacketId), mPacketName(aPacketName), mSpecificCallbackList(aCallbackListPtr)
{
    mPacketID = aPacketId;
    mPacketName = aPacketName;
    mIsUndefinedPacket = aIsUndefined;
    mBasePacketID = -1;
}

PakProcessor::PacketInfo::~PacketInfo()
{
    delete mSpecificCallbackList;
}

bool PakProcessor::PacketInfo::IsUndefinedPacket()
{
    return mIsUndefinedPacket;
}

void PakProcessor::PacketInfo::ConnectSpecific(UtCallback* aCallbackPtr)
{
    mSpecificCallbackList->Connect((UtCallbackN<void(PakPacket&)>*)aCallbackPtr);
}

void PakProcessor::PacketInfo::ConnectGeneric(UtCallback* aCallbackPtr)
{
    mGenericCallbackList.Connect((UtCallbackN<void(PakPacket&)>*)aCallbackPtr);
}

PakPacket* PakProcessor::PacketInfo::GetNewPacket()
{
    return (*mNewFn)();
}

void PakProcessor::PacketInfo::Call(PakPacket& aPkt)
{
    mSpecificCallbackList->Call(aPkt);
    mGenericCallbackList.Call(aPkt);
}
