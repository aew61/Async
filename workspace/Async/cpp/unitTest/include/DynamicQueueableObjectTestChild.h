#pragma once
#ifndef ASYNC_TESTS_DYNAMICQUEUEABLEOBJECTTESTCHILD_H
#define ASYNC_TESTS_DYNAMICQUEUEABLEOBJECTTESTCHILD_H

// SYSTEM  INCLUDES
#include <functional>

// C++ PROJECT INCLUDES
#include "Async/QueueableObject.h"

namespace Async
{
namespace Tests
{

    class DynamicQueueableObjectTestChild : public QueueableObject
    {
    public:

        DynamicQueueableObjectTestChild(std::function<void()> pFunc);

        ~DynamicQueueableObjectTestChild();

        virtual bool Queue(Concurrency::WorkerThread* pThread) override;

        virtual void Load(IValueHolder* pPromise) override;

        virtual void WaitForExecution() override;

        virtual std::exception_ptr GetError() const override;

        bool GetVal();

        const int GetChildRefCount();

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

#endif // end of ASYNC_TESTS_DYNAMICQUEUEABLEOBJECTTESTCHILD_H
