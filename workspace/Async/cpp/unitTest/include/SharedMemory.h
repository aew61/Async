#pragma once
#ifndef ASYNC_TESTS_SHAREDMEMORY_H
#define ASYNC_TESTS_SHAREDMEMORY_H

// SYSTEM INCLUDES
#include <mutex>
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/WorkerThread.h"
#include "Async/GarbageCollector.h"
#include "DynamicQueueableObjectTestChild.h"

namespace Async
{
namespace Tests
{

    struct SharedMemory
    {
        SharedMemory(Concurrency::WorkerThread* pWT);

        ~SharedMemory();

        std::vector<DynamicQueueableObjectTestChild*>   _vec;
        std::mutex                                      _mutex;
        int                                             _count;
        Concurrency::WorkerThread*                      _pWT;
    };

} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_TESTS_SHAREDMEMORY_H
