// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "DynamicQueueableWorkItemTestChild.h"

namespace Async
{
namespace Tests
{

    DynamicQueueableWorkItemTestChild::DynamicQueueableWorkItemTestChild(std::function<void()> pFunc) :
        _pFunc(pFunc), _toSet(false)
    {
    }

    DynamicQueueableWorkItemTestChild::~DynamicQueueableWorkItemTestChild()
    {
    }

    Types::Result_t DynamicQueueableWorkItemTestChild::Queue(Concurrency::WorkerThread* pThread)
    {
        return Types::Result_t::SUCCESS;
    }

    bool DynamicQueueableWorkItemTestChild::GetVal()
    {
        return this->_toSet;
    }

    int DynamicQueueableWorkItemTestChild::GetRefCount()
    {
        return this->QueueableWorkItem::GetRefCount();
    }

    void DynamicQueueableWorkItemTestChild::Execute()
    {
        this->_pFunc();
        this->_toSet = true;
    }

    bool DynamicQueueableWorkItemTestChild::IsDone()
    {
        return this->_toSet;
    }

} // end of namespace Tests
} // end of namespace Async
