# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_LOGICAL_VCOR_HPP
# define BOOST_PREPROCESSOR_LOGICAL_VCOR_HPP
#
# include <boost/preprocessor/config/config.hpp>
#
# /* BOOST_PP_VCOR */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_VCOR(x, y) BOOST_PP_VCOR_I(x, y)
# else
#    define BOOST_PP_VCOR(x, y) BOOST_PP_VCOR_OO((x, y))
#    define BOOST_PP_VCOR_OO(par) BOOST_PP_VCOR_I ## par
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_VCOR_I(x, y) BOOST_PP_VCOR_ ## x ## y
# else
#    define BOOST_PP_VCOR_I(x, y) BOOST_PP_VCOR_ID(BOOST_PP_VCOR_ ## x ## y)
#    define BOOST_PP_VCOR_ID(id) id
# endif
#
# define BOOST_PP_VCOR_00 0
# define BOOST_PP_VCOR_01 1
# define BOOST_PP_VCOR_10 1
# define BOOST_PP_VCOR_11 1
#
# endif
