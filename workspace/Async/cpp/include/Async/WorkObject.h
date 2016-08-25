#pragma once
#ifndef ASYNC_WORKOBJECT_H
#define ASYNC_WORKOBJECT_H

// SYSTEM INCLUDES
#include <atomic>
#include <memory>
#include <mutex>
#include <queue>

// C++ PROJECT INCLUDES
#include "Async/QueueableObject.h"
#include "Async/Interfaces/IContinuableObject.h"
#include "Async/Detail/Semaphore.h"

namespace Async
{

    class WorkObject : public QueueableObject, public IContinuableObject
    {
    public:

        WorkObject();

        virtual ~WorkObject();

        virtual bool Queue(Concurrency::WorkerThread* pThread) override;

        virtual void Load(IValueHolder* pPromise) override;

        bool AddContinuation(WorkObject* pChild,
                             bool onSuccess=true) override;

        virtual void WaitForExecution() override;

        virtual std::exception_ptr GetError() const override;

    protected:

        virtual void Execute() override;

    private:

        virtual bool IsDone() override;

    private:

        std::queue<WorkObject*>                 _onSuccess, _onFailure;
        //Detail::Semaphore                       _sem;
        std::condition_variable                 _cv;
        IValueHolder*                           _pHolder;
        std::atomic<bool>                       _queued, _done;
        std::exception_ptr                      _pException;
        std::mutex                              _continuationMutex, _cvMutex;

    };

} // end of namespace Async

#endif // end of ASYNC_WORKOBJECT_H
