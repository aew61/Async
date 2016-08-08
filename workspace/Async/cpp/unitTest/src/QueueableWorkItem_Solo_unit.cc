// SYSTEM INCLUDES
#include "gtest/gtest.h"

// C++ PROJECT INCLUDES
#include "QueueableWorkItemTestChild.h"

namespace Async
{
namespace Tests
{

    TEST(Async_QueueableWorkItem_unit, Test_Child_Constructor)
    {
        QueueableWorkItemTestChild test;

        EXPECT_NE(nullptr, &test);
        EXPECT_EQ(1, test.GetRefCount());
        EXPECT_FALSE(test.GetVal());
    }

    TEST(Async_QueueableWorkItem_unit, Test_Child_Execute)
    {
        QueueableWorkItemTestChild test;

        test.ExecuteInterface();
        EXPECT_TRUE(test.GetVal());
    }

} // end of namespace Tests
} // end of namespace Async
