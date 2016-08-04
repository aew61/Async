#pragma once
#ifndef ASYNC_WORKITEM_H
#define ASYNC_WORKITEM_H


// SYSTEM INCLUDES
#include <mutex>
#include <memory>

// C++ PROJECT INCLUDES
#include "Async/IExecutableWorkItem.h"
// #include "Async/PromiseBaseImpl.h"

namespace Async
{

    class WorkItem : public IExecutableWorkItem
    {
        friend class PromiseBaseImpl;
        friend class GarbageCollector;
    public:

        WorkItem(uint64_t id=0);

        ~WorkItem();

        virtual void SetId(const uint64_t) override;

        virtual const uint64_t GetId() override;

        virtual Types::Result_t Queue(Concurrency::WorkerThread* pThread) override;

        virtual std::exception_ptr GetException() const override;

        const std::string GetStateAsString();

    protected:

        virtual void AttachMainFunction(FunctionPtr pFunc) override;

        virtual void AttachCleanupFunction(FunctionPtr pFunc) override;

        void SetException(const std::exception_ptr pException);

        virtual States::WorkItemState Execute() override;

        void Finish();

        States::WorkItemState GetState();

        void SetState(States::WorkItemState newState);

        virtual void DecRef() override;

    private:

        FunctionPtr GetCleanupFunction();

        virtual bool IsDone() override;

    private:

        uint64_t                    _id;
        Concurrency::WorkerThread*  _pThread;
        FunctionPtr                 _pMainFunction;
        FunctionPtr                 _pCleanupFunction;
        std::exception_ptr          _pException;
        States::WorkItemState       _innerState;
    };

} // end of namespace Async

#endif // end of ASYNC_WORKITEM_H
