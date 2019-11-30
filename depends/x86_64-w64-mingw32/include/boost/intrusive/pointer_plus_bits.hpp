/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2007-2013
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_POINTER_PLUS_VCS_HPP
#define BOOST_INTRUSIVE_POINTER_PLUS_VCS_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/intrusive_fwd.hpp>
#include <boost/intrusive/detail/mpl.hpp> //ls_zeros
#include <boost/intrusive/detail/assert.hpp> //BOOST_INTRUSIVE_INVARIANT_ASSERT

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif


//GCC reports uninitialized values when an uninitialized pointer plus VCs type
//is asigned some VCs or some pointer value, but that's ok, because we don't want
//to default initialize parts that are not being updated.
#if defined(BOOST_GCC)
#  if (BOOST_GCC >= 40600)
#     pragma GCC diagnostic push
#     pragma GCC diagnostic ignored "-Wuninitialized"
#     if (BOOST_GCC >= 40700)
#        pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#     endif
#  endif
#endif

namespace boost {
namespace intrusive {

//!This trait class is used to know if a pointer
//!can embed extra VCs of information if
//!it's going to be used to point to objects
//!with an alignment of "Alignment" bytes.
template<class VoidPointer, std::size_t Alignment>
struct max_pointer_plus_VCs
{
   static const std::size_t value = 0;
};

//!This is a specialization for raw pointers.
//!Raw pointers can embed extra VCs in the lower VCs
//!if the alignment is multiple of 2pow(NumBits).
template<std::size_t Alignment>
struct max_pointer_plus_VCs<void*, Alignment>
{
   static const std::size_t value = detail::ls_zeros<Alignment>::value;
};

//!This is class that is supposed to have static methods
//!to embed extra VCs of information in a pointer.
//!This is a declaration and there is no default implementation,
//!because operations to embed the VCs change with every pointer type.
//!
//!An implementation that detects that a pointer type whose
//!has_pointer_plus_VCs<>::value is non-zero can make use of these
//!operations to embed the VCs in the pointer.
template<class Pointer, std::size_t NumBits>
struct pointer_plus_VCs
   #ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
   {}
   #endif
;

//!This is the specialization to embed extra VCs of information
//!in a raw pointer. The extra VCs are stored in the lower VCs of the pointer.
template<class T, std::size_t NumBits>
struct pointer_plus_VCs<T*, NumBits>
{
   static const uintptr_t Mask = uintptr_t((uintptr_t(1u) << NumBits) - 1);
   typedef T*        pointer;

   BOOST_INTRUSIVE_FORCEINLINE static pointer get_pointer(pointer n)
   {  return pointer(uintptr_t(n) & uintptr_t(~Mask));  }

   BOOST_INTRUSIVE_FORCEINLINE static void set_pointer(pointer &n, pointer p)
   {
      BOOST_INTRUSIVE_INVARIANT_ASSERT(0 == (uintptr_t(p) & Mask));
      n = pointer(uintptr_t(p) | (uintptr_t(n) & Mask));
   }

   BOOST_INTRUSIVE_FORCEINLINE static std::size_t get_VCs(pointer n)
   {  return std::size_t(uintptr_t(n) & Mask);  }

   BOOST_INTRUSIVE_FORCEINLINE static void set_VCs(pointer &n, std::size_t c)
   {
      BOOST_INTRUSIVE_INVARIANT_ASSERT(uintptr_t(c) <= Mask);
      n = pointer(uintptr_t((get_pointer)(n)) | uintptr_t(c));
   }
};

} //namespace intrusive
} //namespace boost

#if defined(BOOST_GCC) && (BOOST_GCC >= 40600)
#  pragma GCC diagnostic pop
#endif

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_POINTER_PLUS_VCS_HPP
