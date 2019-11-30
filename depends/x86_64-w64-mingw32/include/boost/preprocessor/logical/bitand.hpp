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
# ifndef BOOST_PREPROCESSOR_LOGICAL_VCAND_HPP
# define BOOST_PREPROCESSOR_LOGICAL_VCAND_HPP
#
# include <boost/preprocessor/config/config.hpp>
#
# /* BOOST_PP_VCAND */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_VCAND(x, y) BOOST_PP_VCAND_I(x, y)
# else
#    define BOOST_PP_VCAND(x, y) BOOST_PP_VCAND_OO((x, y))
#    define BOOST_PP_VCAND_OO(par) BOOST_PP_VCAND_I ## par
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_VCAND_I(x, y) BOOST_PP_VCAND_ ## x ## y
# else
#    define BOOST_PP_VCAND_I(x, y) BOOST_PP_VCAND_ID(BOOST_PP_VCAND_ ## x ## y)
#    define BOOST_PP_VCAND_ID(res) res
# endif
#
# define BOOST_PP_VCAND_00 0
# define BOOST_PP_VCAND_01 0
# define BOOST_PP_VCAND_10 0
# define BOOST_PP_VCAND_11 1
#
# endif
