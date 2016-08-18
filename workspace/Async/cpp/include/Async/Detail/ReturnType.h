#pragma once
#ifndef ASYNC_DETAIL_RETURNTYPE_H
#define ASYNC_DETAIL_RETURNTYPE_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES


namespace Async
{
namespace Detail
{

    template<typename RETURN_TYPE>
    struct ReturnType;

    // functor
    template<typename RETURN_TYPE, typename... ARGS>
    struct ReturnType<RETURN_TYPE(ARGS...)>
    {
        using type = RETURN_TYPE;
    }

    // functor with variadic args
    template<typename RETURN_TYPE, typename... ARGS>
    struct ReturnType<RETURN_TYPE(ARGS..., ...)>
    {
        using type = RETURN_TYPE;
    }

    // function pointer
    template<typename RETURN_TYPE, typename... ARGS>
    struct ReturnType<RETURN_TYPE(*)(ARGS...)>
    {
        using type = RETURN_TYPE;
    }

    // function pointer with variadic arguments
    template<typename RETURN_TYPE, typename... ARGS>
    struct ReturnType<RETURN_TYPE(*)(ARGS..., ...)>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...)>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...)>
    {
        using type = RETURN_TYPE;
    }

    // different qualifiers

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...) &>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) &>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...) &&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) &&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...) const>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) const>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...) const&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) const&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...) const&&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) const&&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...) volatile>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) volatile>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...) volatile&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) volatile&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...) volatile&&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) volatile&&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...)  const volatile>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) const volatile>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...)  const volatile&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) const volatile&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS...)  const volatile&&>
    {
        using type = RETURN_TYPE;
    }

    // class member function pointer with variadic arguments
    template<typename RETURN_TYPE, typename CLASS, typename... ARGS>
    struct ReturnType<RETURN_TYPE(CLASS::*)(ARGS..., ...) const volatile&&>
    {
        using type = RETURN_TYPE;
    }

} // end of namespace Detail
} // end of namespace Async

#endif // end of ASYNC_DETAIL_RETURNTYPE_H
