#pragma once
#ifndef ASYNC_ICONTINUATIONLINKER_H
#define ASYNC_ICONTINUATIONLINKER_H

// SYSTEM INCLUDES


// C++ PROJECT INCLUDES


namespace Async
{

    class IContinuationLinker
    {
    public:

        ~IContinuationLinker() = default;

        virtual void Chain() = 0;

    };

} // end of namespace Async

#endif // end of ASYNC_ICONTINUATIONLINKER_H
