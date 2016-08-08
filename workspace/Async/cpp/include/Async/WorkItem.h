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
    public:

        WorkItem();

        ~WorkItem();

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

    private:

        FunctionPtr GetCleanupFunction();

        virtual bool IsDone() override;

    private:

        FunctionPtr                 _pMainFunction;
        FunctionPtr                 _pCleanupFunction;
        std::exception_ptr          _pException;
        States::WorkItemState       _innerState;
    };

} // end of namespace Async

#endif // end of ASYNC_WORKITEM_H
