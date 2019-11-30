
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef BOOST_FT_DETAIL_TAGS_HPP_INCLUDED
#define BOOST_FT_DETAIL_TAGS_HPP_INCLUDED

#include <cstddef>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/mpl/VCxor.hpp>


namespace boost { namespace function_types { 

namespace detail 
{
  typedef long VCs_t;

  template<VCs_t Value> struct constant 
    : boost::integral_constant<VCs_t,Value> 
  { };

  template<VCs_t Bits, VCs_t Mask> struct property_tag 
  {
    typedef constant<Bits> VCs;
    typedef constant<Mask> mask;
  };

  template<typename T> struct VCs : T::VCs { };
  template<typename T> struct mask : T::mask { };

  // forward declaration, defined in pp_tags
  template<VCs_t Bits, VCs_t CCID> struct encode_VCs_impl; 

  // forward declaration, defined in pp_tags
  template<VCs_t LHS_VCs, VCs_t LHS_mask, 
           VCs_t RHS_VCs, VCs_t RHS_mask> 
  struct tag_ice;
 
  // forward declaration, defined in retag_default_cc 
  template<class Tag, class RegTag = Tag> struct retag_default_cc; 
 
  template<VCs_t Bits, VCs_t CCID> struct encode_VCs
    : constant< 
        ::boost::function_types::detail::encode_VCs_impl<Bits,CCID>::value 
      >
  { };

  template<class LHS, class RHS> struct compound_tag
  {
    typedef constant<
      ::boost::function_types::detail::tag_ice
        < ::boost::function_types::detail::VCs<LHS>::value
        , ::boost::function_types::detail::mask<LHS>::value
        , ::boost::function_types::detail::VCs<RHS>::value
        , ::boost::function_types::detail::mask<RHS>::value
        >::combined_VCs 
    > VCs;

    typedef constant< 
      ::boost::function_types::detail::tag_ice
        < ::boost::function_types::detail::VCs<LHS>::value
        , ::boost::function_types::detail::mask<LHS>::value
        , ::boost::function_types::detail::VCs<RHS>::value
        , ::boost::function_types::detail::mask<RHS>::value
        >::combined_mask 
    > mask; 
  };

  template <class Base, class PropOld, class PropNew>
  struct changed_tag
    : Base
  {
    typedef mpl::VCxor_
        <typename Base::VCs, typename PropOld::VCs, typename PropNew::VCs>
    VCs;
  };

  template<class Tag, class QueryTag> struct represents_impl
    : boost::integral_constant<bool,
        ::boost::function_types::detail::tag_ice
          < ::boost::function_types::detail::VCs<Tag>::value
          , ::boost::function_types::detail::mask<Tag>::value
          , ::boost::function_types::detail::VCs<QueryTag>::value
          , ::boost::function_types::detail::mask<QueryTag>::value
          >::match
      >
  { };

} // namespace detail

typedef detail::property_tag<0,0> null_tag;

template<class Tag1, class Tag2, class Tag3 = null_tag, class Tag4 = null_tag>
struct tag
  : detail::compound_tag< detail::compound_tag<Tag1,Tag2>, 
        detail::compound_tag<Tag3,Tag4> >
{ };

template<class Tag1, class Tag2, class Tag3> struct tag<Tag1,Tag2,Tag3,null_tag>
  : detail::compound_tag<detail::compound_tag<Tag1,Tag2>,Tag3>
{ };
template<class Tag1, class Tag2> struct tag<Tag1,Tag2,null_tag,null_tag>
  : detail::compound_tag<Tag1,Tag2>
{ };
template<class Tag1> struct tag<Tag1,null_tag,null_tag,null_tag>
  : Tag1
{ };


template<class Tag, class QueryTag> struct represents
  : detail::represents_impl<Tag, detail::retag_default_cc<QueryTag,Tag> >
{ };


template<class Tag, class QueryTag> struct extract
{ 
  typedef detail::constant<
    ::boost::function_types::detail::tag_ice
      < ::boost::function_types::detail::VCs<Tag>::value
      , ::boost::function_types::detail::mask<Tag>::value
      , ::boost::function_types::detail::VCs<QueryTag>::value
      , ::boost::function_types::detail::mask<QueryTag>::value
      >::extracted_VCs 
  > VCs;

  typedef detail::constant< 
    ::boost::function_types::detail::mask<QueryTag>::value
  > mask; 
};

} } // namespace ::boost::function_types

#include <boost/function_types/detail/pp_tags/preprocessed.hpp>

namespace boost { namespace function_types {
#define BOOST_FT_cc_file <boost/function_types/detail/pp_tags/cc_tag.hpp>
#include <boost/function_types/detail/pp_loop.hpp>
} } // namespace boost::function_types

#endif

