#pragma once
#ifndef ASYNC_TESTS_WORKITEMTESTCHILD_H
#define ASYNC_TESTS_WORKITEMTESTCHILD_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/WorkItem.h"
#include "Async/IGarbageCollector.h"

namespace Async
{
namespace Tests
{

    class WorkItemTestChild : public WorkItem
    {
    public:

        WorkItemTestChild();

        ~WorkItemTestChild();

        void AttachMainFunction(FunctionPtr pFunc);

        void AttachCleanupFunction(FunctionPtr pFunc);

        void SetException(const std::exception_ptr pException);

        States::WorkItemState Execute();

        void Finish();

        States::WorkItemState GetState();

        void SetState(States::WorkItemState newState);

    };

} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_TESTS_WORKITEMTESTCHILD_H
