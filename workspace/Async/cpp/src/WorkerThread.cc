// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/WorkerThread.h"

namespace Async
{
namespace Concurrency
{

    WorkerThread::WorkerThread() : _thread(), _threadCV(),
        _state(States::ConcurrencyState::IDLE), _run(true),
        _queueMutex(), _queue()
    {
    }

    WorkerThread::~WorkerThread()
    {
        if(this->_run)
        {
            this->Join();
        }
    }

    std::thread::id WorkerThread::GetId()
    {
        return this->_thread.get_id();
    }

    States::ConcurrencyState WorkerThread::GetState()
    {
        return this->_state;
    }

    Types::Result_t WorkerThread::Queue(IExecutableWorkItem* pWorkItem)
    {
        // modifying queue so need to aquire lock on it
        std::unique_lock<std::mutex> queueLock(this->_queueMutex);
        this->_queue.push(pWorkItem);

        // wake up the spawned thread if it is sleeping.
        this->_threadCV.notify_one();
        queueLock.unlock();

        return Types::Result_t::SUCCESS;
    }

    void WorkerThread::Stop()
    {
        this->_run = false;
    }

    void WorkerThread::Join()
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
        this->_state = States::ConcurrencyState::DONE;
        IExecutableWorkItem* pWorkItem = nullptr;
        while(this->_queue.empty())
        {
            pWorkItem = this->_queue.front();
            this->_queue.pop();
            pWorkItem->DecRef();
        }
        pWorkItem = nullptr;
        // std::cout << "WorkerThread shut down" << std::endl;

    }

    // this is the method that the concurrent thread (this->_thread) will run.
    // it will try and get workItems from the queue when there are stuff
    // to be had, otherwise it will go to sleep to be woken when the
    // queue is populated.
    void WorkerThread::Run()
    {
        // the work item we will be executing
        IExecutableWorkItem* pWorkItem = nullptr;

        while (this->_run)
        {
            std::unique_lock<std::mutex> queueLock(this->_queueMutex);
            while (this->_run && !this->_queue.empty())
            {

                // this is atomic
                this->_state = States::ConcurrencyState::BUSY;

                // lock the queue and retrieve the WorkItem.
                // this is in its own code block because the lock
                // is released only when the object destructs.
                pWorkItem = this->_queue.front();
                this->_queue.pop();
                queueLock.unlock();

                // execute the WorkItem
                // DON'T BLOCK HERE....THIS COULD BE VERY EXPENSIVE
                // BOTH RUNTIME AND RESOURCE WISE
                pWorkItem->Execute();

                queueLock.lock();
                // reschedule if necessary
                if (!pWorkItem->IsDone())
                {
                    this->_queue.push(pWorkItem);
                }
                else
                {
                    pWorkItem->DecRef();
                }
            }
            this->_state = States::ConcurrencyState::IDLE;


            // lock on this thread
            // put the concurrent thread to sleep until it is woken up by
            // a WorkItem being queued.
            if (this->_run)
            {
                this->_threadCV.wait(queueLock);
            }
        }
    }

    bool WorkerThread::IsQueueEmpty()
    {
        std::lock_guard<std::mutex> queueLock(this->_queueMutex);
        return this->_queue.empty();
    }

} // end of namespace Concurrency
} // end of namespace Async
