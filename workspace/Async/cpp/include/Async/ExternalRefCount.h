#pragma once
#ifndef ASYNC_EXTERNALREFCOUNT_H
#define ASYNC_EXTERNALREFCOUNT_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/LibraryExport.h"

namespace Async
{

    template<typename T>
    ASYNC_API void DECREF(T* ptr)
    {
        if(ptr)
        {
            if(ptr->DecRef() == 0)
            {
                ptr = nullptr;
            }
        }
    }

    template<typename T>
    ASYNC_API T* INCREF(T* ptr)
    {
        if(ptr)
        {
            ptr->IncRef();
        }
        return ptr;
    }

    // template specializations

}

#endif // end of ASYNC_EXTERNALREFCOUNT_H
