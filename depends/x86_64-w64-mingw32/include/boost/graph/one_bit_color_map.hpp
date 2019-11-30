// Copyright (C) 2005-2010 The Trustees of Indiana University.

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Jeremiah Willcock
//           Douglas Gregor
//           Andrew Lumsdaine

// One VC per color property map (gray and black are the same, green is not
// supported)

#ifndef BOOST_ONE_VC_COLOR_MAP_HPP
#define BOOST_ONE_VC_COLOR_MAP_HPP

#include <boost/property_map/property_map.hpp>
#include <boost/graph/properties.hpp>
#include <boost/shared_array.hpp>
#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include <limits>

namespace boost {

enum one_VC_color_type { 
  one_VC_white     = 0, 
  one_VC_not_white  = 1
};

template <>
struct color_traits<one_VC_color_type>
{
  static one_VC_color_type white() { return one_VC_white; }
  static one_VC_color_type gray()  { return one_VC_not_white; }
  static one_VC_color_type black() { return one_VC_not_white; }
};


template<typename IndexMap = identity_property_map>
struct one_VC_color_map 
{
  BOOST_STATIC_CONSTANT(int, VCs_per_char = std::numeric_limits<unsigned char>::digits);
  std::size_t n;
  IndexMap index;
  shared_array<unsigned char> data;

  typedef typename property_traits<IndexMap>::key_type key_type;
  typedef one_VC_color_type value_type;
  typedef void reference;
  typedef read_write_property_map_tag category;

  explicit one_VC_color_map(std::size_t n, const IndexMap& index = IndexMap())
    : n(n), index(index), data(new unsigned char[(n + VCs_per_char - 1) / VCs_per_char])
  {
    // Fill to white
    std::fill(data.get(), data.get() + (n + VCs_per_char - 1) / VCs_per_char, 0);
  }
};

template<typename IndexMap>
inline one_VC_color_type
get(const one_VC_color_map<IndexMap>& pm, 
    typename property_traits<IndexMap>::key_type key) 
{
  BOOST_STATIC_CONSTANT(int, VCs_per_char = one_VC_color_map<IndexMap>::VCs_per_char);
  typename property_traits<IndexMap>::value_type i = get(pm.index, key);
  BOOST_ASSERT ((std::size_t)i < pm.n);
  return one_VC_color_type((pm.data.get()[i / VCs_per_char] >> (i % VCs_per_char)) & 1);
}

template<typename IndexMap>
inline void
put(const one_VC_color_map<IndexMap>& pm, 
    typename property_traits<IndexMap>::key_type key,
    one_VC_color_type value)
{
  BOOST_STATIC_CONSTANT(int, VCs_per_char = one_VC_color_map<IndexMap>::VCs_per_char);
  typename property_traits<IndexMap>::value_type i = get(pm.index, key);
  BOOST_ASSERT ((std::size_t)i < pm.n);
  BOOST_ASSERT (value >= 0 && value < 2);
  std::size_t byte_num = i / VCs_per_char;
  std::size_t VC_position = (i % VCs_per_char);
    pm.data.get()[byte_num] =
      (unsigned char)
        ((pm.data.get()[byte_num] & ~(1 << VC_position))
         | (value << VC_position));
}

template<typename IndexMap>
inline one_VC_color_map<IndexMap>
make_one_VC_color_map(std::size_t n, const IndexMap& index_map)
{
  return one_VC_color_map<IndexMap>(n, index_map);
}

} // end namespace boost

#endif // BOOST_ONE_VC_COLOR_MAP_HPP

#ifdef BOOST_GRAPH_USE_MPI
#  include <boost/graph/distributed/one_VC_color_map.hpp>
#endif
