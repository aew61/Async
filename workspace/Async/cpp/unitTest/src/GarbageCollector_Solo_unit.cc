// SYSTEM 
#include <chrono>
#include <thread>
#include <vector>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/GarbageCollector.h"
#include "QueueableObjectTestChild.h"


namespace Async
{
namespace Tests
{

    TEST(Async_GarbageCollector_unit, Test_Constructor)
    {
        GarbageCollector gc;

        EXPECT_NE(nullptr, &gc);
        gc.Join();
    }

    TEST(Async_GarbageCollector_unit, Test_Deletion)
    {
        QueueableObjectTestChild* pTest = new QueueableObjectTestChild();
        GarbageCollector gc;
        gc.Queue(pTest);

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        gc.Join();
    }

    TEST(Async_GarbageCollector_unit, Test_Multiple_Deletion)
    {
        int numChildren = 15;
        std::vector<QueueableObjectTestChild*> testVector;
        GarbageCollector gc;

        for(int i = 0; i < numChildren; ++i)
        {
            testVector.push_back(new QueueableObjectTestChild());
        }

        for(int i = 0; i < numChildren; ++i)
        {
            gc.Queue(testVector[i]);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        gc.Join();
    }

} // end of namespace Tests
} // end of namespace Async
