// SYSTEM INCLUDES
#include <memory>

// C++ PROJECT INCLUDES
#include "Async/AsyncEngineWrapper.h"
#include "Async/QueueableObject.h"
#include "Async/Engine.h"
#include "Async/ExternalRefCount.h"
#include "Async/WorkObject.h"

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

    bool Queue(IRefCountedObject* pWorkObject, std::thread::id thread)
    {
        if(_pEngine)
        {
            return _pEngine->Queue(dynamic_cast<QueueableObject*>(pWorkObject), thread);
        }
        throw std::logic_error("Async is not started. Call Async::Start() first!");
    }

    bool IsIdle()
    {
        if(_pEngine)
        {
            return _pEngine->IsIdle();
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

    IRefCountedObject* MakeObject()
    {
        if(_pEngine)
        {
            return dynamic_cast<QueueableObject*>(new WorkObject());
        }
        return nullptr;
    }

    IRefCountedObject* COPY(IRefCountedObject* pObj)
    {
        return dynamic_cast<IRefCountedObject*>(RefCounter::IncRef(dynamic_cast<QueueableObject*>(pObj)));
    }

    void DECREF(IRefCountedObject** pObj)
    {
        if(pObj)
        {
            QueueableObject* pObjDest = dynamic_cast<QueueableObject*>(*pObj);
            if(*pObj)
            {
                if(RefCounter::DecRef(pObjDest) == 0)
                {
                    // delete pWorkObject
                    if(_pEngine)
                    {
                        _pEngine->GarbageCollect(pObjDest);
                    }
                    else
                    {
                        throw std::logic_error("Async was either: never started or stopped before all objects could be collected.");
                    }
                }
            }
            *pObj = nullptr;
        }
    }

} // end of namespace Async
