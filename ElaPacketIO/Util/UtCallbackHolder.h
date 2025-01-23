#ifndef UTCALLBACKHOLDER_H
#define UTCALLBACKHOLDER_H

#include "ElaPacketIO_Export.h"

#include <memory>
#include <vector>

#include "UtCallback.h"
class ELA_PACKETIO_EXPORT UtCallbackHolder
{
public:
    UtCallbackHolder() = default;
    UtCallbackHolder(const UtCallbackHolder& /*aSrc*/)
        : mCallbacks()
    {
    }
    UtCallbackHolder& operator=(const UtCallbackHolder&) = delete;
    ~UtCallbackHolder() = default;

    void Add(std::unique_ptr<UtCallback> aCallbackPtr);
    std::unique_ptr<UtCallback> Remove(UtCallback* aCallbackPtr);
    void operator+=(std::unique_ptr<UtCallback> aCallbackPtr) { Add(std::move(aCallbackPtr)); }
    void Clear();
    bool Empty() const { return mCallbacks.empty(); }

private:
    std::vector<std::unique_ptr<UtCallback>> mCallbacks;
};

#endif
