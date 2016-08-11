// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "SharedMemory.h"

namespace Async
{
namespace Tests
{

    SharedMemory::SharedMemory() : _vec(), _mutex(), _count(0),
        _gc(), _wc(&this->_gc)
    {
    }

    SharedMemory::~SharedMemory()
    {
        this->_wc.Join();
        this->_gc.Join();
    }

} // end of namespace Tests
} // end of namespace Async
