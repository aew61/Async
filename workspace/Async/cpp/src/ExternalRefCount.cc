// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/ExternalRefCount.h"

namespace Async
{

    QueueableWorkItem* RefCounter::IncRef(QueueableWorkItem* pWorkItem)
    {
        if(pWorkItem)
        {
            pWorkItem->IncRef();
        }
        return pWorkItem;
    }

    void RefCounter::DecRef(QueueableWorkItem* pWorkItem)
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

    // DecRefPtr DECREF = &RefCounter::DecRef;

} // end of namespace Async
