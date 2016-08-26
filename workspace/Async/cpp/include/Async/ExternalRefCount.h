#pragma once
#ifndef ASYNC_EXTERNALREFCOUNT_H
#define ASYNC_EXTERNALREFCOUNT_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/QueueableObject.h"

namespace Async
{
    class RefCounter
    {
    public:

        static QueueableObject* IncRef(QueueableObject* pWorkItem);

        static int DecRef(QueueableObject* pWorkItem);

    private:

        RefCounter();

        ~RefCounter();

    };

    //template<typename T>
    //ASYNC_API Promise<T>* COPY(Promise<T>*& pWorkItem)
    //{
    //    return dynamic_cast<Promise<T>*>(dynamic_cast<QueueableWorkItem*>(IncRef(pWorkItem)));
    //}

    //template<typename T>
    //ASYNC_API void DECREF(Promise<T>*& pWorkItem)
    //{
    //    DecRef(dynamic_cast<QueueableWorkItem*>(pWorkItem));
    //    pWorkItem = nullptr;
    //}

    // typedef void (*DecRefPtr)(QueueableWorkItem*);

    // extern DecRefPtr DECREF;

    // template specializations

}

#endif // end of ASYNC_EXTERNALREFCOUNT_H
