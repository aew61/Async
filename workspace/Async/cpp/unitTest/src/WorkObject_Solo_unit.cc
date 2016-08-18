// SYSTEM INCLUDES
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/WorkObject.h"

namespace Async
{
namespace Tests
{

    TEST(Async_WorkObject_unit, Test_Constructor)
    {
        WorkObject w;
        EXPECT_NE(nullptr, &w);
        EXPECT_EQ(nullptr, w.GetError());
    }

} // end of namespace Tests
} // end of namespace Async
