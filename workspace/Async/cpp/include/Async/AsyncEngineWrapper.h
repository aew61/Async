#pragma once
#ifndef ASYNC_ASYNC_H
#define ASYNC_ASYNC_H

// SYSTEM INCLUDES
#include <thread>
#include <utility>
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.h"
#include "Async/Interfaces/IRefCountedObject.h"

namespace Async
{


    ASYNC_API bool Start(const unsigned int numThreads);

    ASYNC_API const std::vector<std::thread::id> SpawnAdditionalThreads(const unsigned int numThreads);

    ASYNC_API void StopAdditionalThreads(const std::vector<std::thread::id> threads);

    ASYNC_API const std::vector<std::pair<std::thread::id, int> > GetThreadLoadSnapshot();

    ASYNC_API const unsigned int NumActiveThreads();

    ASYNC_API const std::vector<std::thread::id> ActiveThreads();

    ASYNC_API bool Queue(IRefCountedObject* pWorkObject, std::thread::id thread);

    ASYNC_API bool IsIdle();

    ASYNC_API bool Stop();

    ASYNC_API IRefCountedObject* MakeObject();

    ASYNC_API IRefCountedObject* COPY(IRefCountedObject* pObj);

    ASYNC_API void DECREF(IRefCountedObject** pObj);

} // end of namespace Async

#endif // end of ASYNC_ASYNC_H
