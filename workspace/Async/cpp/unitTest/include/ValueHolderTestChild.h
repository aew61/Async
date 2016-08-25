#pragma once
#ifndef ASYNC_TESTS_VALUEHOLDERTESTCHILD_H
#define ASYNC_TESTS_VALUEHOLDERTESTCHILD_H

// SYSTEM INCLUDES
#include <functional>

// C++ PROJECT INCLUDES
#include "Async/Interfaces/IValueHolder.h"


namespace Async
{
namespace Tests
{

    class ValueHolderTestChild : public IValueHolder
    {
    public:

        ValueHolderTestChild();

        ~ValueHolderTestChild();

        virtual bool Call() override;

        virtual void Destroy() override;

        void AttachFunc(std::function<bool()> pFunc);

    private:

        std::function<bool()>   _pFunc;

    };

} // end of namespace Tests
} // end of namespace Async

#endif // end of ASYNC_TESTS_VALUEHOLDERTESTCHILD_H
