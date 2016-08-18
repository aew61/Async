// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/QueueableObject.h"

namespace Async
{

    QueueableObject::QueueableObject() :
        _refCountMutex(), _refCount(1)
    {
    }

    QueueableObject::~QueueableObject()
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

    int QueueableObject::DecRef()
    {
        std::lock_guard<std::mutex> refCountLock(this->_refCountMutex);
        return --this->_refCount;
    }

    int QueueableObject::IncRef()
    {
        std::lock_guard<std::mutex> refCountLock(this->_refCountMutex);
        return ++this->_refCount;
    }

    const int QueueableObject::GetRefCount()
    {
        std::lock_guard<std::mutex> refCountLock(this->_refCountMutex);
        return this->_refCount;
    }

} // end of namespace Async
