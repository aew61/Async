// SYSTEM INCLUDES
#include <chrono>
#include <thread>
#include <functional>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "UnitTest_Utils.h"

namespace Async
{
namespace Tests
{

    void CreateChain(std::vector<DynamicQueueableObjectTestChild*>* pVec,
                     Concurrency::WorkerThread* pThread,
                     int blockingTime, int numWorkItemSoFar, int maxWorkItems)
    {
        if(numWorkItemSoFar < maxWorkItems)
        {
            DynamicQueueableObjectTestChild* pTestChild =
                new DynamicQueueableObjectTestChild(
                    [pVec, pThread, blockingTime, numWorkItemSoFar, maxWorkItems]() -> void
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(blockingTime));
                        CreateChain(pVec, pThread, blockingTime, numWorkItemSoFar + 1, maxWorkItems);
                    }
                );
            pVec->push_back(pTestChild);
            EXPECT_TRUE(pThread->Queue(pTestChild));
        }
    }

    void SpawnChild(SharedMemory* pShared)
    {
        std::lock_guard<std::mutex> lock(pShared->_mutex);
        int* pInt = &pShared->_count;
        DynamicQueueableObjectTestChild* pChild =
            new DynamicQueueableObjectTestChild([pInt]() -> void
            {
                *pInt = *pInt + 1;
            });
        pShared->_vec.push_back(pChild);
        pShared->_pWT->Queue(pChild);
    }

} // end of namespac Tests
} // end of namespace Async
