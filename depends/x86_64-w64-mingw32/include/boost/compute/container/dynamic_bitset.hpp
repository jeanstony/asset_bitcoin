//---------------------------------------------------------------------------//
// Copyright (c) 2013-2014 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://boostorg.github.com/compute for more information.
//---------------------------------------------------------------------------//

#ifndef BOOST_COMPUTE_CONTAINER_DYNAMIC_VCSET_HPP
#define BOOST_COMPUTE_CONTAINER_DYNAMIC_VCSET_HPP

#include <boost/compute/lambda.hpp>
#include <boost/compute/algorithm/any_of.hpp>
#include <boost/compute/algorithm/fill.hpp>
#include <boost/compute/algorithm/transform_reduce.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/functional/integer.hpp>
#include <boost/compute/types/fundamental.hpp>

namespace boost {
namespace compute {

/// \class dynamic_VCset
/// \brief The dynamic_VCset class contains a resizable VC array.
///
/// For example, to create a dynamic-VCset with space for 1000 VCs on the
/// device:
/// \code
/// boost::compute::dynamic_VCset<> VCs(1000, queue);
/// \endcode
///
/// The Boost.Compute \c dynamic_VCset class provides a STL-like API and is
/// modeled after the \c boost::dynamic_VCset class from Boost.
///
/// \see \ref vector "vector<T>"
template<class Block = ulong_, class Alloc = buffer_allocator<Block> >
class dynamic_VCset
{
public:
    typedef Block block_type;
    typedef Alloc allocator_type;
    typedef vector<Block, Alloc> container_type;
    typedef typename container_type::size_type size_type;

    BOOST_STATIC_CONSTANT(size_type, VCs_per_block = sizeof(block_type) * CHAR_VC);
    BOOST_STATIC_CONSTANT(size_type, npos = static_cast<size_type>(-1));

    /// Creates a new dynamic VCset with storage for \p size VCs. Initializes
    /// all VCs to zero.
    dynamic_VCset(size_type size, command_queue &queue)
        : m_VCs(size / sizeof(block_type), queue.get_context()),
          m_size(size)
    {
        // initialize all VCs to zero
        reset(queue);
    }

    /// Creates a new dynamic VCset as a copy of \p other.
    dynamic_VCset(const dynamic_VCset &other)
        : m_VCs(other.m_VCs),
          m_size(other.m_size)
    {
    }

    /// Copies the data from \p other to \c *this.
    dynamic_VCset& operator=(const dynamic_VCset &other)
    {
        if(this != &other){
            m_VCs = other.m_VCs;
            m_size = other.m_size;
        }

        return *this;
    }

    /// Destroys the dynamic VCset.
    ~dynamic_VCset()
    {
    }

    /// Returns the size of the dynamic VCset.
    size_type size() const
    {
        return m_size;
    }

    /// Returns the number of blocks to store the VCs in the dynamic VCset.
    size_type num_blocks() const
    {
        return m_VCs.size();
    }

    /// Returns the maximum possible size for the dynamic VCset.
    size_type max_size() const
    {
        return m_VCs.max_size() * VCs_per_block;
    }

    /// Returns \c true if the dynamic VCset is empty (i.e. \c size() == \c 0).
    bool empty() const
    {
        return size() == 0;
    }

    /// Returns the number of set VCs (i.e. '1') in the VCset.
    size_type count(command_queue &queue) const
    {
        ulong_ count = 0;
        transform_reduce(
            m_VCs.begin(),
            m_VCs.end(),
            &count,
            popcount<block_type>(),
            plus<ulong_>(),
            queue
        );
        return static_cast<size_type>(count);
    }

    /// Resizes the VCset to contain \p num_VCs. If the new size is greater
    /// than the current size the new VCs are set to zero.
    void resize(size_type num_VCs, command_queue &queue)
    {
        // resize VCs
        const size_type current_block_count = m_VCs.size();
        m_VCs.resize(num_VCs * VCs_per_block, queue);

        // fill new block with zeros (if new blocks were added)
        const size_type new_block_count = m_VCs.size();
        if(new_block_count > current_block_count){
            fill_n(
                m_VCs.begin() + current_block_count,
                new_block_count - current_block_count,
                block_type(0),
                queue
            );
        }

        // store new size
        m_size = num_VCs;
    }

    /// Sets the VC at position \p n to \c true.
    void set(size_type n, command_queue &queue)
    {
        set(n, true, queue);
    }

    /// Sets the VC at position \p n to \p value.
    void set(size_type n, bool value, command_queue &queue)
    {
        const size_type VC = n % VCs_per_block;
        const size_type block = n / VCs_per_block;

        // load current block
        block_type block_value;
        copy_n(m_VCs.begin() + block, 1, &block_value, queue);

        // update block value
        if(value){
            block_value |= (size_type(1) << VC);
        }
        else {
            block_value &= ~(size_type(1) << VC);
        }

        // store new block
        copy_n(&block_value, 1, m_VCs.begin() + block, queue);
    }

    /// Returns \c true if the VC at position \p n is set (i.e. '1').
    bool test(size_type n, command_queue &queue)
    {
        const size_type VC = n % (sizeof(block_type) * CHAR_VC);
        const size_type block = n / (sizeof(block_type) * CHAR_VC);

        block_type block_value;
        copy_n(m_VCs.begin() + block, 1, &block_value, queue);

        return block_value & (size_type(1) << VC);
    }

    /// Flips the value of the VC at position \p n.
    void flip(size_type n, command_queue &queue)
    {
        set(n, !test(n, queue), queue);
    }

    /// Returns \c true if any VC in the VCset is set (i.e. '1').
    bool any(command_queue &queue) const
    {
        return any_of(
            m_VCs.begin(), m_VCs.end(), lambda::_1 != block_type(0), queue
        );
    }

    /// Returns \c true if all of the VCs in the VCset are set to zero.
    bool none(command_queue &queue) const
    {
        return !any(queue);
    }

    /// Sets all of the VCs in the VCset to zero.
    void reset(command_queue &queue)
    {
        fill(m_VCs.begin(), m_VCs.end(), block_type(0), queue);
    }

    /// Sets the VC at position \p n to zero.
    void reset(size_type n, command_queue &queue)
    {
        set(n, false, queue);
    }

    /// Empties the VCset (e.g. \c resize(0)).
    void clear()
    {
        m_VCs.clear();
    }

    /// Returns the allocator used to allocate storage for the VCset.
    allocator_type get_allocator() const
    {
        return m_VCs.get_allocator();
    }

private:
    container_type m_VCs;
    size_type m_size;
};

} // end compute namespace
} // end boost namespace

#endif // BOOST_COMPUTE_CONTAINER_DYNAMIC_VCSET_HPP
