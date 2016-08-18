// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "DynamicQueueableObjectTestChild.h"

namespace Async
{
namespace Tests
{

    DynamicQueueableObjectTestChild::DynamicQueueableObjectTestChild(std::function<void()> pFunc) :
        _pFunc(pFunc), _toSet(false)
    {
    }

    DynamicQueueableObjectTestChild::~DynamicQueueableObjectTestChild()
    {
    }

    bool DynamicQueueableObjectTestChild::Queue(Concurrency::WorkerThread* pThread)
    {
        return true;
    }

    void DynamicQueueableObjectTestChild::WaitForExecution()
    {
    }

    void DynamicQueueableObjectTestChild::Load(IValueHolder* pPromise)
    {
    }

    std::exception_ptr DynamicQueueableObjectTestChild::GetError() const
    {
        return nullptr;
    }

    bool DynamicQueueableObjectTestChild::GetVal()
    {
        return this->_toSet;
    }

    const int DynamicQueueableObjectTestChild::GetChildRefCount()
    {
        return this->QueueableObject::GetRefCount();
    }

    void DynamicQueueableObjectTestChild::Execute()
    {
        this->_pFunc();
        this->_toSet = true;
    }

    bool DynamicQueueableObjectTestChild::IsDone()
    {
        return this->_toSet;
    }

} // end of namespace Tests
} // end of namespace Async
