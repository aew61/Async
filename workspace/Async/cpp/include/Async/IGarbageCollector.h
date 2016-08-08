#pragma once
#ifndef ASYNC_IGARBAGECOLLECTOR_H
#define ASYNC_IGARBAGECOLLECTOR_H

// SYSTEM INCLUDES
#include <thread>

// C++ PROJECT INCLUDES
// #include "Async/LibraryExport.h"
// #include "Async/IExecutableWorkItem.h"
#include "Async/QueueableWorkItem.h"

namespace Async
{

    class /* ASYNC_API */ IGarbageCollector
    {
    public:

        virtual ~IGarbageCollector() = default;

        virtual std::thread::id GetId() = 0;

        // virtual void Queue(IExecutableWorkItem* pWorkItem) = 0;
        virtual void Queue(QueueableWorkItem* pWorkItem) = 0;

        virtual void Join() = 0;

        virtual void Run() = 0;

    };

} // end of namespace Async

#endif // end of ASYNC_IGARBAGECOLLECTOR_H