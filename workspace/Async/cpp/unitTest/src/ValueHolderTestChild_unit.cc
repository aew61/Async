// SYSTEM INCLUDES
#include <functional>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "ValueHolderTestChild.h"

namespace Async
{
namespace Tests
{

    TEST(Async_ValueHolderTestChild_unit, Test_Constructor)
    {
        ValueHolderTestChild v;
        EXPECT_NE(nullptr, &v);
    }

    TEST(Async_ValueHolderTestChild_unit, Test_Default_Call)
    {
        ValueHolderTestChild v;
        EXPECT_TRUE(v.Call());
    }

    TEST(Async_ValueHolderTestChild_unit, Test_Custom_Call)
    {
        ValueHolderTestChild v;
        int x = 5;
        auto pFunc = [&x]() -> bool
        {
            x += 5;
            return true;
        };
        v.AttachFunc(pFunc);
        EXPECT_TRUE(v.Call());
        EXPECT_EQ(10, x);
    }

} // end of namespace Tests
} // end of namespace Async
