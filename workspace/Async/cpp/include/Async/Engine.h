#pragma once
#ifndef ASYNC_ENGINE_H
#define ASYNC_ENGINE_H

// SYSTEM INCLUDES
#include <functional>
#include <map>
#include <mutex>
#include <utility>
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/GarbageCollector.h"
#include "Async/WorkerThread.h"
#include "Async/WorkObject.h"
#include "Async/Detail/Semaphore.h"

namespace Async
{

    class Engine
    {
    public:

        Engine(const unsigned int defaultNumThreads);

        ~Engine();

        void Shutdown();

        int NumThreads();

        // this is a write operation
        const std::vector<std::thread::id> SpawnWorkerThreads(const unsigned int numThreads);

        // this is a write operation
        void ShutdownWorkerThreads(const std::vector<std::thread::id>& threads);

        // this is a read operation
        std::vector<std::pair<std::thread::id, int> > GetThreadLoadSnapshot();

        void QuickSort(std::vector<std::pair<std::thread::id, int> >& snapshotRef, int left, int right);

        bool Queue(WorkObject* pWorkItem, std::thread::id threadId);

    protected:

        int QuickSortPartition(std::vector<std::pair<std::thread::id, int> >& snapshotRef, int left, int right);

        void BeginReadOperation();

        void EndReadOperation();

        void BeginWriteOperation();

        void EndWriteOperation();

    private:

        GarbageCollector                                        _gc;
        std::map<std::thread::id, Concurrency::WorkerThread*>   _workerThreads;
        int                                                     _readCount, _writeCount;
        std::mutex                                              _rMutex, _wMutex;
        Detail::Semaphore                                       _readTry, _resource;
        bool                                                    _running;

    };

} // end of namespace Async

#endif // end of ASYNC_ENGINE_H
