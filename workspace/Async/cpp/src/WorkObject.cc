// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/WorkObject.h"
#include "Async/WorkerThread.h"
#include "Async/ExternalRefCount.h"

namespace Async
{

    WorkObject::WorkObject() : _onSuccess(),
        _onFailure(), _sem(0), _pHolder(nullptr),
        _done{false, false}, _pException(nullptr), _doneMutex()
    {
    }

    WorkObject::~WorkObject()
    {
        this->_pException = nullptr;
        delete this->_pHolder;

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

        if(this->_done[0])
        {
            return false;
        }

        return pThread->Queue(this);
    }

    void WorkObject::Load(IValueHolder* pHolder)
    {
        this->_pHolder = pHolder;
    }

    void WorkObject::AddContinuation(WorkObject* pChild, bool onSuccess)
    {
        std::lock_guard<std::mutex> lock(this->_doneMutex);
        if(onSuccess)
        {
            this->_onSuccess.push(pChild);
        }
        else
        {
            this->_onFailure.push(pChild);
        }
    }

    void WorkObject::WaitForExecution()
    {
        this->_sem.Wait();
    }

    std::exception_ptr WorkObject::GetError() const
    {
        return this->_pException;
    }

    void WorkObject::Execute()
    {
        try
        {
            this->_done[0] = this->_pHolder->Call();
        }
        catch(...)
        {
            this->_pException = std::current_exception();
        }

        if(this->_done[0])
        {
            std::lock_guard<std::mutex> lock(this->_doneMutex);
            this->_done[1] = true;
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
            this->_sem.SignalAll();
        }
    }

    bool WorkObject::IsDone()
    {
        std::lock_guard<std::mutex> lock(this->_doneMutex);
        return this->_done[1];
    }

} // end of namespace Async
