#pragma once
#ifndef ASYNC_PROMISE_H
#define ASYNC_PROMISE_H

// SYSTEM INCLUDES
#include <queue>
#include <functional>


// C++ PROJECT INCLUDES
#include "Async/AsyncEngineWrapper.h"
#include "Async/ValueHolder.h"

template<typename RESULT_TYPE>
class Promise
{
public:

    Promise(std::function<RESULT_TYPE>() pFunc) : _pObject(Async::MakeObject()),
        _pHolder(new ValueHolder<RESULT_TYPE>(std::move(pFunc))) // use std::move here.
    {
        this->_pObject->Load(this->_pHolder);
    }

    Promise(Promise<RESULT_TYPE>&& other)
    {
        this->_pObject = other._pObject;
        other._pObject = nullptr;

        this->_pHolder = other._pHolder;
        other._pHolder = nullptr;
    }

    Promise& operator=(Promise<RESULT_TYPE>&& other)
    {
        this->_pObject = other._pObject;
        other._pObject = nullptr;

        this->_pHolder = other._pHolder;
        other._pHolder = nullptr;
        return *this;
    }

    ~Promise()
    {
        if(this->_pObject)
        {
            Async::DECREF(&this->_pObject); // this->_pHolder deleted when this->_pObject is deleted.
        }
    }

    

private:

    IRefCountedObject*          _pObject;
    ValueHolder<RESULT_TYPE>    _pHolder;

};


//template<typename RESULT_TYPE, typename... ARG_TYPES>
//class Promise
//{
//public:

    

//private:

    

//};

#endif // end of ASYNC_PROMISE_H
