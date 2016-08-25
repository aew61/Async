// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/WorkObject.h"
#include "Async/WorkerThread.h"
#include "Async/ExternalRefCount.h"

namespace Async
{

    WorkObject::WorkObject() : _onSuccess(),
        _onFailure(), _cv(), _pHolder(nullptr), _queued(false),
        _done(false), _pException(nullptr), _continuationMutex(), _cvMutex()
    {
    }

    WorkObject::~WorkObject()
    {
        this->_pException = nullptr;
        if(this->_pHolder)
        {
            this->_pHolder->Destroy();
        }

        WorkObject* pSuccessor = nullptr;

        // this case will occur if and only if continuations have not been queued when Async
        // is stopped
        while(!this->_onSuccess.empty())
        {
            pSuccessor = this->_onSuccess.front();
            this->_onSuccess.pop();
            // decrement pSuccessor
            if(pSuccessor->DecRef() == 0)
            {
                // queue pSuccessor on garbage collector (using dll side utility function)
            }
        }

        while(!this->_onFailure.empty())
        {
            pSuccessor = this->_onSuccess.front();
            this->_onSuccess.pop();
            // decrement pSuccessor
            if(pSuccessor->DecRef() == 0)
            {
                // queue pSuccessor on garbage collector (using dll side utility function)
            }
        }
    }

    bool WorkObject::Queue(Concurrency::WorkerThread* pThread)
    {

        if(this->_queued)
        {
            return false;
        }

        this->_queued = pThread->Queue(this);
        return this->_queued;
    }

    void WorkObject::Load(IValueHolder* pHolder)
    {
        this->_pHolder = pHolder;
    }

    bool WorkObject::AddContinuation(WorkObject* pChild, bool onSuccess)
    {
        //std::lock_guard<std::mutex> lock(this->_doneMutex);
        if(!this->_done)
        {
            std::lock_guard<std::mutex> lock(this->_continuationMutex);
            if(onSuccess)
            {
                this->_onSuccess.push(pChild);
            }
            else
            {
                this->_onFailure.push(pChild);
            }
            return true;
        }
        return false;
    }

    void WorkObject::WaitForExecution()
    {
        std::unique_lock<std::mutex> lock(this->_cvMutex);
        while(!this->_done)
        {
            this->_cv.wait(lock);
        }
    }

    std::exception_ptr WorkObject::GetError() const
    {
        return this->_pException;
    }

    void WorkObject::Execute()
    {
        try
        {
            this->_done = this->_pHolder->Call();
        }
        catch(...)
        {
            this->_pException = std::current_exception();
        }

        if(this->_done)
        {
            std::unique_lock<std::mutex> cvLock(this->_cvMutex);
            this->_cv.notify_all();
            cvLock.unlock();

            std::lock_guard<std::mutex> lock(this->_continuationMutex);
            std::queue<WorkObject*>& queue = this->_onSuccess;
            if(this->GetError())
            {
                queue = this->_onFailure;
            }

            WorkObject* pSuccessor = nullptr;
            while(!queue.empty())
            {
                pSuccessor = queue.front();
                queue.pop();

                // queue pSuccessor (use a dll side utility)
                // do NOT call IncRef or DecRef, this instance
                // is holding the RefCount that a WorkerThread will "inherit"
                // when the WorkObject* is queued. If that does not happen (Async is stopped)
                // BEFORE they can be queued, the destructor of this instance will do all
                // DecRefs and schedule on the garbage collector if necessary.
            }
        }
    }

    bool WorkObject::IsDone()
    {
        return this->_done;
    }

} // end of namespace Async
