#pragma once
#ifndef ASYNC_VALUEHOLDER_H
#define ASYNC_VALUEHOLDER_H

// SYSTEM INCLUDES
#include <functional>
#include <type_traits>
#include <stdexcept>

// C++ PROJECT INCLUDES
#include "Async/Call.h"
#include "Async/Interfaces/IValueHolder.h"

template<typename VALUE>
class ValueHolder : public Async::IValueHolder
{
public:

    static_assert(!std::is_reference<VALUE>::value,
                  "ValueHolder cannot store reference types");
    static_assert(!std::is_abstract<VALUE>::value,
                  "ValueHolder cannot store abstract types");

    ValueHolder(std::function<VALUE()> pFunc) : _hasValue(false), _pCallFunc(nullptr)
    {
        this->_pCallFunc = [pFunc, this]() -> bool
        {
            this->Assign(Async::Call(pFunc));
            return true;
        };
    }

    bool HasValue() const
    {
        return this->_hasValue;
    }

    VALUE* GetPointer()
    {
        return this->_hasValue ? &this->_value : nullptr;
    }

    const VALUE* GetPointer() const
    {
        return this->_hasValue ? &this->_value : nullptr;
    }

    explicit operator bool() const
    {
        return this->HasValue();
    }

    VALUE GetValue()
    {
        this->RequireContents();
        return this->_value;
    }

    const VALUE& GetValue() const
    {
        this->RequireContents();
        return this->_value;
    }

    void Clear()
    {
        this->_hasValue = false;
    }

    void Assign(VALUE&& val)
    {
        if(this->HasValue())
        {
            this->_value = std::move(val);
        }
        else
        {
            this->Construct(std::move(val));
        }
    }

    void Assign(const VALUE& val)
    {
        if(this->HasValue())
        {
            this->_value = val;
        }
        else
        {
            this->Construct(val);
        }
    }

    template<typename ARG>
    ValueHolder& operator=(ARG&& arg)
    {
        this->Assign(std::forward<ARG>(arg));
        return *this;
    }

    const VALUE& operator * () const {return this->GetValue();}
          VALUE  operator * ()       {return this->GetValue();}

    const VALUE* operator -> () const {return &this->GetValue();}
          VALUE* operator -> ()       {return &this->GetValue();}

    bool Call() override
    {
        return this->_pCallFunc();
    }

    void Destroy() override
    {
        delete this;
    }

private:

    void RequireContents() const
    {
        if(!this->_value)
        {
            throw std::logic_error("No value stored");
        }
    }

    template<typename... ARGS>
    void Construct(ARGS&&... args)
    {
        const void* ptr = &this->_value;
        new(const_cast<void*>(ptr)) VALUE(std::forward<ARGS>(args)...);
        this->_hasValue = true;
    }

private:

    VALUE                   _value;
    bool                    _hasValue;
    std::function<bool()>   _pCallFunc;

};

template<typename VALUE, typename... ARGS>
class ValueHolder : public Async::IValueHolder
{
public:

    static_assert(!std::is_reference<VALUE>::value,
                  "ValueHolder cannot store reference types");
    static_assert(!std::is_abstract<VALUE>::value,
                  "ValueHolder cannot store abstract types");

    ValueHolder(std::function<VALUE(ARGS...)> pFunc) : _hasValue(false), _pCallFunc(nullptr),
        _argTuple()
    {
        this->_pCallFunc = [pFunc, this]() -> bool
        {
            this->Assign(Async::Call(pFunc, this->_argTuple));
            return true;
        };
    }

    bool HasValue() const
    {
        return this->_hasValue;
    }

    VALUE* GetPointer()
    {
        return this->_hasValue ? &this->_value : nullptr;
    }

    const VALUE* GetPointer() const
    {
        return this->_hasValue ? &this->_value : nullptr;
    }

    explicit operator bool() const
    {
        return this->HasValue();
    }

    VALUE GetValue()
    {
        this->RequireContents();
        return this->_value;
    }

    const VALUE& GetValue() const
    {
        this->RequireContents();
        return this->_value;
    }

    void Clear()
    {
        this->_hasValue = false;
    }

    void Assign(VALUE&& val)
    {
        if(this->HasValue())
        {
            this->_value = std::move(val);
        }
        else
        {
            this->Construct(std::move(val));
        }
    }

    void Assign(const VALUE& val)
    {
        if(this->HasValue())
        {
            this->_value = val;
        }
        else
        {
            this->Construct(val);
        }
    }

    template<typename ARG>
    ValueHolder& operator=(ARG&& arg)
    {
        this->Assign(std::forward<ARG>(arg));
        return *this;
    }

    const VALUE& operator * () const {return this->GetValue();}
          VALUE  operator * ()       {return this->GetValue();}

    const VALUE* operator -> () const {return &this->GetValue();}
          VALUE* operator -> ()       {return &this->GetValue();}

    bool Call() override
    {
        return this->_pCallFunc();
    }

    void Destroy() override
    {
        delete this;
    }

    void GiveArgs(ARGS&&... args)
    {
        this->_argTuple = std::make_tuple(std::forward<ARGS>(args)...);
    }

private:

    void RequireContents() const
    {
        if(!this->_value)
        {
            throw std::logic_error("No value stored");
        }
    }

    template<typename... ARGS>
    void Construct(ARGS&&... args)
    {
        const void* ptr = &this->_value;
        new(const_cast<void*>(ptr)) VALUE(std::forward<ARGS>(args)...);
        this->_hasValue = true;
    }

private:

    VALUE                   _value;
    bool                    _hasValue;
    std::function<bool()>   _pCallFunc;
    std::tuple<ARGS...>     _argTuple;

};


#endif // end of ASYNC_VALUEHOLDER_H
