// SYSTEM INCLUDES
// #include <iostream>
#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>
#include <gtest/gtest.h>


// C++ PROJECT INCLUDES
#include "Async/AsyncEngineWrapper.h"
#include "Async/QueueableObject.h"
#include "Async/WorkObject.h"
#include "ValueHolderTestChild.h"
#include "UnitTest_Utils.h"

namespace Async
{
namespace Tests
{

    TEST(Async_AsyncEngineWrapper_unit, Test_Start_And_Stop)
    {
        EXPECT_TRUE(Start(3));
        EXPECT_TRUE(Stop());
    }

    TEST(Async_AsyncEngineWrapper_unit, Test_NumActiveThreads)
    {
        EXPECT_TRUE(Start(3));

        EXPECT_EQ(3, NumActiveThreads());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_AsyncEngineWrapper_unit, Test_SpawnAdditionalThreads)
    {
        EXPECT_TRUE(Start(3));

        EXPECT_EQ(3, NumActiveThreads());
        SpawnAdditionalThreads(3);
        EXPECT_EQ(6, NumActiveThreads());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_AsyncEngineWrapper_unit, Test_StopAdditionalThreads)
    {
        EXPECT_TRUE(Start(3));

        EXPECT_EQ(3, NumActiveThreads());
        const std::vector<std::thread::id> newThreads = SpawnAdditionalThreads(3);
        EXPECT_EQ(6, NumActiveThreads());
        StopAdditionalThreads(newThreads);
        EXPECT_EQ(3, NumActiveThreads());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_AsyncEngineWrapper_unit, Test_ActiveThreads)
    {
        EXPECT_TRUE(Start(3));

        EXPECT_EQ(3, ActiveThreads().size());
        const std::vector<std::thread::id> newThreads = SpawnAdditionalThreads(3);
        EXPECT_EQ(6, ActiveThreads().size());
        StopAdditionalThreads(newThreads);
        EXPECT_EQ(3, ActiveThreads().size());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_AsyncEngineWrapper_unit, Test_GetThreadLoadSnapshot)
    {
        EXPECT_TRUE(Start(4));

        EXPECT_EQ(4, GetThreadLoadSnapshot().size());
        const std::vector<std::thread::id> newThreads = SpawnAdditionalThreads(3);
        EXPECT_EQ(7, GetThreadLoadSnapshot().size());
        StopAdditionalThreads(newThreads);
        EXPECT_EQ(4, GetThreadLoadSnapshot().size());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_AsyncEngineWrapper_unit, Test_COPY_DECREF)
    {
        EXPECT_TRUE(Start(1));

        QueueableObject* pObject = new WorkObject();
        DECREF(&pObject);
        EXPECT_EQ(pObject, nullptr);

        pObject = new WorkObject();
        QueueableObject* pObject2 = reinterpret_cast<WorkObject*>(COPY(pObject));
        EXPECT_NE(pObject2, nullptr);
        DECREF(&pObject2);
        EXPECT_EQ(pObject2, nullptr);
        EXPECT_NE(pObject, nullptr);
        DECREF(&pObject);
        EXPECT_EQ(pObject, nullptr);

        EXPECT_TRUE(Stop());
    }

    TEST(Async_AsyncEngineWrapper_unit, Test_Queue_Multithreaded)
    {
        EXPECT_TRUE(Start(3));

        int x = 5;
        auto pFunc = [&x]() -> bool
        {
            x += 5;
            return true;
        };
        unsigned int numSpawningThreads = 10;
        unsigned int numObjectsPerThread = 5;
        std::vector<std::thread> threads(numSpawningThreads);
        for(unsigned int i = 0; i < numSpawningThreads; ++i)
        {
            threads[i] = std::thread(&SpawnAsyncEngineSubmitter, pFunc, numObjectsPerThread);
        }
        for(unsigned int i = 0; i < numSpawningThreads; ++i)
        {
            threads[i].join();
        }
        /*
        auto checkForZeroes = [](std::vector<std::pair<std::thread::id, int> > snapshot) -> bool
        {
            bool zeros = true;
            for(int i = 0; i < snapshot.size(); ++i)
            {
                zeros = zeros && (snapshot[i].second == 0);
            }
            return zeros;
        };
        while(!checkForZeroes(GetThreadLoadSnapshot()))
        {
            // std::cout << "engine still running" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        */

        // just in case snapshot says the queue in 0 BUT the last workItems are actually executing.
        //std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        while(!IsIdle())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        EXPECT_TRUE(Stop());
        EXPECT_TRUE(5 * (numSpawningThreads * numObjectsPerThread) <= x);
    }

} // end of namespace Tests
} // end of namespace Async
