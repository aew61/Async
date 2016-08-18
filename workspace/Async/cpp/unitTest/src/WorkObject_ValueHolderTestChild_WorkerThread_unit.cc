// SYSTEM INCLUDES
#include <chrono>
#include <functional>
#include <thread>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/WorkObject.h"
#include "Async/WorkerThread.h"
#include "ValueHolderTestChild.h"
#include "GarbageCollector_noDealloc.h"

namespace Async
{
namespace Tests
{

    TEST(Async_WorkObject_ValueHolderTestChild_WorkerThread_unit, Test_ValueHolderDeletion)
    {
        WorkObject w;
        ValueHolderTestChild* v = new ValueHolderTestChild();
        w.Load(v);
    }

    TEST(Async_WorkObject_ValueHolderTestChild_WorkerThread_unit, Test_Execute_Default)
    {
        WorkObject w;
        ValueHolderTestChild* v = new ValueHolderTestChild();
        w.Load(v);
        GarbageCollector_noDealloc gc; // no garbage collection
        Concurrency::WorkerThread wt(&gc);

        EXPECT_TRUE(w.Queue(&wt));
        w.WaitForExecution();
        EXPECT_EQ(nullptr, w.GetError());
        wt.Join();
    }

    TEST(Async_WorkObject_ValueHolderTestChild_WorkerThread_unit, Test_Execute_Custom)
    {
        int x = 5;

        WorkObject w;
        ValueHolderTestChild* v = new ValueHolderTestChild();
        v->AttachFunc([&x]() -> bool
        {
            x += 5;
            return true;
        });
        w.Load(v);
        GarbageCollector_noDealloc gc; // no garbage collection
        Concurrency::WorkerThread wt(&gc);

        EXPECT_TRUE(w.Queue(&wt));
        w.WaitForExecution();
        EXPECT_EQ(x, 10);
        EXPECT_EQ(nullptr, w.GetError());
        wt.Join();
    }

} // end of namespace Tests
} // end of namespace Async
