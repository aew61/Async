#pragma once
#ifndef ASYNC_TYPES_RESULT_T_H
#define ASYNC_TYPES_RESULT_T_H


// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES


namespace Async
{
namespace Types
{

    enum class Result_t
    {
        SUCCESS = 0,
        FAILURE = 1,
        UNKNOWN = 2,
    };

} // end of namespace Types

const std::string ResultToString(Types::Result_t result);

} // end of namespace Async

#endif // end of ASYNC_TYPES_RESULT_T_H
