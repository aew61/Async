#pragma once
#ifndef ASYNC_EXTERNALREFCOUNT_H
#define ASYNC_EXTERNALREFCOUNT_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/LibraryExport.h"
#include "Async/Interfaces/IRefCountedObject.h"
#include "Async/QueueableObject.h"

namespace Async
{
    class RefCounter
    {
    public:

        static QueueableObject* IncRef(QueueableObject* pWorkItem);

        static void DecRef(QueueableObject* pWorkItem);

    private:

        RefCounter();

        ~RefCounter();

    };

    ASYNC_API QueueableObject* COPY(IRefCountedObject* pObj);

    ASYNC_API void DECREF(IRefCountedObject*& pObj);

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
