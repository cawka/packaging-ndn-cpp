//  boost utility/base_from_member.hpp header file  --------------------------//

//  Copyright 2001, 2003, 2004 Daryle Walker.  Use, modification, and
//  distribution are subject to the Boost Software License, Version 1.0.  (See
//  accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/utility/> for the library's home page.

#ifndef NDNBOOST_UTILITY_BASE_FROM_MEMBER_HPP
#define NDNBOOST_UTILITY_BASE_FROM_MEMBER_HPP

#include <ndnboost/preprocessor/arithmetic/inc.hpp>
#include <ndnboost/preprocessor/repetition/enum_binary_params.hpp>
#include <ndnboost/preprocessor/repetition/enum_params.hpp>
#include <ndnboost/preprocessor/repetition/repeat_from_to.hpp>


//  Base-from-member arity configuration macro  ------------------------------//

// The following macro determines how many arguments will be in the largest
// constructor template of base_from_member.  Constructor templates will be
// generated from one argument to this maximum.  Code from other files can read
// this number if they need to always match the exact maximum base_from_member
// uses.  The maximum constructor length can be changed by overriding the
// #defined constant.  Make sure to apply the override, if any, for all source
// files during project compiling for consistency.

// Contributed by Jonathan Turkanis

#ifndef NDNBOOST_BASE_FROM_MEMBER_MAX_ARITY
#define NDNBOOST_BASE_FROM_MEMBER_MAX_ARITY  10
#endif


//  An iteration of a constructor template for base_from_member  -------------//

// A macro that should expand to:
//     template < typename T1, ..., typename Tn >
//     base_from_member( T1 x1, ..., Tn xn )
//         : member( x1, ..., xn )
//         {}
// This macro should only persist within this file.

#define NDNBOOST_PRIVATE_CTR_DEF( z, n, data )                            \
    template < NDNBOOST_PP_ENUM_PARAMS(n, typename T) >                   \
    explicit base_from_member( NDNBOOST_PP_ENUM_BINARY_PARAMS(n, T, x) )  \
        : member( NDNBOOST_PP_ENUM_PARAMS(n, x) )                         \
        {}                                                             \
    /**/


namespace ndnboost
{

//  Base-from-member class template  -----------------------------------------//

// Helper to initialize a base object so a derived class can use this
// object in the initialization of another base class.  Used by
// Dietmar Kuehl from ideas by Ron Klatcho to solve the problem of a
// base class needing to be initialized by a member.

// Contributed by Daryle Walker

template < typename MemberType, int UniqueID = 0 >
class base_from_member
{
protected:
    MemberType  member;

    base_from_member()
        : member()
        {}

    NDNBOOST_PP_REPEAT_FROM_TO( 1, NDNBOOST_PP_INC(NDNBOOST_BASE_FROM_MEMBER_MAX_ARITY),
     NDNBOOST_PRIVATE_CTR_DEF, _ )

};  // ndnboost::base_from_member

}  // namespace ndnboost


// Undo any private macros
#undef NDNBOOST_PRIVATE_CTR_DEF


#endif  // NDNBOOST_UTILITY_BASE_FROM_MEMBER_HPP
