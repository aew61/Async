#pragma once
#ifndef ASYNC_TESTS_GARBAGECOLLECTOR_H
#define ASYNC_TESTS_GARBAGECOLLECTOR_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "Async/Interfaces/IGarbageCollector.h"

namespace Async
{
namespace Tests
{

    class GarbageCollector_noDealloc : public IGarbageCollector
    {
    public:

        GarbageCollector_noDealloc();

        ~GarbageCollector_noDealloc();

        virtual std::thread::id GetId() override;

        virtual void Queue(QueueableObject* pWorkItem) override;

        virtual void Join() override;

        virtual void Run() override;

    };

} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_TESTS_GARBAGECOLLECTOR_H
