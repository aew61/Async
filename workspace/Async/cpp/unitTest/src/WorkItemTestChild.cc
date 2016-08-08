// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "WorkItemTestChild.h"

namespace Async
{
namespace Tests
{

    WorkItemTestChild::WorkItemTestChild() : WorkItem()
    {
    }

    WorkItemTestChild::~WorkItemTestChild()
    {
    }

    void WorkItemTestChild::AttachMainFunction(FunctionPtr pFunc)
    {
        this->WorkItem::AttachMainFunction(pFunc);
    }

    void WorkItemTestChild::AttachCleanupFunction(FunctionPtr pFunc)
    {
        this->WorkItem::AttachCleanupFunction(pFunc);
    }

    void WorkItemTestChild::SetException(const std::exception_ptr pException)
    {
        this->WorkItem::SetException(pException);
    }

    States::WorkItemState WorkItemTestChild::Execute()
    {
        return this->WorkItem::Execute();
    }

    void WorkItemTestChild::Finish()
    {
        this->WorkItem::Finish();
    }

    States::WorkItemState WorkItemTestChild::GetState()
    {
        return this->WorkItem::GetState();
    }

    void WorkItemTestChild::SetState(States::WorkItemState newState)
    {
        this->WorkItem::SetState(newState);
    }

} // end of namespace Tests
} // end of namespace Async
