#pragma once
#ifndef ASYNC_STATES_WORKITEMSTATES_H
#define ASYNC_STATES_WORKITEMSTATES_H


// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES


namespace Async
{
namespace States
{

    enum class WorkItemState
    {
        IDLE = 0,
        EVALUATING_PRECONDITIONS = 1,
        EXECUTING_MAIN_FUNCTION = 2,
        EXECUTING_CLEANUP_FUNCTION = 3,
        QUEUE = 4,
        REQUEUE = 5,
        DONE = 6,
    };

} // end of namespace States

std::string GetWorkItemStateString(States::WorkItemState state);

} // end of namespace Async

#endif // end of ASYNC_STATES_WORKITEMSTATES_H
