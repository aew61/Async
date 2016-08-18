#pragma once
#ifndef ASYNC_QUEUEABLEOBJECT_H
#define ASYNC_QUEUEABLEOBJECT_H

// SYSTEM INCLUDES
#include <mutex>

// C++ PROJECT INCLUDES
#include "Async/Interfaces/IRefCountedObject.h"

namespace Async
{

namespace Concurrency
{
    class WorkerThread;
} // end of namespace Concurrency

    class RefCounter;

    class QueueableObject : public IRefCountedObject
    {
        friend class Concurrency::WorkerThread;
        friend class RefCounter;
        friend class GarbageCollector;
    public:

        QueueableObject();

        virtual bool Queue(Concurrency::WorkerThread* pThread) = 0;

    protected:

        virtual void Execute() = 0;

        virtual ~QueueableObject();

        virtual int DecRef() final override;

        virtual int IncRef() final override;

        virtual const int GetRefCount() final override;

    private:

        virtual bool IsDone() = 0;

    private:

        std::mutex          _refCountMutex;
        int                 _refCount;
        

    };

} // end of namespace Async

#endif // end of ASYNC_QUEUEABLEOBJECT_H
