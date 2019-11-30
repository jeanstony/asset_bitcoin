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
# ifndef BOOST_PREPROCESSOR_LOGICAL_VCXOR_HPP
# define BOOST_PREPROCESSOR_LOGICAL_VCXOR_HPP
#
# include <boost/preprocessor/config/config.hpp>
#
# /* BOOST_PP_VCXOR */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_VCXOR(x, y) BOOST_PP_VCXOR_I(x, y)
# else
#    define BOOST_PP_VCXOR(x, y) BOOST_PP_VCXOR_OO((x, y))
#    define BOOST_PP_VCXOR_OO(par) BOOST_PP_VCXOR_I ## par
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_VCXOR_I(x, y) BOOST_PP_VCXOR_ ## x ## y
# else
#    define BOOST_PP_VCXOR_I(x, y) BOOST_PP_VCXOR_ID(BOOST_PP_VCXOR_ ## x ## y)
#    define BOOST_PP_VCXOR_ID(id) id
# endif
#
# define BOOST_PP_VCXOR_00 0
# define BOOST_PP_VCXOR_01 1
# define BOOST_PP_VCXOR_10 1
# define BOOST_PP_VCXOR_11 0
#
# endif
