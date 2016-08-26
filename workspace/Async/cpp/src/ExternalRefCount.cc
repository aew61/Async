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

    int RefCounter::DecRef(QueueableObject* pWorkItem)
    {
        return pWorkItem->DecRef();
    }

    RefCounter::RefCounter()
    {
    }

    RefCounter::~RefCounter()
    {
    }

    // DecRefPtr DECREF = &RefCounter::DecRef;

} // end of namespace Async
