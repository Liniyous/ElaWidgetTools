#include "PakTypeDictionary.h"

#include "PakI.h"
#include "PakO.h"

// Keep as last include
#include "PakSerializeImpl.h"

void PakTypeDictionary::Register(const std::type_index& aType, int aId, void* aData)
{
    mTypeIdTable[aId] = aData;
    mTypeTable[aType] = aId;
}
bool PakTypeDictionary::FindData(int aId, void*& aData)
{
    bool found = false;
    TypeIdMap::iterator i = mTypeIdTable.find(aId);
    if (i != mTypeIdTable.end())
    {
        aData = i->second;
        found = true;
    }
    return found;
}

bool PakTypeDictionary::FindData(const std::type_index& aType, void*& aData)
{
    bool found = false;
    TypeMap::iterator i = mTypeTable.find(aType);
    if (i != mTypeTable.end())
    {
        aData = mTypeIdTable[i->second];
        found = true;
    }
    return found;
}

PakTypeDictionary& PakTypeDictionary::GetInstance()
{
    static PakTypeDictionary i;
    return i;
}

void PakTypeDictionary::SerializeIn(PakI& aAr, void*& aData)
{
    int32_t id;
    aAr & id;
    Data* dataPtr;
    bool found = FindData(id, (void*&)dataPtr);
    assert(found); // Type not added to map?
    if (found)
    {
        aData = (*dataPtr->mCreateFnPtr)();
        (*dataPtr->mSerializeInFnPtr)(aAr, aData);
    }
}
