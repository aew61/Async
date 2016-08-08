// SYSTEM INCLUDES
#include <string>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/WorkItemStates.h"

namespace Async
{
namespace Tests
{

    TEST(Async_WorkItemStates_unit, Test_Instantiation)
    {
        States::WorkItemState val = States::WorkItemState::IDLE;
        EXPECT_EQ(val, States::WorkItemState::IDLE);

        val = States::WorkItemState::DONE;
        EXPECT_EQ(val, States::WorkItemState::DONE);

        val = States::WorkItemState::EVALUATING_PRECONDITIONS;
        EXPECT_EQ(val, States::WorkItemState::EVALUATING_PRECONDITIONS);

        val = States::WorkItemState::EXECUTING_MAIN_FUNCTION;
        EXPECT_EQ(val, States::WorkItemState::EXECUTING_MAIN_FUNCTION);

        val = States::WorkItemState::EXECUTING_CLEANUP_FUNCTION;
        EXPECT_EQ(val, States::WorkItemState::EXECUTING_CLEANUP_FUNCTION);

        val = States::WorkItemState::QUEUE;
        EXPECT_EQ(val, States::WorkItemState::QUEUE);

        val = States::WorkItemState::REQUEUE;
        EXPECT_EQ(val, States::WorkItemState::REQUEUE);
    }

    TEST(Async_WorkItemStates_unit, Test_GetWorkItemStateString)
    {
        std::string comp = "Idle";
        States::WorkItemState val = States::WorkItemState::IDLE;
        EXPECT_EQ(comp, GetWorkItemStateString(val));

        comp = "Done";
        val = States::WorkItemState::DONE;
        EXPECT_EQ(comp, GetWorkItemStateString(val));

        comp = "Evaluating PreConditions";
        val = States::WorkItemState::EVALUATING_PRECONDITIONS;
        EXPECT_EQ(comp, GetWorkItemStateString(val));

        comp = "Executing Main Function";
        val = States::WorkItemState::EXECUTING_MAIN_FUNCTION;
        EXPECT_EQ(comp, GetWorkItemStateString(val));

        comp = "Executing Cleanup Function";
        val = States::WorkItemState::EXECUTING_CLEANUP_FUNCTION;
        EXPECT_EQ(comp, GetWorkItemStateString(val));

        comp = "Queue";
        val = States::WorkItemState::QUEUE;
        EXPECT_EQ(comp, GetWorkItemStateString(val));

        comp = "Requeue";
        val = States::WorkItemState::REQUEUE;
        EXPECT_EQ(comp, GetWorkItemStateString(val));
    }

} // end of namespace Tests
} // end of namespace Async
