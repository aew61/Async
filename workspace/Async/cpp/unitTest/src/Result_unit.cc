// SYSTEM INCLUDES
#include <string>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/Result.h"

namespace Async
{
namespace Tests
{

    TEST(Async_Result_unit, Test_Instantiation)
    {
        Types::Result_t val = Types::Result_t::SUCCESS;
        EXPECT_EQ(val, Types::Result_t::SUCCESS);

		val = Types::Result_t::FAILURE;
        EXPECT_EQ(val, Types::Result_t::FAILURE);

		val = Types::Result_t::UNKNOWN;
        EXPECT_EQ(val, Types::Result_t::UNKNOWN);
    }

    TEST(Async_Result_unit, Test_ResultToString)
    {
        std::string comp = "SUCCESS";
        Types::Result_t val = Types::Result_t::SUCCESS;
        EXPECT_EQ(comp, ResultToString(val));

        comp = "FAILURE";
		val = Types::Result_t::FAILURE;
        EXPECT_EQ(comp, ResultToString(val));

        comp = "UNKNOWN";
		val = Types::Result_t::UNKNOWN;
        EXPECT_EQ(comp, ResultToString(val));
    }

} // end of namespace Tests
} // end of namespace Async
