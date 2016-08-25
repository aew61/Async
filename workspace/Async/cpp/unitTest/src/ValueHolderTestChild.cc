// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "ValueHolderTestChild.h"

namespace Async
{
namespace Tests
{

    ValueHolderTestChild::ValueHolderTestChild() : _pFunc(nullptr)
    {
    }

    ValueHolderTestChild::~ValueHolderTestChild()
    {
        this->_pFunc = nullptr;
    }

    bool ValueHolderTestChild::Call()
    {
        if(this->_pFunc)
        {
            return this->_pFunc();
        }
        return true;
    }

    void ValueHolderTestChild::Destroy()
    {
        delete this;
    }

    void ValueHolderTestChild::AttachFunc(std::function<bool()> pFunc)
    {
        if(!this->_pFunc)
        {
            this->_pFunc = pFunc;
        }
    }

} // end of namespace Tests
} // end of namespace Async
