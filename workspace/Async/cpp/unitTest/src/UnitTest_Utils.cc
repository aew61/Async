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

    void CreateChain(std::vector<DynamicQueueableWorkItemTestChild*>* pVec,
                     Concurrency::WorkerThread* pThread,
                     int blockingTime, int numWorkItemSoFar, int maxWorkItems)
    {
        if(numWorkItemSoFar < maxWorkItems)
        {
            DynamicQueueableWorkItemTestChild* pTestChild =
                new DynamicQueueableWorkItemTestChild(
                    [pVec, pThread, blockingTime, numWorkItemSoFar, maxWorkItems]() -> void
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(blockingTime));
                        CreateChain(pVec, pThread, blockingTime, numWorkItemSoFar + 1, maxWorkItems);
                    }
                );
            pVec->push_back(pTestChild);
            EXPECT_EQ(Types::Result_t::SUCCESS, pThread->Queue(pTestChild));
        }
    }

    void SpawnChild(SharedMemory* pShared)
    {
        std::lock_guard<std::mutex> lock(pShared->_mutex);
        int* pInt = &pShared->_count;
        DynamicQueueableWorkItemTestChild* pChild =
            new DynamicQueueableWorkItemTestChild([pInt]() -> void
            {
                *pInt = *pInt + 1;
            });
        pShared->_vec.push_back(pChild);
        pShared->_wc.Queue(pChild);
    }

} // end of namespac Tests
} // end of namespace Async
