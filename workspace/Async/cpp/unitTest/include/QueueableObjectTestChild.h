#pragma once
#ifndef ASYNC_TESTS_QUEUEABLEOBJECTTESTCHILD_H
#define ASYNC_TESTS_QUEUEABLEOBJECTTESTCHILD_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/QueueableObject.h"

namespace Async
{
namespace Tests
{

    class QueueableObjectTestChild : public QueueableObject
    {
    public:

        QueueableObjectTestChild();

        ~QueueableObjectTestChild();

        virtual bool Queue(Concurrency::WorkerThread* pThread) override;

        virtual void Load(IValueHolder* pPromise) override;

        virtual void WaitForExecution() override;

        virtual std::exception_ptr GetError() const override;

        void ExecuteInterface();

        const int GetChildRefCount();

        bool GetVal();

    protected:

        virtual void Execute() override;

    private:

        virtual bool IsDone() override;

    private:

        bool _toSet;

    };

} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_TESTS_QUEUEABLEOBJECTTESTCHILD_H
