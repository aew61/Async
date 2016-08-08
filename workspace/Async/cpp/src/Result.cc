// SYSTEM INCLUDES
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/Result.h"

namespace Async
{

    const std::string ResultToString(Types::Result_t result)
    {
        switch (result)
		{
		case Types::Result_t::SUCCESS:
			return "SUCCESS";
		case Types::Result_t::FAILURE:
			return "FAILURE";
		case Types::Result_t::UNKNOWN:
			return "UNKNOWN";
		default:
			throw std::logic_error("Unknown Result type");
		}
    }

} // end of namespace Async
