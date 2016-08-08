#pragma once
#ifndef ASYNC_TESTS_DYNAMICQUEUEABLEWORKITEMTESTCHILD_H
#define ASYNC_TESTS_DYNAMICQUEUEABLEWORKITEMTESTCHILD_H

// SYSTEM  INCLUDES
#include <functional>

// C++ PROJECT INCLUDES
#include "Async/QueueableWorkItem.h"

namespace Async
{
namespace Tests
{

    class DynamicQueueableWorkItemTestChild : public QueueableWorkItem
    {
    public:

        DynamicQueueableWorkItemTestChild(std::function<void()> pFunc);

        ~DynamicQueueableWorkItemTestChild();

        virtual Types::Result_t Queue(Concurrency::WorkerThread* pThread) override;

        bool GetVal();

        int GetRefCount();

    protected:

        virtual void Execute() override;

    private:

        virtual bool IsDone() override;

    private:

        std::function<void()>   _pFunc;
        bool                    _toSet;

    };

} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_TESTS_DYNAMICQUEUEABLEWORKITEMTESTCHILD_H
