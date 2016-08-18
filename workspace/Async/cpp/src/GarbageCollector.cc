// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/GarbageCollector.h"

namespace Async
{

    GarbageCollector::GarbageCollector() : _thread(), _threadCV(),
        _run(true), _queueMutex(), _queue()
    {
        this->_thread = std::thread(&GarbageCollector::Run, this);
    }

    GarbageCollector::~GarbageCollector()
    {
        if(this->_run)
        {
            this->Join();
        }
    }

    std::thread::id GarbageCollector::GetId()
    {
        return this->_thread.get_id();
    }

    // void GarbageCollector::Queue(IExecutableWorkItem* pWorkItem)
    void GarbageCollector::Queue(QueueableObject* pWorkItem)
    {
        // modifying queue so need to aquire lock on it
        std::unique_lock<std::mutex> queueLock(this->_queueMutex);
        this->_queue.push(pWorkItem);

        // wake up the spawned thread if it is sleeping.
        this->_threadCV.notify_one();
        queueLock.unlock();
    }

    void GarbageCollector::Join()
    {
        this->_run = false;

        // synchronization! This prevents a algorithmic hole in
        // the second while loop in WorkerThread::Run
        std::unique_lock<std::mutex> queueLock(this->_queueMutex);
        // std::cout << "WorkerThread shutting down...";

        this->_threadCV.notify_all();

        // MUST unlock! Otherwise, if we try and join with the _thread,
        // the _thread is waiting for _queueMutex to be released.
        // if we don't release the lock, we will have a deadlock
        // (this thread will wait for _thread to exit, while _thread is
        // waiting for _queueMutex to be unlocked but this thread possesses
        // a lock on _queueMutex which will never be released)!
        queueLock.unlock();
        if (this->_thread.joinable())
        {
            this->_thread.join();
        }
        // std::cout << "WorkerThread shut down" << std::endl;

    }

    // this is the method that the concurrent thread (this->_thread) will run.
    // it will try and get workItems from the queue when there are stuff
    // to be had, otherwise it will go to sleep to be woken when the
    // queue is populated.
    void GarbageCollector::Run()
    {
        // the work item we will be executing
        // IExecutableWorkItem* pWorkItem = nullptr;
        QueueableObject* pWorkItem = nullptr;

        while (this->_run)
        {
            std::unique_lock<std::mutex> queueLock(this->_queueMutex);
            while (this->_run && !this->_queue.empty())
            {

                // lock the queue and retrieve the WorkItem.
                // this is in its own code block because the lock
                // is released only when the object destructs.
                pWorkItem = this->_queue.front();
                this->_queue.pop();
                queueLock.unlock();

                // delete the item
                delete pWorkItem;
                pWorkItem = nullptr;

                queueLock.lock();
            }


            // lock on this thread
            // put the concurrent thread to sleep until it is woken up by
            // a WorkItem being queued.
            if (this->_run)
            {
                this->_threadCV.wait(queueLock);
            }
        }

        // being told to shutdown
        while(!this->_queue.empty())
        {
            pWorkItem = this->_queue.front();
            this->_queue.pop();
            delete pWorkItem;
            pWorkItem = nullptr;
        }
    }

    bool GarbageCollector::IsQueueEmpty()
    {
        std::lock_guard<std::mutex> queueLock(this->_queueMutex);
        return this->_queue.empty();
    }

} // end of namespace Async
