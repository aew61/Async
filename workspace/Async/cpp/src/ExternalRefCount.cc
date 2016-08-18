// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/ExternalRefCount.h"

namespace Async
{

    QueueableObject* RefCounter::IncRef(QueueableObject* pWorkItem)
    {
        if(pWorkItem)
        {
            pWorkItem->IncRef();
        }
        return pWorkItem;
    }

    void RefCounter::DecRef(QueueableObject* pWorkItem)
    {
        if(pWorkItem)
        {
            if(pWorkItem->DecRef() == 0)
            {
                // delete object
            }
        }
    }

    RefCounter::RefCounter()
    {
    }

    RefCounter::~RefCounter()
    {
    }

    QueueableObject* COPY(IRefCountedObject* pObj)
    {
        return RefCounter::IncRef(dynamic_cast<QueueableObject*>(pObj));
    }

    void DECREF(IRefCountedObject*& pObj)
    {
        RefCounter::DecRef(dynamic_cast<QueueableObject*>(pObj));
        pObj = nullptr;
    }

    // DecRefPtr DECREF = &RefCounter::DecRef;

} // end of namespace Async
