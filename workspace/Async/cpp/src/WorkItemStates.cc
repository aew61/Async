// SYSTEM INCLUDES
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/WorkItemStates.h"

namespace Async
{

    std::string GetWorkItemStateString(States::WorkItemState state)
	{
		switch (state)
		{
		case States::WorkItemState::DONE:
			return "Done";
		case States::WorkItemState::IDLE:
			return "Idle";
        case States::WorkItemState::EVALUATING_PRECONDITIONS:
            return "Evaluating PreConditions";
		case States::WorkItemState::EXECUTING_MAIN_FUNCTION:
			return "Executing Main Function";
		case States::WorkItemState::EXECUTING_CLEANUP_FUNCTION:
			return "Executing Cleanup Function";
        case States::WorkItemState::QUEUE:
            return "Queue";
		case States::WorkItemState::REQUEUE:
			return "Requeue";
		}
		throw std::logic_error("Unexpected WorkItem State");
	}

} // end of namespace Async
