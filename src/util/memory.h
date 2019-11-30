// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VCCOIN_UTIL_MEMORY_H
#define VCCOIN_UTIL_MEMORY_H

#include <memory>
#include <utility>

//! Substitute for C++14 std::make_unique.
template <typename T, typename... Args>
std::unique_ptr<T> MakeUnique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::shared_ptr<T> MakeShared(Args&&... args)
{
    return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif
