#pragma once
#ifndef ASYNC_EXTERNALREFCOUNT_H
#define ASYNC_EXTERNALREFCOUNT_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/LibraryExport.h"
#include "Async/QueueableWorkItem.h"

namespace Async
{
    class RefCounter
    {
    public:

        static QueueableWorkItem* IncRef(QueueableWorkItem* pWorkItem);

        static void DecRef(QueueableWorkItem* pWorkItem);

    private:

        RefCounter();

        ~RefCounter();

    };

    template<typename T>
    ASYNC_API T* COPY(T*& pWorkItem)
    {
        return std::dynamic_cast<T*>(std::dynamic_cast<QueueableWorkItem*>(RefCounter::IncRef(pWorkItem)));
    }

    template<typename T>
    ASYNC_API void DECREF(T*& pWorkItem)
    {
        RefCounter::DecRef(std::dynamic_cast<QueueableWorkItem*>(pWorkItem));
        pWorkItem = nullptr;
    }

    // typedef void (*DecRefPtr)(QueueableWorkItem*);

    // extern DecRefPtr DECREF;

    // template specializations
    template<>
    ASYNC_API QueueableWorkItem* COPY<QueueableWorkItem>(QueueableWorkItem*& pWorkItem)
    {
        return RefCounter::IncRef(pWorkItem);
    }

    template<>
    ASYNC_API void DECREF<QueueableWorkItem>(QueueableWorkItem*& pWorkItem)
    {
        RefCounter::DecRef(pWorkItem);
        pWorkItem = nullptr;
    }

}

#endif // end of ASYNC_EXTERNALREFCOUNT_H
