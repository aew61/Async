// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "GarbageCollector_noDealloc.h"

namespace Async
{
namespace Tests
{

    GarbageCollector_noDealloc::GarbageCollector_noDealloc()
    {
    }

    GarbageCollector_noDealloc::~GarbageCollector_noDealloc()
    {
    }

    std::thread::id GarbageCollector_noDealloc::GetId()
    {
        std::thread th;
        return th.get_id();
    }

    void GarbageCollector_noDealloc::Queue(QueueableWorkItem* pWorkItem)
    {
    }

    void GarbageCollector_noDealloc::Join()
    {
    }

    void GarbageCollector_noDealloc::Run()
    {
    }

} // end of namespace Tests
} // end of namespace Async
