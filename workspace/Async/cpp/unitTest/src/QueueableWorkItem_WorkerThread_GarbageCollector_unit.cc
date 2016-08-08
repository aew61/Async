// SYSTEM INCLUDES
#include <chrono>
#include <functional>
#include <thread>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/WorkerThread.h"
#include "Async/GarbageCollector.h"
#include "DynamicQueueableWorkItemTestChild.h"

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
    }

} // end of namespace Tests
} // end of namespace Async
