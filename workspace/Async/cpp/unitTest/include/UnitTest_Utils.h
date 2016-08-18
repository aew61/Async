#pragma once
#ifndef ASYNC_TESTS_UNITTEST_UTILS_H
#define ASYNC_TESTS_UNITTEST_UTILS_H

// SYSTEM INCLUDES
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/WorkerThread.h"
#include "DynamicQueueableObjectTestChild.h"
#include "SharedMemory.h"


namespace Async
{
namespace Tests
{

    void CreateChain(std::vector<DynamicQueueableObjectTestChild*>* pVec,
                     Concurrency::WorkerThread* pThread,
                     int blockingTime, int numWorkItemSoFar, int maxWorkItems);

    void SpawnChild(SharedMemory* pShared);

} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_TESTS_UNITTEST_UTILS_H
