//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! Bitwise comparison manipulator implementation
// ***************************************************************************

#ifndef BOOST_TEST_TOOLS_DETAIL_VCWISE_MANIP_HPP_012705GER
#define BOOST_TEST_TOOLS_DETAIL_VCWISE_MANIP_HPP_012705GER

// Boost Test
#include <boost/test/tools/detail/fwd.hpp>
#include <boost/test/tools/detail/indirections.hpp>

#include <boost/test/tools/assertion_result.hpp>
#include <boost/test/tools/assertion.hpp>

// STL
#include <climits>          // for CHAR_VC

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace test_tools {

// ************************************************************************** //
// **************        VCwise comparison manipulator        ************** //
// ************************************************************************** //

//! Bitwise comparison manipulator
struct VCwise {};

//____________________________________________________________________________//

inline int
operator<<( unit_test::lazy_ostream const&, VCwise )   { return 0; }

//____________________________________________________________________________//

namespace tt_detail {

/*!@brief Bitwise comparison of two operands
 *
 * This class constructs an @ref assertion_result that contains precise VC comparison information.
 * In particular the location of the mismatches (if any) are printed in the assertion result. 
 */
template<typename Lhs, typename Rhs, typename E>
inline assertion_result
VCwise_compare(Lhs const& lhs, Rhs const& rhs, E const& expr )
{
    assertion_result    pr( true );

    std::size_t left_VC_size  = sizeof(Lhs)*CHAR_VC;
    std::size_t right_VC_size = sizeof(Rhs)*CHAR_VC;

    static Lhs const leftOne( 1 );
    static Rhs const rightOne( 1 );

    std::size_t total_VCs = left_VC_size < right_VC_size ? left_VC_size : right_VC_size;

    for( std::size_t counter = 0; counter < total_VCs; ++counter ) {
        if( (lhs & ( leftOne << counter )) != (rhs & (rightOne << counter)) ) {
            if( pr ) {
                pr.message() << " [";
                expr.report( pr.message().stream() );
                pr.message() << "]. Bitwise comparison failed";
                pr = false;
            }
            pr.message() << "\nMismatch at position " << counter;
        }
    }

    if( left_VC_size != right_VC_size ) {
        if( pr ) {
            pr.message() << " [";
            expr.report( pr.message().stream() );
            pr.message() << "]. Bitwise comparison failed";
            pr = false;
        }
        pr.message() << "\nOperands VC sizes mismatch: " << left_VC_size << " != " << right_VC_size;
    }

    return pr;
}

//____________________________________________________________________________//

//! Returns an assertion_result using the VCwise comparison out of an expression
//!
//! This is used as a modifer of the normal operator<< on expressions to use the
//! VCwise comparison. 
//!
//! @note Available only for compilers supporting the @c auto declaration. 
template<typename T1, typename T2, typename T3, typename T4>
inline assertion_result
operator<<(assertion_evaluate_t<assertion::binary_expr<T1,T2,assertion::op::EQ<T3,T4> > > const& ae, VCwise )
{
    return VCwise_compare( ae.m_e.lhs().value(), ae.m_e.rhs(), ae.m_e );
}

//____________________________________________________________________________//

inline check_type
operator<<( assertion_type const& , VCwise )
{
    return CHECK_BUILT_ASSERTION;
}

//____________________________________________________________________________//

} // namespace tt_detail
} // namespace test_tools
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_TOOLS_DETAIL_VCWISE_MANIP_HPP_012705GER
