// SYSTEM INCLUDES
// #include <iostream>

// C++ PROJECT INCLUDES
#include "QueueableObjectTestChild.h"

namespace Async
{
namespace Tests
{

    QueueableObjectTestChild::QueueableObjectTestChild() : QueueableObject(),
        _toSet(false)
    {
    }

    QueueableObjectTestChild::~QueueableObjectTestChild()
    {
    }

    bool QueueableObjectTestChild::Queue(Concurrency::WorkerThread* pThread)
    {
        return true;
    }

    void QueueableObjectTestChild::WaitForExecution()
    {
    }

    void QueueableObjectTestChild::Load(IValueHolder* pPromise)
    {
    }

    std::exception_ptr QueueableObjectTestChild::GetError() const
    {
        return nullptr;
    }

    void QueueableObjectTestChild::ExecuteInterface()
    {
        this->Execute();
    }

    const int QueueableObjectTestChild::GetChildRefCount()
    {
        return this->QueueableObject::GetRefCount();
    }

    bool QueueableObjectTestChild::GetVal()
    {
        return this->_toSet;
    }

    void QueueableObjectTestChild::Execute()
    {
        // std::cout << "Executing work item!" << std::endl;
        this->_toSet = true;
    }

    bool QueueableObjectTestChild::IsDone()
    {
        return this->_toSet;
    }

} // end of namespace Tests
} // end of namespace Async
