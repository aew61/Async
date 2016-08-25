#pragma once
#ifndef ASYNC_CONCURRENCY_WORKERTHREAD_H
#define ASYNC_CONCURRENCY_WORKERTHREAD_H

// SYSTEM INCLUDES
#include <atomic>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <thread>

// C++ PROJECT INCLUDES
#include "Async/Interfaces/IThread.h"
#include "Async/Interfaces/IGarbageCollector.h"

namespace Async
{
namespace Concurrency
{

    class WorkerThread : public IThread
    {
    public:

        WorkerThread(IGarbageCollector* pIGC);

        ~WorkerThread();

        virtual std::thread::id GetId() override;

        virtual States::ConcurrencyState GetState() override;

        // virtual Types::Result_t Queue(IExecutableWorkItem* pWorkItem) override;
        virtual bool Queue(QueueableObject* pWorkItem) override;

        virtual void Stop() override;

        virtual void Join() override;

        virtual void Run() override;

        virtual int NumWaitingJobs() override;

    private:

        bool IsQueueEmpty();

    private:

        std::thread                             _thread;
        std::condition_variable                 _threadCV;
        std::atomic<States::ConcurrencyState>   _state;
        std::atomic<bool>                       _run;
        std::mutex                              _queueMutex;
        // std::queue<IExecutableWorkItem*>        _queue;
        std::queue<QueueableObject*>            _queue;
        IGarbageCollector*                      _pIGC;

    };

} // end of namespace Concurrency
} // end of namespace Async

#endif // end of ASYNC_CONCURRENCY_WORKERTHREAD_H