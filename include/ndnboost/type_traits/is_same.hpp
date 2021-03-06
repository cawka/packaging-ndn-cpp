
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, 
//      Howard Hinnant and John Maddock 2000. 
//  (C) Copyright Mat Marcus, Jesse Jones and Adobe Systems Inc 2001

//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

//    Fixed is_pointer, is_reference, is_const, is_volatile, is_same, 
//    is_member_pointer based on the Simulated Partial Specialization work 
//    of Mat Marcus and Jesse Jones. See  http://opensource.adobe.com or 
//    http://groups.yahoo.com/group/boost/message/5441 
//    Some workarounds in here use ideas suggested from "Generic<Programming>: 
//    Mappings between Types and Values" 
//    by Andrei Alexandrescu (see http://www.cuj.com/experts/1810/alexandr.html).


#ifndef NDNBOOST_TT_IS_SAME_HPP_INCLUDED
#define NDNBOOST_TT_IS_SAME_HPP_INCLUDED

#include <ndnboost/type_traits/config.hpp>
#ifdef NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <ndnboost/type_traits/detail/yes_no_type.hpp>
#include <ndnboost/type_traits/detail/ice_and.hpp>
#include <ndnboost/type_traits/is_reference.hpp>
#endif
// should be the last #include
#include <ndnboost/type_traits/detail/bool_trait_def.hpp>

namespace ndnboost {

#ifndef NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

NDNBOOST_TT_AUX_BOOL_TRAIT_DEF2(is_same,T,U,false)
NDNBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T,is_same,T,T,true)
#if NDNBOOST_WORKAROUND(__BORLANDC__, < 0x600)
// without this, Borland's compiler gives the wrong answer for
// references to arrays:
NDNBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(typename T,is_same,T&,T&,true)
#endif

#else // NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail {

#ifdef NDNBOOST_MSVC
// the following VC6 specific implementation is *NOT* legal
// C++, but has the advantage that it works for incomplete
// types.

template< typename T1 >
struct is_same_part_1
{
    template<typename T2>  struct part_2     { enum { value = false }; };
    template<>             struct part_2<T1> { enum { value = true }; };
};

template< typename T1, typename T2 >
struct is_same_impl
{
    enum { value = ndnboost::detail::is_same_part_1<T1>::template part_2<T2>::value };
};

#else // generic "no-partial-specialization" version

template <typename T>
::ndnboost::type_traits::yes_type
NDNBOOST_TT_DECL is_same_tester(T*, T*);

::ndnboost::type_traits::no_type
NDNBOOST_TT_DECL is_same_tester(...);

template <typename T, typename U>
struct is_same_impl
{
   static T t;
   static U u;

   NDNBOOST_STATIC_CONSTANT(bool, value =
      (::ndnboost::type_traits::ice_and<
         (sizeof(type_traits::yes_type) == sizeof(ndnboost::detail::is_same_tester(&t,&u))),
         (::ndnboost::is_reference<T>::value == ::ndnboost::is_reference<U>::value),
         (sizeof(T) == sizeof(U))
        >::value));
};

#endif // NDNBOOST_MSVC

} // namespace detail

NDNBOOST_TT_AUX_BOOL_TRAIT_DEF2(is_same,T,U,(::ndnboost::detail::is_same_impl<T,U>::value))

#endif // NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace ndnboost

#include <ndnboost/type_traits/detail/bool_trait_undef.hpp>

#endif  // NDNBOOST_TT_IS_SAME_HPP_INCLUDED

