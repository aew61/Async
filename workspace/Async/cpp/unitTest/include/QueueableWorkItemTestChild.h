#pragma once
#ifndef ASYNC_TESTS_QUEUEABLEWORKITEMTESTCHILD_H
#define ASYNC_TESTS_QUEUEABLEWORKITEMTESTCHILD_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/QueueableWorkItem.h"

namespace Async
{
namespace Tests
{

    class QueueableWorkItemTestChild : public QueueableWorkItem
    {
    public:

        QueueableWorkItemTestChild();

        ~QueueableWorkItemTestChild();

        virtual Types::Result_t Queue(Concurrency::WorkerThread* pThread) override;

        void ExecuteInterface();

        int GetRefCount();

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

#endif // end of ASYNC_TESTS_QUEUEABLEWORKITEMTESTCHILD_H
