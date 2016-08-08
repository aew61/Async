// SYSTEM INCLUDES
// #include <iostream>

// C++ PROJECT INCLUDES
#include "QueueableWorkItemTestChild.h"

namespace Async
{
namespace Tests
{

    QueueableWorkItemTestChild::QueueableWorkItemTestChild() : QueueableWorkItem(),
        _toSet(false)
    {
    }

    QueueableWorkItemTestChild::~QueueableWorkItemTestChild()
    {
    }

    Types::Result_t QueueableWorkItemTestChild::Queue(Concurrency::WorkerThread* pThread)
    {
        return Types::Result_t::SUCCESS;
    }

    void QueueableWorkItemTestChild::ExecuteInterface()
    {
        this->Execute();
    }

    int QueueableWorkItemTestChild::GetRefCount()
    {
        return this->QueueableWorkItem::GetRefCount();
    }

    bool QueueableWorkItemTestChild::GetVal()
    {
        return this->_toSet;
    }

    void QueueableWorkItemTestChild::Execute()
    {
        // std::cout << "Executing work item!" << std::endl;
        this->_toSet = true;
    }

    bool QueueableWorkItemTestChild::IsDone()
    {
        return this->_toSet;
    }

} // end of namespace Tests
} // end of namespace Async
