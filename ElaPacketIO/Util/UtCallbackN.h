#ifndef UT_CALLBACK_N_H
#define UT_CALLBACK_N_H

#include <memory>
#include <vector>

#include "UtCallback.h"

template <typename Signature>
class UtCallbackN;

template <typename R, typename... Args>
class UtCallbackN<R(Args...)> : public UtCallback
{
public:
    using FunctionType = std::function<R(Args...)>;

    UtCallbackN()
        : UtCallback()
    {
    }
    UtCallbackN(const FunctionType& aFunc)
        : mFunc(aFunc)
    {
    }

    R operator()(Args... args) const { return mFunc(args...); }

private:
    FunctionType mFunc;
};

template <typename Signature>
class UtCallbackListN;

template <typename R, typename... Args>
class UtCallbackListN<R(Args...)> : public UtCallbackList
{
    UtCallbackListN<R(Args...)>& operator=(UtCallbackListN<R(Args...)>&) = delete; // No implementation
public:
    using CallbackType = UtCallbackN<R(Args...)>;

    void operator()(Args... args) const
    {
        for (auto it = mCallbackList.begin(); it != mCallbackList.end();)
        {
            (*static_cast<CallbackType*>(*it++))(args...);
        }
    }

    std::unique_ptr<CallbackType> Connect(const std::function<R(Args...)>& aFunc)
    {
        auto callbackPtr = Create(aFunc);
        Connect(callbackPtr.get());
        return callbackPtr;
    }

    CallbackType* Connect(CallbackType* aCallbackPtr)
    {
        ConnectP(aCallbackPtr);
        return aCallbackPtr;
    }

    std::unique_ptr<CallbackType> Connect(R (*aFuncPtr)(Args...))
    {
        auto callbackPtr = Create(UtStd::Bind(aFuncPtr));
        Connect(callbackPtr.get());
        return callbackPtr;
    }

    template <typename CT>
    std::unique_ptr<CallbackType> Connect(R (CT::*aFuncPtr)(Args...), CT* aObjPtr)
    {
        auto callbackPtr = Create(UtStd::Bind(aFuncPtr, aObjPtr));
        Connect(callbackPtr.get());
        return callbackPtr;
    }

    void GetCallbacks(std::vector<CallbackType*>& aCallbacks) const
    {
        for (const auto& callback: mCallbackList)
        {
            aCallbacks.push_back(static_cast<CallbackType*>(callback));
        }
    }

    void Merge(UtCallbackListN& aOtherCallbackList) { MergeP(aOtherCallbackList); }

private:
    std::unique_ptr<CallbackType> Create(const std::function<R(Args...)>& aFunc)
    {
        return std::make_unique<CallbackType>(aFunc);
    }
};

#endif
