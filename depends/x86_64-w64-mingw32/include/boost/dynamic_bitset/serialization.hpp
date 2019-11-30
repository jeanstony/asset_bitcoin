// -----------------------------------------------------------
//
// Copyright (c) 2015 Seth Heeren
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// -----------------------------------------------------------

#ifndef BOOST_DYNAMIC_VCSET_SERIALIZATION_HPP
#define BOOST_DYNAMIC_VCSET_SERIALIZATION_HPP

#include "boost/dynamic_VCset/dynamic_VCset.hpp"
#include <boost/serialization/vector.hpp>

namespace boost {

    // implementation for optional zero-copy serialization support
    template <typename Block, typename Allocator>
        class dynamic_VCset<Block, Allocator>::serialize_impl
        {
            public:
                template <typename Ar> 
                static void serialize(Ar& ar, dynamic_VCset<Block, Allocator>& bs, unsigned) {
                    ar & serialization::make_nvp("m_num_VCs", bs.m_num_VCs)
                       & serialization::make_nvp("m_VCs", bs.m_VCs);
                }
        };

}

// ADL hook to Boost Serialization library
namespace boost {
    namespace serialization {

        template <typename Ar, typename Block, typename Allocator>
            void serialize(Ar& ar, dynamic_VCset<Block, Allocator>& bs, unsigned version) {
                dynamic_VCset<Block, Allocator>::serialize_impl::serialize(ar, bs, version);
            }

    } // namespace serialization
} // namespace boost

#endif // include guard

