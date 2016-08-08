// SYSTEM INCLUDES
#include <exception>
#include <string>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/WorkItem.h"
#include "Async/Result.h"
#include "Async/WorkItemStates.h"
#include "WorkItemTestChild.h"

namespace Async
{
namespace Tests
{

    TEST(Async_WorkItem_unit, Test_Constructor)
    {
        WorkItem workItem;
        EXPECT_NE(nullptr, &workItem);
    }

    TEST(Async_WorkItem_unit, Test_GetException)
    {
        // default value
        WorkItem workItem;
        EXPECT_EQ(nullptr, workItem.GetException());

        WorkItemTestChild wTest;
        std::exception_ptr pException = nullptr;

        // generate exception
        try
        {
            throw std::logic_error("Test exception");
        }
        catch(std::exception e)
        {
            pException = std::current_exception();
        }
        wTest.SetException(pException);

        EXPECT_EQ(pException, wTest.GetException());
    }

    TEST(Async_WorkItem_unit, Test_GetStateAsString)
    {
        // default value
        WorkItem workItem;
        std::string defaultState = "Idle";
        EXPECT_EQ(defaultState, workItem.GetStateAsString());

        WorkItemTestChild wTest;
        States::WorkItemState state = States::WorkItemState::DONE;
        wTest.SetState(state);
        EXPECT_EQ(state, wTest.GetState());
        EXPECT_EQ("Done", wTest.GetStateAsString());
    }

    TEST(Async_WorkItem_unit, Test_Get_And_Set_State)
    {
        WorkItemTestChild wTest;
        // default value
        EXPECT_EQ(States::WorkItemState::IDLE, wTest.GetState());

        wTest.SetState(States::WorkItemState::DONE);
        EXPECT_EQ(States::WorkItemState::DONE, wTest.GetState());
    }

    TEST(Async_WorkItem_unit, Test_AttachMainFunction_And_Execute)
    {
        WorkItemTestChild wTest;
        int x = 5;
        auto pFunc = [&x]() -> States::WorkItemState
        {
            x += 5;
            return States::WorkItemState::DONE;
        };

        wTest.AttachMainFunction(pFunc);
        wTest.Execute();

        EXPECT_EQ(10, x);
        EXPECT_EQ(States::WorkItemState::DONE, wTest.GetState());
    }

    TEST(Async_WorkItem_unit, Test_AttachMainFunction_And_AttachCleanupFunction_And_Execute)
    {
        WorkItemTestChild wTest;
        int x = 5;
        auto pMainFunc = [&x]() -> States::WorkItemState
        {
            x += 5;
            return States::WorkItemState::DONE;
        };
        auto pCleanupFunc = [&x]() -> States::WorkItemState
        {
            x += 5;
            return States::WorkItemState::DONE;
        };

        wTest.AttachMainFunction(pMainFunc);
        wTest.AttachCleanupFunction(pCleanupFunc);
        wTest.Execute();

        EXPECT_EQ(15, x);
        EXPECT_EQ(States::WorkItemState::DONE, wTest.GetState());
    }

    TEST(Async_WorkItem_unit, Test_Main_Function_That_Throws)
    {
        WorkItemTestChild wTest;
        auto pFunc = []() -> States::WorkItemState
        {
            throw std::logic_error("Main Error");
            return States::WorkItemState::DONE;
        };

        wTest.AttachMainFunction(pFunc);
        wTest.Execute();

        EXPECT_EQ(States::WorkItemState::DONE, wTest.GetState());
        EXPECT_NE(nullptr, wTest.GetException());
        try
        {
            std::rethrow_exception(wTest.GetException());
        }
        catch(std::exception e)
        {
            std::string expectedErrorMsg = "Main Error";
            std::string actualErrorMsg = e.what();
            EXPECT_EQ(expectedErrorMsg, actualErrorMsg);
        }
    }

    TEST(Async_WorkItem_unit, Test_Cleanup_Function_That_Throws)
    {
        WorkItemTestChild wTest;
        auto pMainFunc = []() -> States::WorkItemState
        {
            return States::WorkItemState::DONE;
        };
        auto pCleanupFunc = []() -> States::WorkItemState
        {
            throw std::logic_error("Cleanup Error");
            return States::WorkItemState::DONE;
        };

        wTest.AttachMainFunction(pMainFunc);
        wTest.AttachCleanupFunction(pCleanupFunc);
        wTest.Execute();

        EXPECT_EQ(States::WorkItemState::DONE, wTest.GetState());
        EXPECT_NE(nullptr, wTest.GetException());
        try
        {
            std::rethrow_exception(wTest.GetException());
        }
        catch(std::exception e)
        {
            std::string expectedErrorMsg = "Cleanup Error";
            std::string actualErrorMsg = e.what();
            EXPECT_EQ(expectedErrorMsg, actualErrorMsg);
        }
    }

    TEST(Async_WorkItem_unit, Test_Function_Execution_Order)
    {
        WorkItemTestChild wTest;
        int x = 5;

        // do some not communtative math operation. Should execute 4 * (5 + 5)
        auto pMainFunc = [&x]() -> States::WorkItemState
        {
            x += 5;
            return States::WorkItemState::DONE;
        };
        auto pCleanupFunc = [&x]() -> States::WorkItemState
        {
            x *= 4;
            return States::WorkItemState::DONE;
        };

        wTest.AttachMainFunction(pMainFunc);
        wTest.AttachCleanupFunction(pCleanupFunc);

        wTest.Execute();

        EXPECT_EQ(40, x);
    }

} // end of namespace Tests
} // end of namespace Async
