// SYSTEM INCLUDES
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/Enums/ConcurrencyStates.h"

namespace Async
{

    std::string GetConcurrencyStateString(const States::ConcurrencyState state)
    {
        switch(state)
        {
            case States::ConcurrencyState::IDLE:
                return "IDLE";
            case States::ConcurrencyState::BUSY:
                return "BUSY";
            case States::ConcurrencyState::DONE:
                return "DONE";
            throw std::logic_error("Unknown State");
        }
        return "";
    }

} // end of namespace Async
