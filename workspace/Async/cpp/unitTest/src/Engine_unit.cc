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
#include "Async/Engine.h"
#include "Async/WorkObject.h"
#include "ValueHolderTestChild.h"
#include "UnitTest_Utils.h"

namespace Async
{
namespace Tests
{

    TEST(Async_Engine_unit, Test_Constructor_And_Shutdown)
    {
        // create engine instance with 2 worker threads and 1 garbage collector (3 total)
        Engine e(2);
        EXPECT_EQ(2, e.NumThreads());
        e.Shutdown();
        EXPECT_EQ(0, e.NumThreads());

        Engine e2(0);
        EXPECT_EQ(0, e2.NumThreads());
        e2.Shutdown();
        EXPECT_EQ(0, e2.NumThreads());
    }

    TEST(Async_Engine_unit, Test_Internal_QuickSort_Algorithm)
    {
        Engine e(0);
        std::vector<std::pair<std::thread::id, int> > toSort = {
            {std::this_thread::get_id(), 10},
            {std::this_thread::get_id(), 11},
            {std::this_thread::get_id(), 9}
        };
        std::vector<std::pair<std::thread::id, int> > sorted = {
            {std::this_thread::get_id(), 9},
            {std::this_thread::get_id(), 10},
            {std::this_thread::get_id(), 11}
        };

        e.QuickSort(toSort, 0, toSort.size() - 1);
        EXPECT_EQ(sorted, toSort);

        int newSize = 50;
        toSort.clear();
        sorted.clear();

        // build toSort in reverse order
        // and sorted in correct order
        for(int i = 0; i < newSize; ++i)
        {
            toSort.push_back(std::make_pair(std::this_thread::get_id(), newSize - i - 1));
            sorted.push_back(std::make_pair(std::this_thread::get_id(), i));
        }

        e.QuickSort(toSort, 0, toSort.size() - 1);
        EXPECT_EQ(sorted, toSort);
    }

    TEST(Async_Engine_unit, Test_GetThreadLoadSnapshot)
    {
        Engine e(5);
        std::vector<std::pair<std::thread::id, int> > vec = e.GetThreadLoadSnapshot();
        EXPECT_EQ(5, vec.size());
    }

    TEST(Async_Engine_unit, Test_SpawnWorkerThreads_ShutdownWorkerThreads)
    {
        Engine e(3);
        EXPECT_EQ(3, e.NumThreads());
        std::vector<std::thread::id> newThreads = e.SpawnWorkerThreads(2);
        EXPECT_EQ(5, e.NumThreads());
        e.ShutdownWorkerThreads(newThreads);
        EXPECT_EQ(3, e.NumThreads());
    }

    TEST(Async_Engine_unit, Test_Queue)
    {
        Engine e(3);
        EXPECT_EQ(3, e.NumThreads());
        std::vector<std::pair<std::thread::id, int> > snapshot = e.GetThreadLoadSnapshot();
        EXPECT_EQ(3, snapshot.size());

        int x = 5;
        // create the workItem
        WorkObject* pWorkObject = new WorkObject();
        ValueHolderTestChild* pHolder = new ValueHolderTestChild();
        pHolder->AttachFunc([&x]() -> bool
        {
            x += 5;
            return true;
        });
        pWorkObject->Load(pHolder);
        EXPECT_TRUE(e.Queue(pWorkObject, snapshot[0].first));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        EXPECT_EQ(10, x);
        e.Shutdown();
        EXPECT_EQ(0, e.NumThreads());
    }

    TEST(Async_Engine_unit, Test_Queue_Multithreaded_WorkerThread_Distribution)
    {
        Engine e(3);
        EXPECT_EQ(3, e.NumThreads());
        unsigned int numQueuingThreads = 10;
        unsigned int numObjectsPerThread = 5;
        std::vector<std::thread> threads(numQueuingThreads);
        std::mutex recordMutex;
        std::vector<std::vector<std::pair<std::thread::id, int> > > records;
        std::mutex xMutex;
        std::map<std::thread::id, int> threadDistribution;
        std::vector<std::pair<std::thread::id, int> > initialSnapshot = e.GetThreadLoadSnapshot();
        for(int i = 0; i < initialSnapshot.size(); ++i)
        {
            threadDistribution[initialSnapshot[i].first] = 0;
        }
        int x = 5;
        auto pFunc = [&x, &xMutex]() -> bool
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> lock(xMutex);
            x += 5;
            return true;
        };
        for(unsigned int i = 0; i < numQueuingThreads; ++i)
        {
            threads[i] = std::thread(&SpawnEngineSubmitter, &e, pFunc, &recordMutex,
                                                            &records, numObjectsPerThread);
            //std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
        for(int i = 0; i < threads.size(); ++i)
        {
            threads[i].join();
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(100 * numQueuingThreads * numObjectsPerThread));
        auto checkForZeroes = [](std::vector<std::pair<std::thread::id, int> > snapshot) -> bool
        {
            bool zeros = true;
            for(int i = 0; i < snapshot.size(); ++i)
            {
                zeros = zeros && (snapshot[i].second == 0);
            }
            return zeros;
        };
        while(!checkForZeroes(e.GetThreadLoadSnapshot()))
        {
            // std::cout << "engine still running" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        EXPECT_EQ(x, 5 * (numQueuingThreads * numObjectsPerThread + 1));
        EXPECT_EQ(records.size(), numQueuingThreads * numObjectsPerThread);
        for(int i = 0; i < records.size(); ++i)
        {
            //std::cout << "<";
            for(int j = 0; j < records[i].size(); ++j)
            {
                threadDistribution[records[i][j].first] += records[i][j].second;
                //std::cout << "{" << records[i][j].first << ", " << records[i][j].second << "}";
                //if(j < records[i].size() - 1)
                //{
                //    std::cout << ", ";
                //}
            }
            //std::cout << ">" << std::endl;
        }
        for(auto it = threadDistribution.begin(); it != threadDistribution.end(); ++it)
        {
            // std::cout << "{" << it->first << ": " << it->second << "}" << std::endl;
            EXPECT_NE(0, it->second);
        }

        e.Shutdown();
        EXPECT_EQ(0, e.NumThreads());
    }

} // end of namespace Tests
} // end of namespace Async
