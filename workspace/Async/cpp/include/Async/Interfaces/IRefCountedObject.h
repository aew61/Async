#pragma once
#ifndef ASYNC_IREFCOUNTEDOBJECT_H
#define ASYNC_IREFCOUNTEDOBJECT_H

// SYSTEM INCLUDES
#include <exception>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.h"
#include "Async/Interfaces/IValueHolder.h"

namespace Async
{

    class IValueHolder;

    class ASYNC_API IRefCountedObject
    {
    public:

        virtual void Load(IValueHolder* pPromise) = 0;

        virtual std::exception_ptr GetError() const = 0;

        virtual void WaitForExecution() = 0;

    protected:

        virtual ~IRefCountedObject() = default;

        virtual int IncRef() = 0;

        virtual int DecRef() = 0;

        virtual const int GetRefCount() = 0;

    private:
    };

} // end of namespace Async

#endif // end of ASYNC_IREFCOUNTEDOBJECT_H
