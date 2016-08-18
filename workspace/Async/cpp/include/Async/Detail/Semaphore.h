#pragma once
#ifndef ASYNC_SEMAPHORE_H
#define ASYNC_SEMAPHORE_H

// SYSTEM INCLUDES
#include <atomic>
#include <mutex>
#include <condition_variable>

// C++ PROJECT INCLUDES

namespace Async
{
namespace Detail
{

    class Semaphore
    {
    public:

        Semaphore(int count);

        ~Semaphore();

        void Wait();

        void Signal();

        void SignalAll();

        int GetCount();

    private:

        std::atomic<int>        _count;
        std::atomic<bool>       _trigger;
        std::condition_variable _cv;
        std::mutex              _mutex;

    };

} // end of namespace Detail
} // end of namespace Async

#endif // end of ASYNC_SEMAPHORE_H