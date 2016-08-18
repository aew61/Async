// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/WorkObject.h"
#include "Async/WorkerThread.h"

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
            this->_onSuccess.push_back(pChild);
        }
        else
        {
            this->_onFailure.push_back(pChild);
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
            std::vector<WorkObject*>& vec = this->_onSuccess;
            if(this->GetError())
            {
                vec = this->_onFailure;
            }

            for(WorkObject* successor : vec)
            {
                // queue successor (using a utility function)
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
