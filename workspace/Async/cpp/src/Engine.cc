// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/Engine.h"

namespace Async
{

    Engine::Engine(const unsigned int defaultNumThreads) : _gc(), _workerThreads(),
        _readCount(0), _writeCount(0), _rMutex(), _wMutex(), _readTry(1), _resource(1),
        _running(true)
    {
        for(unsigned int i = 0; i < defaultNumThreads; ++i)
        {
            Concurrency::WorkerThread* pWth = new Concurrency::WorkerThread(&this->_gc);
            this->_workerThreads[pWth->GetId()] = pWth;
        }
    }

    Engine::~Engine()
    {
        if(this->_running)
        {
            this->Shutdown();
        }
    }

    void Engine::Shutdown()
    {
        if(this->_running)
        {
            this->BeginWriteOperation();
            this->_running = false;
            for(auto it = this->_workerThreads.begin(); it != this->_workerThreads.end(); ++it)
            {
                // shut down the worker thread
                it->second->Join();
                delete it->second;
                it->second = nullptr;
            }
            this->_workerThreads.clear();
            // shut down the garbage collector
            this->_gc.Join();
            this->EndWriteOperation();
        }
    }

    const unsigned int Engine::NumThreads()
    {
        this->BeginReadOperation();
        int size = this->_workerThreads.size();
        this->EndReadOperation();
        return size;
    }

    const std::vector<std::thread::id> Engine::GetActiveThreads()
    {
        this->BeginReadOperation();
        std::vector<std::thread::id> activeThreads(this->_workerThreads.size());
        int index = 0;
        for(auto it = this->_workerThreads.begin(); it != this->_workerThreads.end(); ++it)
        {
            activeThreads[index++] = it->first;
        }
        this->EndReadOperation();
        return activeThreads;
    }

    // this is a write operation
    const std::vector<std::thread::id> Engine::SpawnWorkerThreads(const unsigned int numThreads)
    {
        std::vector<std::thread::id> newThreadIds(numThreads);
        this->BeginWriteOperation();
        for(int i = 0; i < numThreads; ++i)
        {
            Concurrency::WorkerThread* pWth = new Concurrency::WorkerThread(&this->_gc);
            newThreadIds[i] = pWth->GetId();
            this->_workerThreads[pWth->GetId()] = pWth;
        }
        this->EndWriteOperation();
        return newThreadIds;
    }

    // this is a write operation
    void Engine::ShutdownWorkerThreads(const std::vector<std::thread::id>& threads)
    {
        std::vector<Concurrency::WorkerThread*> threadsToDestroy;
        this->BeginWriteOperation();
        for(std::thread::id threadId : threads)
        {
            auto it = this->_workerThreads.find(threadId);
            if(it != this->_workerThreads.end())
            {
                threadsToDestroy.push_back(std::move(this->_workerThreads[threadId]));
                this->_workerThreads.erase(it);
            }
            //else
            //{
                // handle error case?
            //}
        }
        this->EndWriteOperation();

        // since the threads are now out of the map, no reason to block other threads
        for(Concurrency::WorkerThread* pWth : threadsToDestroy)
        {
            pWth->Stop();
        }
        for(Concurrency::WorkerThread* pWth : threadsToDestroy)
        {
            pWth->Join();
            delete pWth;
            pWth = nullptr;
        }
    }

    // this is a read operation
    std::vector<std::pair<std::thread::id, int> > Engine::GetThreadLoadSnapshot()
    {
        int index = 0;
        this->BeginReadOperation();
        std::vector<std::pair<std::thread::id, int> > snapshot(this->_workerThreads.size());
        for(auto it = this->_workerThreads.begin(); it != this->_workerThreads.end(); ++it)
        {
            snapshot[index] = std::make_pair(it->first, it->second->NumWaitingJobs());
            ++index;
        }
        this->EndReadOperation();
        return snapshot;
    }

    void Engine::QuickSort(std::vector<std::pair<std::thread::id, int> >& snapshotRef, int left, int right)
    {
        if(left < right)
        {
            int partition = this->QuickSortPartition(snapshotRef, left, right);
            this->QuickSort(snapshotRef, left, partition);
            this->QuickSort(snapshotRef, partition + 1, right);
        }
    }

    bool Engine::Queue(QueueableObject* pWorkItem, std::thread::id threadId)
    {
        bool result = false;
        this->BeginReadOperation();
        auto it = this->_workerThreads.find(threadId);
        if(it != this->_workerThreads.end())
        {
            result = pWorkItem->Queue(it->second);
        }
        // else
        // {
            // handle error case
        // }
        this->EndReadOperation();
        return result;
    }

    void Engine::GarbageCollect(QueueableObject* pWorkItem)
    {
        this->_gc.Queue(pWorkItem);
    }

    int Engine::QuickSortPartition(std::vector<std::pair<std::thread::id, int> >& snapshotRef, int left, int right)
    {
        int lower = snapshotRef[left].second;
        int upper = snapshotRef[right].second;
        int middle = snapshotRef[(left + right) / 2].second;
        if(middle < lower) { lower = middle; }
        if(middle > upper) { upper = middle; }
        int pivot = (middle + upper) / 2;
        int i = left - 1;
        int j = right + 1;
        while(true)
        {
            do { ++i; }
            while(snapshotRef[i].second < pivot);

            do { --j; }
            while(snapshotRef[j].second > pivot);

            if(i >= j)
            {
                return j;
            }
            std::swap(snapshotRef[i], snapshotRef[j]);
        }
    }

    void Engine::BeginReadOperation()
    {
        this->_readTry.Wait();
        std::unique_lock<std::mutex> rLock(this->_rMutex);
        if(++this->_readCount == 1)
        {
            this->_resource.Wait();
        }
        rLock.unlock();
        this->_readTry.Signal();
    }

    void Engine::EndReadOperation()
    {
        std::lock_guard<std::mutex> rLock(this->_rMutex);
        if(--this->_readCount == 0)
        {
            this->_resource.Signal();
        }
    }

    void Engine::BeginWriteOperation()
    {
        std::unique_lock<std::mutex> wLock(this->_wMutex);
        if(++this->_writeCount == 1)
        {
            this->_readTry.Wait();
        }
        wLock.unlock();
        this->_resource.Wait();
    }

    void Engine::EndWriteOperation()
    {
        this->_resource.Signal();
        std::lock_guard<std::mutex> wLock(this->_wMutex);
        if(--this->_writeCount == 0)
        {
            this->_readTry.Signal();
        }
    }

} // end of namespace Async
