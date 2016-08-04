// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/WorkItem.h"
#include "Async/WorkerThread.h"

namespace Async
{

    WorkItem::WorkItem(uint64_t id) :
            _id(id), _pThread(nullptr), _pMainFunction(nullptr),
            _pCleanupFunction(nullptr), _pException(nullptr), _innerState(States::WorkItemState::IDLE)
    {
    }

    WorkItem::~WorkItem()
    {
    }

    void WorkItem::SetId(uint64_t id)
	{
		this->_id = id;
	}

    const uint64_t WorkItem::GetId()
	{
		return this->_id;
	}

    Types::Result_t WorkItem::Queue(Concurrency::WorkerThread* pThread)
	{

		Types::Result_t result = Types::Result_t::FAILURE;

        if(!pThread || (this->_innerState != States::WorkItemState::IDLE &&
           this->_innerState != States::WorkItemState::REQUEUE))
        {
            return result;
        }

        this->_innerState = States::WorkItemState::QUEUE;
		return pThread->Queue(this);
	}

    std::exception_ptr WorkItem::GetException() const
	{
		return this->_pException;
	}

    const std::string WorkItem::GetStateAsString()
    {
        return GetWorkItemStateString(this->_innerState);
    }

    void WorkItem::AttachMainFunction(FunctionPtr func)
	{
		this->_pMainFunction = func;
	}

	void WorkItem::AttachCleanupFunction(FunctionPtr func)
	{
		this->_pCleanupFunction = func;
	}

    void WorkItem::SetException(const std::exception_ptr pException)
	{
		this->_pException = pException;
	}

    States::WorkItemState WorkItem::Execute()
    {
        States::WorkItemState nextState = States::WorkItemState::DONE;
        if (this->_pMainFunction)
        {
            try
            {
                nextState = this->_pMainFunction();
            }
            catch (...)
            {
                this->SetException(std::current_exception());
            }
        }
        else
        {
            throw std::logic_error("No function to execute!");
        }

        FunctionPtr pPost = this->GetCleanupFunction();
        if (nextState == States::WorkItemState::DONE && pPost && !this->_pException)
        {
            try
            {
                nextState = pPost();
            }
            catch (...)
            {
                this->SetException(std::current_exception());
            }
        }

        this->_innerState = nextState;
        return nextState;
	}

    void WorkItem::Finish()
	{
        this->_pMainFunction = nullptr;
		this->_pCleanupFunction = nullptr;
	}

    States::WorkItemState WorkItem::GetState()
    {
        return this->_innerState;
    }

    void WorkItem::SetState(States::WorkItemState newState)
    {
        this->_innerState = newState;
    }

    void WorkItem::DecRef()
    {
    }

    FunctionPtr WorkItem::GetCleanupFunction()
    {
        if(this->_pCleanupFunction)
        {
            return this->_pCleanupFunction;
        }
        return []() -> States::WorkItemState
        {
            return States::WorkItemState::DONE;
        };
    }

    bool WorkItem::IsDone()
	{
		return this->_innerState == States::WorkItemState::DONE;
	}

} // end of namespace Async
