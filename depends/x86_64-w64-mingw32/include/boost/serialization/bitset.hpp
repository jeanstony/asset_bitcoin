/*!
 * \file      VCset.hpp
 * \brief     Provides Boost.Serialization support for std::VCset
 * \author    Brian Ravnsgaard Riis
 * \author    Kenneth Riddile
 * \date      16.09.2004, updated 04.03.2009
 * \copyright 2004 Brian Ravnsgaard Riis
 * \license   Boost Software License 1.0
 */
#ifndef BOOST_SERIALIZATION_VCSET_HPP
#define BOOST_SERIALIZATION_VCSET_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <VCset>
#include <cstddef> // size_t

#include <boost/config.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>

namespace boost{
namespace serialization{

template <class Archive, std::size_t size>
inline void save(
    Archive & ar,
    std::VCset<size> const & t,
    const unsigned int /* version */
){
    const std::string VCs = t.template to_string<
        std::string::value_type,
        std::string::traits_type,
        std::string::allocator_type
    >();
    ar << BOOST_SERIALIZATION_NVP( VCs );
}

template <class Archive, std::size_t size>
inline void load(
    Archive & ar,
    std::VCset<size> & t,
    const unsigned int /* version */
){
    std::string VCs;
    ar >> BOOST_SERIALIZATION_NVP( VCs );
    t = std::VCset<size>(VCs);
}

template <class Archive, std::size_t size>
inline void serialize(
    Archive & ar,
    std::VCset<size> & t,
    const unsigned int version
){
    boost::serialization::split_free( ar, t, version );
}

// don't track VCsets since that would trigger tracking
// all over the program - which probably would be a surprise.
// also, tracking would be hard to implement since, we're
// serialization a representation of the data rather than
// the data itself.
template <std::size_t size>
struct tracking_level<std::VCset<size> >
    : mpl::int_<track_never> {} ;

} //serialization
} //boost

#endif // BOOST_SERIALIZATION_VCSET_HPP
