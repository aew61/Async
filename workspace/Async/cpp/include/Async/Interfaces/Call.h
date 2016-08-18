#pragma once
#ifndef ASYNC_CALL_H
#define ASYNC_CALL_H

// SYSTEM INCLUDES
#include <tuple>

// C++ PROJECT INCLUDES
#include "Async/Detail/ReturnType.h"
#include "Async/Detail/CallPolicy.h"

namespace Async
{

    // in the following, a "function object" can either be:
    //  1)a pointer (to function, member function, static member function, etc.),
    //  2)a std::function,
    //  3)a functor object

    // call a function object with arguments stored in a tuple
    template<typename FUNCTION, typename TUPLE>
    auto call(FUNCTION f, TUPLE&& t)
        -> typename Detail::ReturnType<typename std::remove_reference<FUNCTION>::type>::type
    {
        typedef typename std::decay<TUPLE>::type tupType;
        return Detail::call_impl<FUNCTION, TUPLE, 0 == std::tuple_size<tupType>::value,
                                 std::tuple_size<tupType>::value>::call(f, std::forward<TUPLE>(t));
    }

    // call a function object that takes no arguments
    template<typename FUNCTION>
    auto call(FUNCTION f)
        -> typename Detail::ReturnType<typename std::remove_reference<FUNCTION>::type>::type
    {
        return f();
    }

    // call a member function with arguments stored in a tuple
    template<typename MEMBER_FUNCTION, typename CLASS, typename TUPLE>
    auto call(MEMBER_FUNCTION mf, CLASS* pInstance, TUPLE&& t)
        -> typename Detail::ReturnType<typename std::remove_reference<MEMBER_FUNCTION>::type>::type
    {
        typedef typename std::decay<TUPLE>::type tupType;
        return Detail::call_member_function_impl<FUNCTION, CLASS, 0 == std::tuple_size<tupType>::value,
                                                 std::tuple_size<tupType>::value>::call(mf, pInstance,
                                                                                        std::forward<TUPLE>(t));
    }

    // call a member function with that takes no arguments
    template<typename MEMBER_FUNCTION, typename CLASS, typename TUPLE>
    auto call(MEMBER_FUNCTION mf, CLASS* pInstance)
        -> typename Detail::ReturnType<typename std::remove_reference<MEMBER_FUNCTION>::type>::type
    {
        return (pInstance->*mf)();
    }

} // end of namespace Async

#endif // end of ASYNC_CALL_H
