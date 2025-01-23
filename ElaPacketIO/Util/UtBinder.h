#ifndef UTBINDER_H
#define UTBINDER_H

#include <cstddef>
#include <functional>
#include <type_traits>

template <int...>
struct int_sequence {
};

template <int N, int... IntSeq>
struct make_int_sequence : make_int_sequence<N - 1, N - 1, IntSeq...> {
};
template <int... IntSeq>
struct make_int_sequence<0, IntSeq...> : int_sequence<IntSeq...> {
};

template <int> // begin with 0 here!
struct placeholder_template {
};

namespace std
{
template <int N>
struct is_placeholder<placeholder_template<N>> : integral_constant<int, N + 1> // the one is important
{
};
}; // namespace std

namespace UtStd
{
// Binds a static function with specified integer sequence for placeholders.
template <typename R, typename... Args, int... IntSeq>
std::function<R(Args...)> Bind(R (*aFunc)(Args...), int_sequence<IntSeq...>)
{
    return std::bind(aFunc, placeholder_template<IntSeq>{}...);
}

// Binds a non-static function with specified integer sequence for placeholders.
template <typename R, typename... Args, typename CT, int... IntSeq>
std::function<R(Args...)> Bind(R (CT::*aFunc)(Args...), CT* aObjPtr, int_sequence<IntSeq...>)
{
    return std::bind(aFunc, aObjPtr, placeholder_template<IntSeq>{}...);
}

// Binds a static function.
template <typename R, typename... Args>
std::function<R(Args...)> Bind(R (*aFunc)(Args...))
{
    return Bind(aFunc, make_int_sequence<sizeof...(Args)>{});
}

// Binds a non-static function.
template <typename R, typename... Args, typename CT>
std::function<R(Args...)> Bind(R (CT::*aFunc)(Args...), CT* aObjPtr)
{
    return Bind(aFunc, aObjPtr, make_int_sequence<sizeof...(Args)>{});
}
}; // namespace UtStd

#endif
