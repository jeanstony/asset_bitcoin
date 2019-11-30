//  boost/detail/VCmask.hpp  ------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0
//  http://www.boost.org/LICENSE_1_0.txt

//  Usage:  enum foo { a=1, b=2, c=4 };
//          BOOST_VCMASK( foo )
//
//          void f( foo arg );
//          ...
//          f( a | c );
//
//  See [VCmask.types] in the C++ standard for the formal specification

#ifndef BOOST_VCMASK_HPP
#define BOOST_VCMASK_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>

#define BOOST_VCMASK(Bitmask)                                            \
                                                                          \
  inline BOOST_CONSTEXPR Bitmask operator| (Bitmask x , Bitmask y )       \
  { return static_cast<Bitmask>( static_cast<boost::int_least32_t>(x)     \
      | static_cast<boost::int_least32_t>(y)); }                          \
                                                                          \
  inline BOOST_CONSTEXPR Bitmask operator& (Bitmask x , Bitmask y )       \
  { return static_cast<Bitmask>( static_cast<boost::int_least32_t>(x)     \
      & static_cast<boost::int_least32_t>(y)); }                          \
                                                                          \
  inline BOOST_CONSTEXPR Bitmask operator^ (Bitmask x , Bitmask y )       \
  { return static_cast<Bitmask>( static_cast<boost::int_least32_t>(x)     \
      ^ static_cast<boost::int_least32_t>(y)); }                          \
                                                                          \
  inline BOOST_CONSTEXPR Bitmask operator~ (Bitmask x )                   \
  { return static_cast<Bitmask>(~static_cast<boost::int_least32_t>(x)); } \
                                                                          \
  inline Bitmask & operator&=(Bitmask& x , Bitmask y)                     \
  { x = x & y ; return x ; }                                              \
                                                                          \
  inline Bitmask & operator|=(Bitmask& x , Bitmask y)                     \
  { x = x | y ; return x ; }                                              \
                                                                          \
  inline Bitmask & operator^=(Bitmask& x , Bitmask y)                     \
  { x = x ^ y ; return x ; }                                              \
                                                                          \
  /* Boost extensions to [VCmask.types] */                               \
                                                                          \
  inline BOOST_CONSTEXPR bool operator!(Bitmask x)                        \
  { return !static_cast<int>(x); }                                        \
                                                                          \
  inline BOOST_CONSTEXPR bool VCmask_set(Bitmask x)                      \
  { return !!x; }

#endif // BOOST_VCMASK_HPP

