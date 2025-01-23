#ifndef PAKTYPEDICTIONARY_H
#define PAKTYPEDICTIONARY_H

#include "ElaPacketIO_Export.h"

#include <cassert>
#include <typeindex>
#include <typeinfo>

#include "PakSerialize.h"

class PakO;
class PakI;
class UtNullCreate
{
public:
};

namespace PakTypeDictionaryDetail
{
//   #if __GNUC__ && __GNUC__ == 4 && __GNUC_MINOR__ <= 4
template <typename T>
class HasNullCreateCTor
{
    template <typename C>
    struct type_check;
    template <typename C>
    static char test(type_check<typename C::UseNullCreate>*);
    template <typename C>
    static long test(...);

public:
    enum
    {
        value = sizeof(test<T>(0)) == sizeof(char)
    };
};
template <typename T, bool hasNullCreate>
struct CreateObject {
    static T* Create() { return new T(); }
};
template <typename T>
struct CreateObject<T, true> {
    static T* Create()
    {
        UtNullCreate nc;
        return new T(nc);
    }
};
template <typename T, typename ARCHIVE>
struct TypeMethods {
    static void* TypeCreate() { return CreateObject<T, HasNullCreateCTor<T>::value>::Create(); }
    static void Serialize(ARCHIVE& aBuff, void* aData)
    {
        bool isNull = aData == nullptr;
        aBuff & isNull;
        if (!isNull)
        {
            ((T*)aData)->Serialize(aBuff);
        }
    }
};
template <typename DICTIONARY, typename ARCHIVE, typename T, bool IS_OUTPUT>
struct DictionarySerializeFwd {
    static void Go(DICTIONARY* aThis, PakI& aAr, T*& aData)
    {
        void* dataPtr = (void*)aData;
        aThis->SerializeIn(aAr, dataPtr);
        aData = (T*)dataPtr;
    }
};
template <typename DICTIONARY, typename T>
struct DictionarySerializeFwd<DICTIONARY, PakO, T, true> {
    static void Go(DICTIONARY* aThis, PakO& aAr, T*& aData) { aThis->SerializeOut(aAr, aData); }
};

} // namespace PakTypeDictionaryDetail

class ELA_PACKETIO_EXPORT PakTypeDictionary
{
public:
    static PakTypeDictionary& GetInstance();

    ~PakTypeDictionary()
    {
        for (TypeIdMap::iterator i = mTypeIdTable.begin(); i != mTypeIdTable.end(); ++i)
        {
            Data* dataPtr = (Data*)i->second;
            delete dataPtr;
        }
    }

    template <typename T>
    void RegisterType(int aTypeId)
    {
        using namespace PakTypeDictionaryDetail;
        Data* d = new Data;
        d->mCreateFnPtr = &TypeMethods<T, PakI>::TypeCreate;
        d->mSerializeInFnPtr = &TypeMethods<T, PakI>::Serialize;
        d->mSerializeOutFnPtr = &TypeMethods<T, PakO>::Serialize;
        d->mId = aTypeId;
        Register(typeid(T), aTypeId, d);
    }

    template <typename ARCHIVE, typename T>
    void Serialize(ARCHIVE& aAr, T*& aData)
    {
        bool isValid = (aData != nullptr);
        aAr & isValid;
        if (isValid)
        {
            PakTypeDictionaryDetail::DictionarySerializeFwd<PakTypeDictionary, ARCHIVE, T, ARCHIVE::cIS_OUTPUT>::Go(this,
                                                                                                                    aAr,
                                                                                                                    aData);
        }
        else
        {
            aData = nullptr;
        }
    }

    void SerializeIn(PakI& aAr, void*& aData);

    template <typename T>
    void SerializeOut(PakO& aAr, T* aData)
    {
        Data* dataPtr;
        bool found = FindData(typeid(*aData), (void*&)dataPtr);
        assert(found); // Type not added to map?
        if (found)
        {
            aAr & dataPtr->mId;
            (*dataPtr->mSerializeOutFnPtr)(aAr, (void*)aData);
        }
    }
    void Register(const std::type_index& aType, int aId, void* aData);
    bool FindData(int aId, void*& aData);
    bool FindData(const std::type_index& aType, void*& aData);

private:
    typedef std::map<std::type_index, int32_t> TypeMap;
    typedef std::map<int, void*> TypeIdMap;
    TypeMap mTypeTable;
    TypeIdMap mTypeIdTable;

    struct Data {
        Data()
            : mCreateFnPtr(nullptr), mSerializeInFnPtr(nullptr), mSerializeOutFnPtr(nullptr)
        {
        }
        typedef void* (*CreateFn)();
        typedef void (*SerializeInFn)(PakI&, void*);
        typedef void (*SerializeOutFn)(PakO&, void*);
        CreateFn mCreateFnPtr;
        SerializeInFn mSerializeInFnPtr;
        SerializeOutFn mSerializeOutFnPtr;
        int32_t mId;
    };
};

namespace PakSerialization
{
template <typename T>
inline void Save(PakO& aAr, PakSerialization::PakSerializePolyPtr<T>& aPtr)
{
    PakTypeDictionary::GetInstance().Serialize(aAr, (typename PakSerialization::Non_Const<T*>::Type&)*aPtr.mPtr);
}
template <typename T>
inline void Load(PakI& aAr, PakSerialization::PakSerializePolyPtr<T>& aPtr)
{
    PakTypeDictionary::GetInstance().Serialize(aAr, (typename PakSerialization::Non_Const<T*>::Type&)*aPtr.mPtr);
}
} // namespace PakSerialization

#endif
