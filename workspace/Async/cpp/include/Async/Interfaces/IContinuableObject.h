#pragma once
#ifndef ASYNC_ICONTINUABLEOBJECT_H
#define ASYNC_ICONTINUABLEOBJECT_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES

namespace Async
{

    class WorkObject;

    class IContinuableObject
    {
    public:

        virtual ~IContinuableObject() = default;

        virtual void AddContinuation(WorkObject* pChild,
                                     bool onSuccess=true) = 0;

    };

} // end of namespace Async

#endif // end of ASYNC_ICONTINUABLEOBJECT_H
