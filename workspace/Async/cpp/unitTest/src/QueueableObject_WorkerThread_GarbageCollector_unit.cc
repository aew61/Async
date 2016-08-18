// SYSTEM INCLUDES
#include <chrono>
#include <functional>
#include <thread>
#include <vector>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/WorkerThread.h"
#include "Async/GarbageCollector.h"
#include "DynamicQueueableObjectTestChild.h"
#include "SharedMemory.h"
#include "UnitTest_Utils.h"

namespace Async
{
namespace Tests
{

    TEST(Async_QueueableObject_GarbageCollector_unit, Test_Object_Life_Cycle_Solo)
    {
        int var = 0;
        DynamicQueueableObjectTestChild* pChild =
            new DynamicQueueableObjectTestChild([&var, &pChild]() -> void
            {
                EXPECT_EQ(1, pChild->GetChildRefCount());
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

    TEST(Async_QueueableObject_GarbageCollector_unit, Test_Object_Life_Cycle_Multiple_Linear_No_Block)
    {
        int var = 0;
        int numWorkItems = 15;
        std::vector<DynamicQueueableObjectTestChild*> testVector;
        GarbageCollector gc;
        Concurrency::WorkerThread wc(&gc);

        for(int i = 0; i < numWorkItems; ++i)
        {
            testVector.push_back(new DynamicQueueableObjectTestChild([&var]() -> void
            {
                ++var;
            }));
            EXPECT_TRUE(wc.Queue(testVector[i]));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        wc.Join();
        gc.Join();
        EXPECT_EQ(15, var);
    }

    TEST(Async_QueueableObject_GarbageCollector_unit, Test_Object_Life_Cycle_Multiple_Linear_Block)
    {
        int var = 0;
        int numWorkItems = 15;
        std::vector<DynamicQueueableObjectTestChild*> testVector;
        GarbageCollector gc;
        Concurrency::WorkerThread wc(&gc);

        for(int i = 0; i < numWorkItems; ++i)
        {
            testVector.push_back(new DynamicQueueableObjectTestChild([&var]() -> void
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                ++var;
            }));
            EXPECT_TRUE(wc.Queue(testVector[i]));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100 * 17));
        wc.Join();
        gc.Join();
        EXPECT_EQ(15, var);
    }

    TEST(Async_QueueableObject_GarbageCollector_unit, Test_Object_Life_Cycle_Multiple_Interleaved_Block)
    {
        int numWorkItems = 15;
        std::vector<DynamicQueueableObjectTestChild*> testVector;
        GarbageCollector gc;
        Concurrency::WorkerThread wc(&gc);

        CreateChain(&testVector, &wc, 10, 0, numWorkItems);

        std::this_thread::sleep_for(std::chrono::milliseconds(20 * 20));
        EXPECT_EQ(numWorkItems, testVector.size());
        wc.Join();
        gc.Join();
    }

    TEST(Async_QueueableObject_GarbageCollector_unit, Test_Object_Life_Cycle_Multiple_MultiThreaded)
    {
        int numThreads = 15;
        GarbageCollector gc;
        Concurrency::WorkerThread wt(&gc);
        SharedMemory shared(&wt);
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
        wt.Join();
        gc.Join();
        EXPECT_EQ(numThreads, shared._count);
    }

} // end of namespace Tests
} // end of namespace Async
