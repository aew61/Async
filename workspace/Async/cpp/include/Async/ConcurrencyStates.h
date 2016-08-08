#pragma once
#ifndef ASYNC_STATES_CONCURRENCYSTATES_H
#define ASYNC_STATES_CONCURRENCYSTATES_H

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES


namespace Async
{
namespace States
{

    enum class ConcurrencyState
    {
        IDLE = 0,
        BUSY = 1,
        DONE = 2,
    };

} // end of namespace States

std::string GetConcurrencyStateAsString(const States::ConcurrencyState state);

} // end of namespace Async

#endif // end of ASYNC_STATES_CONCURRENCYSTATES_H
