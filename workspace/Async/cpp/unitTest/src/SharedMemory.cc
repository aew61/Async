// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "SharedMemory.h"

namespace Async
{
namespace Tests
{

    SharedMemory::SharedMemory(Concurrency::WorkerThread* pWT) :
        _vec(), _mutex(), _count(0), _pWT(pWT)
    {
    }

    SharedMemory::~SharedMemory()
    {
    }

} // end of namespace Tests
} // end of namespace Async
