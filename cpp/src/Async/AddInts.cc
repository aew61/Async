// SYSTEM INCLUDES
#include <Utilities/Semaphore.h>

// C++ PROJECT INCLUDES
#include "Async/AddInts.h"

namespace Async
{

    int AddInts(int a, int b)
    {
        Utilities::Semaphore sem(1);
        return a + b;
    }

} // end of namespace Async
