#pragma once
#ifndef ASYNC_IEXECUTABLEWORKITEM_H
#define ASYNC_IEXECUTABLEWORKITEM_H

// SYSTEM INCLUDES
#include <exception>
#include <functional>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.h"
#include "Async/Result.h"
#include "Async/WorkItemStates.h"

namespace Async
{

// namespace Concurrency
// {
//     class WorkerThread;
// }

    using FunctionPtr = std::function<States::WorkItemState()>;

    class ASYNC_API IExecutableWorkItem
    {
        // friend class Concurrency::WorkerThread;
    public:

        virtual ~IExecutableWorkItem() = default;

        virtual std::exception_ptr GetException() const = 0;

    protected:

        virtual States::WorkItemState Execute() = 0;

        virtual void AttachMainFunction(FunctionPtr pFunc) = 0;

        virtual void AttachCleanupFunction(FunctionPtr pFunc) = 0;

    private:

        virtual bool IsDone() = 0;

    };

} // end of namespace Async

#endif // end of ASYNC_IEXECUTABLEWORKITEM_H
