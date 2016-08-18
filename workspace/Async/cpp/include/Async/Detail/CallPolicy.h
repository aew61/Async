#pragma once
#ifndef ASYNC_DETAIL_CALLPOLICY_H
#define ASYNC_DETAIL_CALLPOLICY_H

// SYSTEM INCLUDES
#include <type_traits>

// C++ PROJECT INCLUDES
#include "Async/Detail/ReturnType.h"

namespace Async
{
namespace Detail
{
    // worry about calling a method given a pointer to the member function and a pointer to an instance
    // as well as a tuple of arguments to unpack (if necessary)

    // STRATEGY: The goal is to "unpack" the tuple so that the values
    // in the tuple are used a arguments to the member function call.
    // We can use variadic unpacking (EX: sizeof...(1, 2, 3, 4), EX: Args... args)
    // to actually do the work for us, but we need to build up a compile time list of
    // indices that the tuple contains. This class will, for a given index, increase
    // the compile time list by 1, and check to see if the current index is the last.

    // An example of how this calling goes is this:
    //      Assume we have a function object f with signature int(int, double)
    //      Assume we have a tuple object: t<int, double>
    //          The calls will go like:
    //              wrapper function (not in this file) will call:
    //              call_impl<int(int, double), <int, double>, 0 == t.length, t.length>
    //                  this call will produce call_1, which (if t.length > 0), will produce:
    //                  call_1 -> call_impl<int(int, double), <int, double>, false, 2, 0>
    //                  call_2 -> call_impl<int(int, double), <int, double>, false, 2, 0 1>
    //                  call_3 -> call_impl<int(int, double), <int, double>, true, 2, 0 1 2>
    //                      call_3 will call f(std::get<N>(std::forward< <int, double> >(t))...);
    //                      which expands into f(std::get<0>(t), std::get<1>(t))
    //                      which will call f(int, double) because std::get<0>(t) is an int, and std::get<1>(t) is a double.
    // Remember, this all happens at compile time, so the runtime is just as fast as a normal function invocation.
    // There are still some small details to wrap up. We are using c++11, so there is no blanket "auto" for return types,
    // we need to tell the compiler what return type to get.
    // Fortunatley, that information is in the function object signature!
    // In the "Async/Detail/ReturnType.h" file are similar metaprogramming constructs that strip off the return
    // type of a function object and store that type for later use. The only thing we need to do to use it
    // is to make sure that if we're given a reference to a function that we strip off the reference when computing
    // the return type. We could use std::decay for this but frankly its overkill. This wouldn't work if you passed
    // a pointer to a pointer or something like that anyways, so passing a reference is all we need to worry about,
    // and we can accomplish this with std::remove_reference.

    // This is the inductive step. Continue to advance the indices in the tuple until we reach the size of the tuple.
    template <typename FUNCTION, typename TUPLE, bool DONE, int TOTAL, int... N>
	struct call_impl
	{
        // notice how this is "trailing return type syntax." For example,
        // auto foo(int a) -> int { return a + 5; } is the same as writing int foo(int a) { return a + 5; }
		static auto call_method(FUNCTION f, TUPLE && t)
			-> typename ReturnType<typename std::remove_reference<FUNCTION>::type>::type
		{
            // iterate with the next index!
			return call_impl<FUNCTION, TUPLE, TOTAL == 1 + sizeof...(N),
                             TOTAL, N..., sizeof...(N)>::call(f, std::forward<TUPLE>(t));
		}
	};

    // this is the end step. Eventually, the current index will be the last in the tuple (bool DONE == true),
    // and we can use variadic unpacking to actually call the function with each argument.
	template <typename FUNCTION, typename TUPLE, int TOTAL, int... N>
	struct call_impl<FUNCTION, TUPLE, true, TOTAL, N...>
	{
		static auto call(FUNCTION f, TUPLE && t) //-> decltype(f) // typename std::result_of<FUNCTION>::type
			-> typename ReturnType<typename std::remove_reference<FUNCTION>::type>::type
		{
            // note this will expand to f(std::get<0>(t), std::get<1>(t), ...);
			return f(std::get<N>(std::forward<TUPLE>(t))...);
		}
	};

    // the process of calling a member function is no different, except we need to keep a pointer
    // to an instance of the member function we are trying to call. EXACT SAME STRATEGY just drag
    // an extra parameter along for the ride.
    template<typename MEMBER_FUNCTION, typename CLASS, typename TUPLE, bool DONE, int TOTAL, int...N>
	struct call_member_function_impl
	{
		static auto call(MEMBER_FUNCTION mf, CLASS* pInstance, TUPLE&& t)
			-> typename ReturnType<typename std::remove_reference<MEMBER_FUNCTION>::type>::type
		{
			return call_member_function_impl<MEMBER_FUNCTION, CLASS, TUPLE, TOTAL == 1 + sizeof...(N),
                                    TOTAL, N..., sizeof...(N)>::call(mf, pInstance, std::forward<TUPLE>(t));
		}
	};

    // The end step, same as calling a function but now we're carrying along a pointer to an instance
    // of the class whose member function we want to use.
	template <typename MEMBER_FUNCTION, typename CLASS, typename TUPLE, int TOTAL, int... N>
	struct call_member_function_impl<MEMBER_FUNCTION, CLASS, TUPLE, true, TOTAL, N...>
	{
		static auto call(MEMBER_FUNCTION mf, CLASS* pInstance, TUPLE && t)
			-> typename ReturnType<typename std::remove_reference<MEMBER_FUNCTION>::type>::type
		{
            // to call a member function you use (instance_ptr->*mem_func_pointer)(args...)
            // OR (instance.*mem_func_pointer)(args...). We are keeping a pointer to an instance so
            // we'll use the first syntax.
			return (pInstance->*mf)(std::get<N>(std::forward<TUPLE>(t))...);
		}
	};

} // end of namespace Detail
} // end of namespace Async

#endif // end of ASYNC_DETAIL_CALLPOLICY_H
