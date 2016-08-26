// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "Async/Async.h"
#include "Async/Engine.h"
#include "Async/ExternalRefCount.h"

namespace Async
{

    std::shared_ptr<Engine>  _pEngine = nullptr;

    bool Start(const unsigned int numThreads)
    {
        if(_pEngine)
        {
            return false;
        }
        _pEngine = std::make_shared<Engine>(numThreads);
        return true;
    }

    const std::vector<std::thread::id> SpawnAdditionalThreads(const unsigned int numThreads)
    {
        if(_pEngine)
        {
            return _pEngine->SpawnWorkerThreads(numThreads);
        }
        // throw exception
        throw std::logic_error("Async is not started. Call Async::Start() first!");
    }

    void StopAdditionalThreads(const std::vector<std::thread::id> threads)
    {
        if(_pEngine)
        {
            _pEngine->ShutdownWorkerThreads(threads);
            return;
        }
        throw std::logic_error("Async is not started. Call Async::Start() first!");
    }

    const std::vector<std::pair<std::thread::id, int> > GetThreadLoadSnapshot()
    {
        if(_pEngine)
        {
            return _pEngine->GetThreadLoadSnapshot();
        }
        throw std::logic_error("Async is not started. Call Async::Start() first!");
    }

    const unsigned int NumActiveThreads()
    {
        if(_pEngine)
        {
            return _pEngine->NumThreads();
        }
        return 0;
    }

    const std::vector<std::thread::id> ActiveThreads()
    {
        if(_pEngine)
        {
            return _pEngine->GetActiveThreads();
        }
        throw std::logic_error("Async is not started. Call Async::Start() first!");
    }

    bool Queue(QueueableObject* pWorkObject, std::thread::id thread)
    {
        if(_pEngine)
        {
            return _pEngine->Queue(pWorkObject, thread);
        }
        throw std::logic_error("Async is not started. Call Async::Start() first!");
    }

    bool Stop()
    {
        if(_pEngine)
        {
            _pEngine->Shutdown();
            _pEngine = nullptr;
            return true;
        }
        return false;
    }

    QueueableObject* COPY(QueueableObject* pObj)
    {
        return RefCounter::IncRef(pObj);
    }

    void DECREF(QueueableObject** pObj)
    {
        if(*pObj)
        {
            if(RefCounter::DecRef(*pObj) == 0)
            {
                // delete pWorkObject
                if(_pEngine)
                {
                    _pEngine->GarbageCollect(*pObj);
                    return;
                }
                throw std::logic_error("Async was either: never started or stopped before all objects could be collected.");
            }
        }
        *pObj = nullptr;
    }

} // end of namespace Async
