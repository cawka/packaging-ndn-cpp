//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NDNBOOST_UUID_7E83C166200811DE885E826156D89593
#define NDNBOOST_UUID_7E83C166200811DE885E826156D89593
#if (__GNUC__*100+__GNUC_MINOR__>301) && !defined(NDNBOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma GCC system_header
#endif
#if defined(_MSC_VER) && !defined(NDNBOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(push,1)
#endif

namespace
ndnboost
    {
    template <class E>
    inline
    E *
    current_exception_cast()
        {
        try
            {
            throw;
            }
        catch(
        E & e )
            {
            return &e;
            }
        catch(
        ...)
            {
            return 0;
            }
        }
    }

#if defined(_MSC_VER) && !defined(NDNBOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(pop)
#endif
#endif
