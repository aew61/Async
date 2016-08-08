#pragma once
#ifndef ASYNC_ABSTRACTREFCOUNTEDEXECUTABLEWORKITEM_H
#define ASYNC_ABSTRACTREFCOUNTEDEXECUTABLEWORKITEM_H

// SYSTEM INCLUDES
#include <mutex>

// C++ PROJECT INCLUDES
#include "Async/Result.h"

namespace Async
{

namespace Concurrency
{
    class WorkerThread;
} // end of namespace Concurrency

    class RefCounter;

    class QueueableWorkItem
    {
        friend class Concurrency::WorkerThread;
        friend class RefCounter;
        friend class GarbageCollector;
    public:

        QueueableWorkItem();

        virtual Types::Result_t Queue(Concurrency::WorkerThread* pThread) = 0;

    protected:

        virtual void Execute() = 0;

        virtual ~QueueableWorkItem();

        int DecRef();

        int IncRef();

        const int GetRefCount();

    private:

        virtual bool IsDone() = 0;

    private:

        std::mutex          _refCountMutex;
        int                 _refCount;
        

    };

} // end of namespace Async

#endif // end of ASYNC_ABSTRACTREFCOUNTEDEXECUTABLEWORKITEM_H
