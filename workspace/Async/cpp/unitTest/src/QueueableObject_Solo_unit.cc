// SYSTEM INCLUDES
#include "gtest/gtest.h"

// C++ PROJECT INCLUDES
#include "QueueableObjectTestChild.h"

namespace Async
{
namespace Tests
{

    TEST(Async_QueueableObject_unit, Test_Child_Constructor)
    {
        QueueableObjectTestChild test;

        EXPECT_NE(nullptr, &test);
        EXPECT_EQ(1, test.GetChildRefCount());
        EXPECT_FALSE(test.GetVal());
    }

    TEST(Async_QueueableObject_unit, Test_Child_Execute)
    {
        QueueableObjectTestChild test;

        test.ExecuteInterface();
        EXPECT_TRUE(test.GetVal());
    }

} // end of namespace Tests
} // end of namespace Async
