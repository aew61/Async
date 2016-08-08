// SYSTEM INCLUDES
#include <chrono>
#include <functional>
#include <thread>
#include <vector>
#include <gtest/gtest.h>

// C++ PROJECTS INCLUDES
#include "Async/Result.h"
#include "Async/WorkerThread.h"
#include "QueueableWorkItemTestChild.h"
#include "DynamicQueueableWorkItemTestChild.h"
#include "GarbageCollector_noDealloc.h"

namespace Async
{
namespace Tests
{

    TEST(Async_QueueableWorkItem_WorkerThread_unit, Test_Queue_And_Execution_Solo)
    {
        QueueableWorkItemTestChild test;
        GarbageCollector_noDealloc testCollector;
        Concurrency::WorkerThread wThread(&testCollector);

        // queue work item
        EXPECT_FALSE(test.GetVal());
        EXPECT_EQ(Types::Result_t::SUCCESS, wThread.Queue(&test));

        // cleanup
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        wThread.Join();
        EXPECT_TRUE(test.GetVal());
        testCollector.Join();
    }

    TEST(Async_QueueableWorkItem_WorkerThread_unit, Test_Queue_And_Execution_Sequential)
    {
        int numWorkItems = 15;
        std::vector<QueueableWorkItemTestChild*> testVector;
        GarbageCollector_noDealloc testCollector;
        Concurrency::WorkerThread wThread(&testCollector);

        for(int i = 0; i < numWorkItems; ++i)
        {
            QueueableWorkItemTestChild* pChild = new QueueableWorkItemTestChild();
            EXPECT_FALSE(pChild->GetVal());
            testVector.push_back(pChild);
        }

        // add all the children to the WorkerThread
        for(int i = 0; i < numWorkItems; ++i)
        {
            EXPECT_EQ(Types::Result_t::SUCCESS, wThread.Queue(testVector[i]));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        wThread.Join();

        for(int i = 0; i < numWorkItems; ++i)
        {
            EXPECT_TRUE(testVector[i]->GetVal());
            EXPECT_EQ(0, testVector[i]->GetRefCount());
            delete testVector[i];
        }
        testCollector.Join();
    }

    void CreateChain(std::vector<DynamicQueueableWorkItemTestChild*>* pVec,
                                                  Concurrency::WorkerThread* pThread,
                                                  int numWorkItemSoFar, int maxWorkItems)
    {
        if(numWorkItemSoFar < maxWorkItems)
        {
            DynamicQueueableWorkItemTestChild* pTestChild =
                new DynamicQueueableWorkItemTestChild(
                    [pVec, pThread, numWorkItemSoFar, maxWorkItems]() -> void
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                        CreateChain(pVec, pThread, numWorkItemSoFar + 1, maxWorkItems);
                    }
                );
            pVec->push_back(pTestChild);
            EXPECT_EQ(Types::Result_t::SUCCESS, pThread->Queue(pTestChild));
        }
    }

    TEST(Async_QueueableWorkItem_WorkerThread_unit, Test_Queue_And_Execution_Interleaved)
    {
        int numWorkItems = 15;
        std::vector<DynamicQueueableWorkItemTestChild*> testVector;
        GarbageCollector_noDealloc testCollector;
        Concurrency::WorkerThread wThread(&testCollector);

        CreateChain(&testVector, &wThread, 0, numWorkItems);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        wThread.Join();
        EXPECT_EQ(numWorkItems, testVector.size());
        for(int i = 0; i < testVector.size(); ++i)
        {
            EXPECT_TRUE(testVector[i]->GetVal());
            EXPECT_EQ(0, testVector[i]->GetRefCount());
            delete testVector[i];
        }
        testCollector.Join();
    }

} // end of namespace Tests
} // end of namespace Async