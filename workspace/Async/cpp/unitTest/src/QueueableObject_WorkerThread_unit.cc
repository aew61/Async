// SYSTEM INCLUDES
#include <chrono>
#include <functional>
#include <thread>
#include <vector>
#include <gtest/gtest.h>

// C++ PROJECTS INCLUDES
#include "Async/WorkerThread.h"
#include "QueueableObjectTestChild.h"
#include "DynamicQueueableObjectTestChild.h"
#include "GarbageCollector_noDealloc.h"
#include "UnitTest_Utils.h"

namespace Async
{
namespace Tests
{

    TEST(Async_QueueableObject_WorkerThread_unit, Test_Queue_And_Execution_Solo)
    {
        QueueableObjectTestChild test;
        GarbageCollector_noDealloc testCollector;
        Concurrency::WorkerThread wThread(&testCollector);

        // queue work item
        EXPECT_FALSE(test.GetVal());
        EXPECT_TRUE(wThread.Queue(&test));

        // cleanup
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        wThread.Join();
        EXPECT_TRUE(test.GetVal());
        testCollector.Join();
    }

    TEST(Async_QueueableObject_WorkerThread_unit, Test_Queue_And_Execution_Sequential)
    {
        int numWorkItems = 15;
        std::vector<QueueableObjectTestChild*> testVector;
        GarbageCollector_noDealloc testCollector;
        Concurrency::WorkerThread wThread(&testCollector);

        for(int i = 0; i < numWorkItems; ++i)
        {
            QueueableObjectTestChild* pChild = new QueueableObjectTestChild();
            EXPECT_FALSE(pChild->GetVal());
            testVector.push_back(pChild);
        }

        // add all the children to the WorkerThread
        for(int i = 0; i < numWorkItems; ++i)
        {
            EXPECT_TRUE(wThread.Queue(testVector[i]));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        wThread.Join();

        for(int i = 0; i < numWorkItems; ++i)
        {
            EXPECT_TRUE(testVector[i]->GetVal());
            EXPECT_EQ(0, testVector[i]->GetChildRefCount());
            delete testVector[i];
        }
        testCollector.Join();
        testVector.clear();
    }

    TEST(Async_QueueableObject_WorkerThread_unit, Test_Queue_And_Execution_Interleaved)
    {
        int numWorkItems = 15;
        std::vector<DynamicQueueableObjectTestChild*> testVector;
        GarbageCollector_noDealloc testCollector;
        Concurrency::WorkerThread wThread(&testCollector);

        CreateChain(&testVector, &wThread, 5, 0, numWorkItems);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        wThread.Join();
        EXPECT_EQ(numWorkItems, testVector.size());
        for(int i = 0; i < testVector.size(); ++i)
        {
            EXPECT_TRUE(testVector[i]->GetVal());
            EXPECT_EQ(0, testVector[i]->GetChildRefCount());
            delete testVector[i];
        }
        testCollector.Join();
        testVector.clear();
    }

} // end of namespace Tests
} // end of namespace Async
