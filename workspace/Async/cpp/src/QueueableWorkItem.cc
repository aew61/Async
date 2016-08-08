// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/QueueableWorkItem.h"

namespace Async
{

    QueueableWorkItem::QueueableWorkItem() :
        _refCountMutex(), _refCount(1)
    {
    }

    QueueableWorkItem::~QueueableWorkItem()
    {
    }

    /**
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
    */

    int QueueableWorkItem::DecRef()
    {
        std::lock_guard<std::mutex> refCountLock(this->_refCountMutex);
        return --this->_refCount;
    }

    int QueueableWorkItem::IncRef()
    {
        std::lock_guard<std::mutex> refCountLock(this->_refCountMutex);
        return ++this->_refCount;
    }

    const int QueueableWorkItem::GetRefCount()
    {
        std::lock_guard<std::mutex> refCountLock(this->_refCountMutex);
        return this->_refCount;
    }

} // end of namespace Async
