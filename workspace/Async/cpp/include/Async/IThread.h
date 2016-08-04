#pragma once
#ifndef ASYNC_CONCURRENCY_ITHREAD_H
#define ASYNC_CONCURRENCY_ITHREAD_H

// SYSTEM INCLUDES
#include <thread>

// C++ PROJECT INCLUDES
// #include "Async/LibraryExport.h"
#include "Async/Result.h"
#include "Async/ConcurrencyStates.h"
#include "Async/IExecutableWorkItem.h"

namespace Async
{
namespace Concurrency
{

    class /* ASYNC_API */ IThread
    {
    public:

        virtual ~IThread() = default;

        virtual std::thread::id GetId() = 0;

        virtual States::ConcurrencyState GetState() = 0;

        virtual Types::Result_t Queue(IExecutableWorkItem* pWorkItem) = 0;

        virtual void Stop() = 0;

        virtual void Join() = 0;

        virtual void Run() = 0;

    };

} // end of namespace Concurrency
} // end of namespace Async

#endif // end of ASYNC_CONCURRENCY_ITHREAD_H
