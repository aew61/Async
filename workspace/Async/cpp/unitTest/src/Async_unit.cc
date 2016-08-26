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
#include "Async/Async.h"
#include "Async/QueueableObject.h"
#include "Async/WorkObject.h"
#include "ValueHolderTestChild.h"
#include "UnitTest_Utils.h"

namespace Async
{
namespace Tests
{

    TEST(Async_Async_unit, Test_Start_And_Stop)
    {
        EXPECT_TRUE(Start(3));
        EXPECT_TRUE(Stop());
    }

    TEST(Async_Async_unit, Test_NumActiveThreads)
    {
        EXPECT_TRUE(Start(3));

        EXPECT_EQ(3, NumActiveThreads());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_Async_unit, Test_SpawnAdditionalThreads)
    {
        EXPECT_TRUE(Start(3));

        EXPECT_EQ(3, NumActiveThreads());
        SpawnAdditionalThreads(3);
        EXPECT_EQ(6, NumActiveThreads());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_Async_unit, Test_StopAdditionalThreads)
    {
        EXPECT_TRUE(Start(3));

        EXPECT_EQ(3, NumActiveThreads());
        const std::vector<std::thread::id> newThreads = SpawnAdditionalThreads(3);
        EXPECT_EQ(6, NumActiveThreads());
        StopAdditionalThreads(newThreads);
        EXPECT_EQ(3, NumActiveThreads());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_Async_unit, Test_ActiveThreads)
    {
        EXPECT_TRUE(Start(3));

        EXPECT_EQ(3, ActiveThreads().size());
        const std::vector<std::thread::id> newThreads = SpawnAdditionalThreads(3);
        EXPECT_EQ(6, ActiveThreads().size());
        StopAdditionalThreads(newThreads);
        EXPECT_EQ(3, ActiveThreads().size());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_Async_unit, Test_GetThreadLoadSnapshot)
    {
        EXPECT_TRUE(Start(4));

        EXPECT_EQ(4, GetThreadLoadSnapshot().size());
        const std::vector<std::thread::id> newThreads = SpawnAdditionalThreads(3);
        EXPECT_EQ(7, GetThreadLoadSnapshot().size());
        StopAdditionalThreads(newThreads);
        EXPECT_EQ(4, GetThreadLoadSnapshot().size());

        EXPECT_TRUE(Stop());
    }

    TEST(Async_Async_unit, Test_COPY_DECREF)
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

} // end of namespace Tests
} // end of namespace Async
