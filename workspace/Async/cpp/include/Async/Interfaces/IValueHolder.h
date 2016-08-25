#pragma once
#ifndef ASYNC_IVALUEHOLDER_H
#define ASYNC_IVALUEHOLDER_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES


namespace Async
{

    class IValueHolder
    {
    public:

        virtual ~IValueHolder() = default;

        virtual bool Call() = 0;

        virtual void Destroy() = 0;

    };

} // end of namespace Async

#endif // end of ASYNC_IVALUEHOLDER_H
