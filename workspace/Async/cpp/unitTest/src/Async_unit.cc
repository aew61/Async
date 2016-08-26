// SYSTEM INCLUDES
#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>
#include <gtest/gtest.h>


// C++ PROJECT INCLUDES
#include "Async/Async.h"
#include "Async/WorkObject.h"
#include "ValueHolderTestChild.h"
#include "UnitTest_utils.h"

namespace Async
{
namespace Tests
{

    TEST(Async_Async_unit, Test_Start_And_Stop)
    {
        EXPECT_TRUE(Start(3));
        EXPECT_TRUE(Stop());
    }

    TEST(Async_Async_unit, Test_NumActiveThreads)
    {
        EXPECT_TRUE(Start(3));
        EXPECT_EQ(3, NumActiveThreads());
        EXPECT_TRUE(Stop());
    }

} // end of namespace Tests
} // end of namespace Async
