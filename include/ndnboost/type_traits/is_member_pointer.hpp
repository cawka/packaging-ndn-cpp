
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


#ifndef NDNBOOST_TT_IS_MEMBER_POINTER_HPP_INCLUDED
#define NDNBOOST_TT_IS_MEMBER_POINTER_HPP_INCLUDED

#include <ndnboost/type_traits/config.hpp>
#include <ndnboost/detail/workaround.hpp>

#if !defined(NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !NDNBOOST_WORKAROUND(__BORLANDC__, < 0x600)
#   include <ndnboost/type_traits/is_member_function_pointer.hpp>
#else
#   include <ndnboost/type_traits/is_reference.hpp>
#   include <ndnboost/type_traits/is_array.hpp>
#   include <ndnboost/type_traits/detail/is_mem_fun_pointer_tester.hpp>
#   include <ndnboost/type_traits/detail/yes_no_type.hpp>
#   include <ndnboost/type_traits/detail/false_result.hpp>
#   include <ndnboost/type_traits/detail/ice_or.hpp>
#endif

// should be the last #include
#include <ndnboost/type_traits/detail/bool_trait_def.hpp>

namespace ndnboost {

#if defined( __CODEGEARC__ )
NDNBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,__is_member_pointer(T))
#elif NDNBOOST_WORKAROUND(__BORLANDC__, < 0x600)
NDNBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,false)
NDNBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*,true)

#elif !defined(NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
NDNBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,::ndnboost::is_member_function_pointer<T>::value)
NDNBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*,true)

#if !NDNBOOST_WORKAROUND(__MWERKS__,<=0x3003) && !NDNBOOST_WORKAROUND(__IBMCPP__, <=600)
NDNBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*const,true)
NDNBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*volatile,true)
NDNBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*const volatile,true)
#endif

#else // no partial template specialization

namespace detail {

template <typename R, typename T>
::ndnboost::type_traits::yes_type NDNBOOST_TT_DECL is_member_pointer_tester(R T::*const volatile*);
::ndnboost::type_traits::no_type NDNBOOST_TT_DECL is_member_pointer_tester(...);

template <bool>
struct is_member_pointer_select
    : public ::ndnboost::type_traits::false_result
{
};

template <>
struct is_member_pointer_select<false>
{
    template <typename T> struct result_
    {
        static T* make_t();
        NDNBOOST_STATIC_CONSTANT(
            bool, value =
            (::ndnboost::type_traits::ice_or<
                (1 == sizeof(::ndnboost::type_traits::is_mem_fun_pointer_tester(make_t()))),
                (1 == sizeof(is_member_pointer_tester(make_t())))
            >::value) );
    };
};

template <typename T>
struct is_member_pointer_impl
    : public is_member_pointer_select<
          ::ndnboost::type_traits::ice_or<
              ::ndnboost::is_reference<T>::value
            , ::ndnboost::is_array<T>::value
            >::value
        >::template result_<T>
{
};

NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_pointer,void,false)
#ifndef NDNBOOST_NO_CV_VOID_SPECIALIZATIONS
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_pointer,void const,false)
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_pointer,void volatile,false)
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_pointer,void const volatile,false)
#endif

} // namespace detail

NDNBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,::ndnboost::detail::is_member_pointer_impl<T>::value)

#endif // __BORLANDC__

} // namespace ndnboost

#include <ndnboost/type_traits/detail/bool_trait_undef.hpp>

#endif // NDNBOOST_TT_IS_MEMBER_POINTER_HPP_INCLUDED
