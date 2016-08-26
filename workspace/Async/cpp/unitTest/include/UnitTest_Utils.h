#pragma once
#ifndef ASYNC_TESTS_UNITTEST_UTILS_H
#define ASYNC_TESTS_UNITTEST_UTILS_H

// SYSTEM INCLUDES
#include <functional>
#include <mutex>
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/WorkerThread.h"
#include "Async/Engine.h"
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

    void SpawnEngineSubmitter(Engine* pEngine, std::function<bool()> pFunc,
                              std::mutex* pRecordMutex,
                              std::vector<std::vector<std::pair<std::thread::id, int> > >* toRecord,
                              unsigned int numObjectsToSpawn);

} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_TESTS_UNITTEST_UTILS_H
