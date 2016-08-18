#pragma once
#ifndef ASYNC_GARBAGECOLLECTOR_H
#define ASYNC_GARBAGECOLLECTOR_H

// SYSTEM INCLUDES
#include <atomic>
#include <condition_variable>
#include <queue>
#include <memory>
#include <mutex>

// C++ PROJECT INCLUDES
#include "Async/Interfaces/IGarbageCollector.h"

namespace Async
{

    class GarbageCollector : public IGarbageCollector,
        public std::enable_shared_from_this<GarbageCollector>
    {
    public:

        GarbageCollector();

        ~GarbageCollector();

        virtual std::thread::id GetId() override;

        // virtual void Queue(IExecutableWorkItem* pWorkItem) override;
        virtual void Queue(QueueableObject* pWorkItem) override;

        virtual void Join() override;

        virtual void Run() override;

    private:

        bool IsQueueEmpty();

    private:

        std::thread                                 _thread;
        std::condition_variable                     _threadCV;
        std::atomic<bool>                           _run;
        std::mutex                                  _queueMutex;
        // std::queue<IExecutableWorkItem*>            _queue;
        std::queue<QueueableObject*>                _queue;

    };

} // end of namespace Async

#endif // end of ASYNC_GARBAGECOLLECTOR_H
