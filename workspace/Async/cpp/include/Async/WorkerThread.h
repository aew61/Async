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
#include "Async/IThread.h"

namespace Async
{
namespace Concurrency
{

    class WorkerThread : public IThread
    {
    public:

        WorkerThread();

        ~WorkerThread();

        virtual std::thread::id GetId() override;

        virtual States::ConcurrencyState GetState() override;

        virtual Types::Result_t Queue(IExecutableWorkItem* pWorkItem) override;

        virtual void Stop() override;

        virtual void Join() override;

        virtual void Run() override;

    private:

        bool IsQueueEmpty();

    private:

        std::thread                             _thread;
        std::condition_variable                 _threadCV;
        std::atomic<States::ConcurrencyState>   _state;
        std::atomic<bool>                       _run;
        std::mutex                              _queueMutex;
        std::queue<IExecutableWorkItem*>        _queue;

    };

} // end of namespace Concurrency
} // end of namespace Async

#endif // end of ASYNC_CONCURRENCY_WORKERTHREAD_H