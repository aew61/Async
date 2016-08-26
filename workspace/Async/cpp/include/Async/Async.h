#pragma once
#ifndef ASYNC_ASYNC_H
#define ASYNC_ASYNC_H

// SYSTEM INCLUDES
#include <thread>
#include <utility>
#include <vector>

// C++ PROJECT INCLUDES
#include "Async/LibraryExport.h"
#include "Async/QueueableObject.h"

namespace Async
{


    ASYNC_API bool Start(const unsigned int numThreads);

    ASYNC_API const std::vector<std::thread::id> SpawnAdditionalThreads(const unsigned int numThreads);

    ASYNC_API void StopAdditionalThreads(const std::vector<std::thread::id> threads);

    ASYNC_API const std::vector<std::pair<std::thread::id, int> > GetThreadLoadSnapshot();

    ASYNC_API const unsigned int NumActiveThreads();

    ASYNC_API const std::vector<std::thread::id> ActiveThreads();

    ASYNC_API bool Queue(QueueableObject* pWorkObject, std::thread::id thread);

    ASYNC_API bool Stop();

    ASYNC_API QueueableObject* COPY(QueueableObject* pObj);

    ASYNC_API void DECREF(QueueableObject** pObj);

} // end of namespace Async

#endif // end of ASYNC_ASYNC_H
