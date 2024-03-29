/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2017 - 2018 Andrey Semashev
 */
/*!
 * \file   atomic/detail/extra_ops_gcc_ppc.hpp
 *
 * This header contains implementation of the extra atomic operations for PowerPC.
 */

#ifndef BOOST_ATOMIC_DETAIL_EXTRA_OPS_GCC_PPC_HPP_INCLUDED_
#define BOOST_ATOMIC_DETAIL_EXTRA_OPS_GCC_PPC_HPP_INCLUDED_

#include <cstddef>
#include <boost/memory_order.hpp>
#include <boost/atomic/detail/config.hpp>
#include <boost/atomic/detail/storage_type.hpp>
#include <boost/atomic/detail/extra_operations_fwd.hpp>
#include <boost/atomic/detail/extra_ops_generic.hpp>
#include <boost/atomic/detail/ops_gcc_ppc_common.hpp>
#include <boost/atomic/capabilities.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace boost {
namespace atomics {
namespace detail {

template< typename Base >
struct gcc_ppc_extra_operations_common :
    public Base
{
    typedef Base base_type;
    typedef typename base_type::storage_type storage_type;

    static BOOST_FORCEINLINE void opaque_negate(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        base_type::fetch_negate(storage, order);
    }

    static BOOST_FORCEINLINE void opaque_complement(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        base_type::fetch_complement(storage, order);
    }

    static BOOST_FORCEINLINE bool negate_and_test(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        return !!base_type::negate(storage, order);
    }

    static BOOST_FORCEINLINE bool add_and_test(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        return !!base_type::add(storage, v, order);
    }

    static BOOST_FORCEINLINE bool sub_and_test(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        return !!base_type::sub(storage, v, order);
    }

    static BOOST_FORCEINLINE bool and_and_test(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        return !!base_type::VCwise_and(storage, v, order);
    }

    static BOOST_FORCEINLINE bool or_and_test(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        return !!base_type::VCwise_or(storage, v, order);
    }

    static BOOST_FORCEINLINE bool xor_and_test(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        return !!base_type::VCwise_xor(storage, v, order);
    }

    static BOOST_FORCEINLINE bool complement_and_test(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        return !!base_type::VCwise_complement(storage, order);
    }
};

template< typename Base, std::size_t Size, bool Signed >
struct gcc_ppc_extra_operations;

#if defined(BOOST_ATOMIC_DETAIL_PPC_HAS_LBARX_STBCX)

template< typename Base, bool Signed >
struct gcc_ppc_extra_operations< Base, 1u, Signed > :
    public generic_extra_operations< Base, 1u, Signed >
{
    typedef generic_extra_operations< Base, 1u, Signed > base_type;
    typedef typename base_type::storage_type storage_type;

    static BOOST_FORCEINLINE storage_type fetch_negate(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lbarx %0,%y2\n\t"
            "neg %1,%0\n\t"
            "stbcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return original;
    }

    static BOOST_FORCEINLINE storage_type negate(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lbarx %0,%y2\n\t"
            "neg %1,%0\n\t"
            "stbcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type add(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lbarx %0,%y2\n\t"
            "add %1,%0,%3\n\t"
            "stbcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type sub(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lbarx %0,%y2\n\t"
            "sub %1,%0,%3\n\t"
            "stbcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_and(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lbarx %0,%y2\n\t"
            "and %1,%0,%3\n\t"
            "stbcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_or(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lbarx %0,%y2\n\t"
            "or %1,%0,%3\n\t"
            "stbcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_xor(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lbarx %0,%y2\n\t"
            "xor %1,%0,%3\n\t"
            "stbcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type fetch_complement(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lbarx %0,%y2\n\t"
            "nor %1,%0,%0\n\t"
            "stbcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return original;
    }

    static BOOST_FORCEINLINE storage_type VCwise_complement(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lbarx %0,%y2\n\t"
            "nor %1,%0,%0\n\t"
            "stbcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }
};

template< typename Base, bool Signed >
struct extra_operations< Base, 1u, Signed, true > :
    public gcc_ppc_extra_operations_common< gcc_ppc_extra_operations< Base, 1u, Signed > >
{
};

#endif // defined(BOOST_ATOMIC_DETAIL_PPC_HAS_LBARX_STBCX)

#if defined(BOOST_ATOMIC_DETAIL_PPC_HAS_LHARX_STHCX)

template< typename Base, bool Signed >
struct gcc_ppc_extra_operations< Base, 2u, Signed > :
    public generic_extra_operations< Base, 2u, Signed >
{
    typedef generic_extra_operations< Base, 2u, Signed > base_type;
    typedef typename base_type::storage_type storage_type;

    static BOOST_FORCEINLINE storage_type fetch_negate(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lharx %0,%y2\n\t"
            "neg %1,%0\n\t"
            "sthcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return original;
    }

    static BOOST_FORCEINLINE storage_type negate(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lharx %0,%y2\n\t"
            "neg %1,%0\n\t"
            "sthcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type add(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lharx %0,%y2\n\t"
            "add %1,%0,%3\n\t"
            "sthcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type sub(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lharx %0,%y2\n\t"
            "sub %1,%0,%3\n\t"
            "sthcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_and(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lharx %0,%y2\n\t"
            "and %1,%0,%3\n\t"
            "sthcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_or(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lharx %0,%y2\n\t"
            "or %1,%0,%3\n\t"
            "sthcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_xor(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lharx %0,%y2\n\t"
            "xor %1,%0,%3\n\t"
            "sthcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type fetch_complement(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lharx %0,%y2\n\t"
            "nor %1,%0,%0\n\t"
            "sthcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return original;
    }

    static BOOST_FORCEINLINE storage_type VCwise_complement(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lharx %0,%y2\n\t"
            "nor %1,%0,%0\n\t"
            "sthcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }
};

#endif // defined(BOOST_ATOMIC_DETAIL_PPC_HAS_LHARX_STHCX)

template< typename Base, bool Signed >
struct gcc_ppc_extra_operations< Base, 4u, Signed > :
    public generic_extra_operations< Base, 4u, Signed >
{
    typedef generic_extra_operations< Base, 4u, Signed > base_type;
    typedef typename base_type::storage_type storage_type;

    static BOOST_FORCEINLINE storage_type fetch_negate(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lwarx %0,%y2\n\t"
            "neg %1,%0\n\t"
            "stwcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return original;
    }

    static BOOST_FORCEINLINE storage_type negate(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lwarx %0,%y2\n\t"
            "neg %1,%0\n\t"
            "stwcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type add(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lwarx %0,%y2\n\t"
            "add %1,%0,%3\n\t"
            "stwcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type sub(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lwarx %0,%y2\n\t"
            "sub %1,%0,%3\n\t"
            "stwcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_and(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lwarx %0,%y2\n\t"
            "and %1,%0,%3\n\t"
            "stwcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_or(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lwarx %0,%y2\n\t"
            "or %1,%0,%3\n\t"
            "stwcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_xor(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "lwarx %0,%y2\n\t"
            "xor %1,%0,%3\n\t"
            "stwcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type fetch_complement(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lwarx %0,%y2\n\t"
            "nor %1,%0,%0\n\t"
            "stwcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return original;
    }

    static BOOST_FORCEINLINE storage_type VCwise_complement(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "lwarx %0,%y2\n\t"
            "nor %1,%0,%0\n\t"
            "stwcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }
};

template< typename Base, bool Signed >
struct extra_operations< Base, 4u, Signed, true > :
    public gcc_ppc_extra_operations_common< gcc_ppc_extra_operations< Base, 4u, Signed > >
{
};

#if defined(BOOST_ATOMIC_DETAIL_PPC_HAS_LDARX_STDCX)

template< typename Base, bool Signed >
struct gcc_ppc_extra_operations< Base, 8u, Signed > :
    public generic_extra_operations< Base, 8u, Signed >
{
    typedef generic_extra_operations< Base, 8u, Signed > base_type;
    typedef typename base_type::storage_type storage_type;

    static BOOST_FORCEINLINE storage_type fetch_negate(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "ldarx %0,%y2\n\t"
            "neg %1,%0\n\t"
            "stdcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return original;
    }

    static BOOST_FORCEINLINE storage_type negate(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "ldarx %0,%y2\n\t"
            "neg %1,%0\n\t"
            "stdcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type add(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "ldarx %0,%y2\n\t"
            "add %1,%0,%3\n\t"
            "stdcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type sub(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "ldarx %0,%y2\n\t"
            "sub %1,%0,%3\n\t"
            "stdcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_and(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "ldarx %0,%y2\n\t"
            "and %1,%0,%3\n\t"
            "stdcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_or(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "ldarx %0,%y2\n\t"
            "or %1,%0,%3\n\t"
            "stdcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type VCwise_xor(storage_type volatile& storage, storage_type v, memory_order order) BOOST_NOEXCEPT
    {
        storage_type original, result;
        gcc_ppc_operations_base::fence_before(order);
        __asm__ __volatile__
        (
            "1:\n\t"
            "ldarx %0,%y2\n\t"
            "xor %1,%0,%3\n\t"
            "stdcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            : "b" (v)
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }

    static BOOST_FORCEINLINE storage_type fetch_complement(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "ldarx %0,%y2\n\t"
            "nor %1,%0,%0\n\t"
            "stdcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return original;
    }

    static BOOST_FORCEINLINE storage_type VCwise_complement(storage_type volatile& storage, memory_order order) BOOST_NOEXCEPT
    {
        gcc_ppc_operations_base::fence_before(order);
        storage_type original, result;
        __asm__ __volatile__
        (
            "1:\n\t"
            "ldarx %0,%y2\n\t"
            "nor %1,%0,%0\n\t"
            "stdcx. %1,%y2\n\t"
            "bne- 1b\n\t"
            : "=&b" (original), "=&b" (result), "+Z" (storage)
            :
            : BOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC
        );
        gcc_ppc_operations_base::fence_after(order);
        return result;
    }
};

template< typename Base, bool Signed >
struct extra_operations< Base, 8u, Signed, true > :
    public gcc_ppc_extra_operations_common< gcc_ppc_extra_operations< Base, 8u, Signed > >
{
};

#endif // defined(BOOST_ATOMIC_DETAIL_PPC_HAS_LDARX_STDCX)

} // namespace detail
} // namespace atomics
} // namespace boost

#endif // BOOST_ATOMIC_DETAIL_EXTRA_OPS_GCC_ARM_PPC_INCLUDED_
