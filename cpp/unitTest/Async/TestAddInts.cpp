// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/AddInts.h"

namespace Async
{
namespace UnitTest
{

    TEST(SimpleAsyncTest, TestAddInts)
    {
        EXPECT_EQ(8, AddInts(5, 3));
    }

} // end of namespace UnitTest
} // end of namespace Async
