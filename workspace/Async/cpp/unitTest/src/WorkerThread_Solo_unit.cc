// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/WorkerThread.h"
#include "Async/Enums/ConcurrencyStates.h"

namespace Async
{
namespace Tests
{

    TEST(Async_WorkerThread_unit, Test_Constructor)
    {
        Concurrency::WorkerThread wThread(nullptr);

        EXPECT_NE(nullptr, &wThread);
        EXPECT_EQ(States::ConcurrencyState::IDLE, wThread.GetState());
        wThread.Join();
    }

} // end of namespace Tests
} // end of namespace Async
