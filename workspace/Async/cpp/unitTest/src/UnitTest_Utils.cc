// SYSTEM INCLUDES
#include <algorithm>
#include <chrono>
#include <thread>
#include <functional>
#include <gtest/gtest.h>

// C++ PROJECT INCLUDES
#include "Async/WorkObject.h"
#include "Async/AsyncEngineWrapper.h"
#include "UnitTest_Utils.h"
#include "ValueHolderTestChild.h"

namespace Async
{
namespace Tests
{

    void CreateChain(std::vector<DynamicQueueableObjectTestChild*>* pVec,
                     Concurrency::WorkerThread* pThread,
                     int blockingTime, int numWorkItemSoFar, int maxWorkItems)
    {
        if(numWorkItemSoFar < maxWorkItems)
        {
            DynamicQueueableObjectTestChild* pTestChild =
                new DynamicQueueableObjectTestChild(
                    [pVec, pThread, blockingTime, numWorkItemSoFar, maxWorkItems]() -> void
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(blockingTime));
                        CreateChain(pVec, pThread, blockingTime, numWorkItemSoFar + 1, maxWorkItems);
                    }
                );
            pVec->push_back(pTestChild);
            EXPECT_TRUE(pThread->Queue(pTestChild));
        }
    }

    void SpawnChild(SharedMemory* pShared)
    {
        std::lock_guard<std::mutex> lock(pShared->_mutex);
        int* pInt = &pShared->_count;
        DynamicQueueableObjectTestChild* pChild =
            new DynamicQueueableObjectTestChild([pInt]() -> void
            {
                *pInt = *pInt + 1;
            });
        pShared->_vec.push_back(pChild);
        pShared->_pWT->Queue(pChild);
    }

    void SpawnEngineSubmitter(Engine* pEngine, std::function<bool()> pFunc,
                              std::mutex* pRecordMutex,
                              std::vector<std::vector<std::pair<std::thread::id, int> > >* toRecord,
                              unsigned int numObjectsToSpawn)
    {
        unsigned int iter = 0;
        auto pComp = [](std::pair<std::thread::id, int> left, std::pair<std::thread::id, int> right) -> bool
        {
            return left.second < right.second;
        };
        do
        {
            WorkObject* pObject = new WorkObject();
            ValueHolderTestChild* pHolder = new ValueHolderTestChild();
            pHolder->AttachFunc(pFunc);
            pObject->Load(pHolder);

            std::vector<std::pair<std::thread::id, int> > snapshot = pEngine->GetThreadLoadSnapshot();
            EXPECT_EQ(snapshot.size(), pEngine->NumThreads());
            EXPECT_TRUE(snapshot.size() > 0);
            std::sort(snapshot.begin(), snapshot.end(), pComp);

            std::unique_lock<std::mutex> lock(*pRecordMutex);
            toRecord->push_back(snapshot);
            lock.unlock();

            EXPECT_TRUE(pEngine->Queue(pObject, snapshot[0].first));
        }
        while(++iter < numObjectsToSpawn);
    }

    void SpawnAsyncEngineSubmitter(std::function<bool()> pFunc, unsigned int numObjectsToSpawn)
    {
        unsigned int iter = 0;
        auto pComp = [](std::pair<std::thread::id, int> left, std::pair<std::thread::id, int> right) -> bool
        {
            return left.second < right.second;
        };
        do
        {
            IRefCountedObject* pObject = new WorkObject();
            ValueHolderTestChild* pHolder = new ValueHolderTestChild();
            pHolder->AttachFunc(pFunc);
            pObject->Load(pHolder);

            std::vector<std::pair<std::thread::id, int> > snapshot = GetThreadLoadSnapshot();
            EXPECT_EQ(snapshot.size(), NumActiveThreads());
            EXPECT_TRUE(snapshot.size() > 0);
            std::sort(snapshot.begin(), snapshot.end(), pComp);

            EXPECT_TRUE(Queue(pObject, snapshot[0].first));
        }
        while(++iter < numObjectsToSpawn);
    }

} // end of namespac Tests
} // end of namespace Async
