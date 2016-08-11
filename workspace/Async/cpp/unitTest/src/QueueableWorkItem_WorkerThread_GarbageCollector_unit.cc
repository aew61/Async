// SYSTEM INCLUDES
#include <chrono>
#include <functional>
#include <thread>
#include <vector>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/WorkerThread.h"
#include "Async/GarbageCollector.h"
#include "DynamicQueueableWorkItemTestChild.h"
#include "SharedMemory.h"
#include "UnitTest_Utils.h"

namespace Async
{
namespace Tests
{

    TEST(Async_QueueableWorkItem_GarbageCollector_unit, Test_WorkItem_Life_Cycle_Solo)
    {
        int var = 0;
        DynamicQueueableWorkItemTestChild* pChild =
            new DynamicQueueableWorkItemTestChild([&var, &pChild]() -> void
            {
                EXPECT_EQ(1, pChild->GetRefCount());
                ++var;
            });
        GarbageCollector gc;
        Concurrency::WorkerThread wc(&gc);
        wc.Queue(pChild);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        wc.Join();
        gc.Join();
        EXPECT_EQ(1, var);
    }

    TEST(Async_QueueableWorkItem_GarbageCollector_unit, Test_WorkItem_Life_Cycle_Multiple_Linear_No_Block)
    {
        int var = 0;
        int numWorkItems = 15;
        std::vector<DynamicQueueableWorkItemTestChild*> testVector;
        GarbageCollector gc;
        Concurrency::WorkerThread wc(&gc);

        for(int i = 0; i < numWorkItems; ++i)
        {
            testVector.push_back(new DynamicQueueableWorkItemTestChild([&var]() -> void
            {
                ++var;
            }));
            EXPECT_EQ(Types::Result_t::SUCCESS, wc.Queue(testVector[i]));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        wc.Join();
        gc.Join();
        EXPECT_EQ(15, var);
    }

    TEST(Async_QueueableWorkItem_GarbageCollector_unit, Test_WorkItem_Life_Cycle_Multiple_Linear_Block)
    {
        int var = 0;
        int numWorkItems = 15;
        std::vector<DynamicQueueableWorkItemTestChild*> testVector;
        GarbageCollector gc;
        Concurrency::WorkerThread wc(&gc);

        for(int i = 0; i < numWorkItems; ++i)
        {
            testVector.push_back(new DynamicQueueableWorkItemTestChild([&var]() -> void
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                ++var;
            }));
            EXPECT_EQ(Types::Result_t::SUCCESS, wc.Queue(testVector[i]));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100 * 17));
        wc.Join();
        gc.Join();
        EXPECT_EQ(15, var);
    }

    TEST(Async_QueueableWorkItem_GarbageCollector_unit, Test_WorkItem_Life_Cycle_Multiple_Interleaved_Block)
    {
        int numWorkItems = 15;
        std::vector<DynamicQueueableWorkItemTestChild*> testVector;
        GarbageCollector gc;
        Concurrency::WorkerThread wc(&gc);

        CreateChain(&testVector, &wc, 10, 0, numWorkItems);

        std::this_thread::sleep_for(std::chrono::milliseconds(20 * 20));
        EXPECT_EQ(numWorkItems, testVector.size());
        wc.Join();
        gc.Join();
    }

    TEST(Async_QueueableWorkItem_GarbageCollector_unit, Test_WorkItem_Life_Cycle_Multiple_MultiThreaded)
    {
        int numThreads = 15;
        SharedMemory shared;
        std::vector<std::thread> threads(numThreads);

        for(int i = 0; i < numThreads; ++i)
        {
            threads[i] = std::thread(&SpawnChild, &shared);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for(int i = 0; i < threads.size(); ++i)
        {
            if(threads[i].joinable())
            {
                threads[i].join();
            }
        }
        EXPECT_EQ(numThreads, threads.size());
        EXPECT_EQ(numThreads, shared._vec.size());
        for(int i = 0; i < shared._vec.size(); ++i)
        {
            EXPECT_TRUE(shared._vec[i]->GetVal());
        }
        EXPECT_EQ(numThreads, shared._count);
    }

} // end of namespace Tests
} // end of namespace Async
