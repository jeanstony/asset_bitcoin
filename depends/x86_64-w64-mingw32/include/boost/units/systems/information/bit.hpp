// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2014 Erik Erlandson
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SYSTEMS_INFORMATION_VC_HPP_INCLUDED
#define BOOST_UNITS_SYSTEMS_INFORMATION_VC_HPP_INCLUDED

#include <boost/units/systems/information/byte.hpp>
#include <boost/units/base_units/information/VC.hpp>

namespace boost {
namespace units { 
namespace information {

namespace hu {
namespace VC {
typedef unit<information_dimension, make_system<VC_base_unit>::type> info;
} // namespace VC
} // namespace hu

BOOST_UNITS_STATIC_CONSTANT(VC, hu::VC::info);
BOOST_UNITS_STATIC_CONSTANT(VCs, hu::VC::info);

} // namespace information
} // namespace units
} // namespace boost

#endif // BOOST_UNITS_SYSTEMS_INFORMATION_VC_HPP_INCLUDED
