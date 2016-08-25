#pragma once
#ifndef ASYNC_TESTS_QSORT_H
#define ASYNC_TESTS_QSORT_H

// SYSTEM INCLUDES
#include <thread>
#include <utility>
#include <vector>

// C++ PROJECT INCLUDES



namespace Async
{
namespace Tests
{

    void QSort(std::vector<std::pair<std::thread::id, int> >& toSort, int left, int right);

    int QSortPartition(std::vector<std::pair<std::thread::id, int> >& toSort, int left, int right);

} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_TESTS_QSORT_H
