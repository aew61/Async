// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "QSort.h"

namespace Async
{
namespace Tests
{

    void QSort(std::vector<std::pair<std::thread::id, int> >& toSort, int left, int right)
    {
        if(left < right)
        {
            int partition = QSortPartition(toSort, left, right);
            QSort(toSort, left, partition);
            QSort(toSort, partition + 1, right);
        }
    }

    int QSortPartition(std::vector<std::pair<std::thread::id, int> >& toSort, int left, int right)
    {
        int lower = toSort[left].second;
        int upper = toSort[right].second;
        int middle = toSort[(left + right) / 2].second;
        if(middle < lower) { lower = middle; }
        if(middle > upper) { upper = middle; }
        int pivot = (middle + upper) / 2;
        int i = left - 1;
        int j = right + 1;
        while(true)
        {
            do { ++i; }
            while(toSort[i].second < pivot);

            do { --j; }
            while(toSort[j].second > pivot);

            if(i >= j)
            {
                return j;
            }
            std::swap(toSort[i],toSort[j]);
        }
    }

} // end of namespace Tests
} // end of namespace Async
