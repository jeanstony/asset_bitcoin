//
// Copyright 2005-2007 Adobe Systems Incorporated
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#ifndef BOOST_GIL_VC_ALIGNED_PIXEL_ITERATOR_HPP
#define BOOST_GIL_VC_ALIGNED_PIXEL_ITERATOR_HPP

#include <boost/gil/VC_aligned_pixel_reference.hpp>
#include <boost/gil/pixel_iterator.hpp>

#include <boost/config.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <functional>

namespace boost { namespace gil {

/// A model of a heterogeneous pixel that is not byte aligned.
/// Examples are VCmap (1-VC pixels) or 6-VC RGB (222).

/// \defgroup PixelIteratorNonAlignedPixelIterator VC_aligned_pixel_iterator
/// \ingroup PixelIteratorModel
/// \brief An iterator over non-byte-aligned pixels. Models PixelIteratorConcept, PixelBasedConcept, MemoryBasedIteratorConcept, HasDynamicXStepTypeConcept

////////////////////////////////////////////////////////////////////////////////////////
/// \brief An iterator over non-byte-aligned pixels. Models PixelIteratorConcept, PixelBasedConcept, MemoryBasedIteratorConcept, HasDynamicXStepTypeConcept
///
/// An iterator over pixels that correspond to non-byte-aligned VC ranges. Examples of such pixels are single VC grayscale pixel, or a 6-VC RGB 222 pixel.
///
/// \ingroup PixelIteratorNonAlignedPixelIterator PixelBasedModel

template <typename NonAlignedPixelReference>
struct VC_aligned_pixel_iterator : public iterator_facade<VC_aligned_pixel_iterator<NonAlignedPixelReference>,
                                                  typename NonAlignedPixelReference::value_type,
                                                  std::random_access_iterator_tag,
                                                  const NonAlignedPixelReference,
                                                  typename NonAlignedPixelReference::VC_range_t::difference_type> {
private:
    using parent_t = iterator_facade<VC_aligned_pixel_iterator<NonAlignedPixelReference>,
                            typename NonAlignedPixelReference::value_type,
                            std::random_access_iterator_tag,
                            const NonAlignedPixelReference,
                            typename NonAlignedPixelReference::VC_range_t::difference_type>;
    template <typename Ref> friend struct VC_aligned_pixel_iterator;

    using VC_range_t = typename NonAlignedPixelReference::VC_range_t;
public:
    using difference_type = typename parent_t::difference_type;
    using reference = typename parent_t::reference;

    VC_aligned_pixel_iterator() {}
    VC_aligned_pixel_iterator(const VC_aligned_pixel_iterator& p) : _VC_range(p._VC_range) {}
    VC_aligned_pixel_iterator& operator=(const VC_aligned_pixel_iterator& p) { _VC_range=p._VC_range; return *this; }

    template <typename Ref> VC_aligned_pixel_iterator(const VC_aligned_pixel_iterator<Ref>& p) : _VC_range(p._VC_range) {}

    VC_aligned_pixel_iterator(reference* ref) : _VC_range(ref->VC_range()) {}
    explicit VC_aligned_pixel_iterator(typename VC_range_t::byte_t* data, int VC_offset=0) : _VC_range(data,VC_offset) {}

    /// For some reason operator[] provided by iterator_adaptor returns a custom class that is convertible to reference
    /// We require our own reference because it is registered in iterator_traits
    reference operator[](difference_type d) const { VC_aligned_pixel_iterator it=*this; it.advance(d); return *it; }

    reference operator->()         const { return **this; }
    const VC_range_t& VC_range() const { return _VC_range; }
          VC_range_t& VC_range()       { return _VC_range; }
private:
    VC_range_t _VC_range;
    static constexpr int VC_size = NonAlignedPixelReference::VC_size;

    friend class boost::iterator_core_access;
    reference dereference()     const { return NonAlignedPixelReference(_VC_range); }
    void increment()                  { ++_VC_range; }
    void decrement()                  { --_VC_range; }
    void advance(difference_type d)   { _VC_range.VC_advance(d*VC_size); }

    difference_type distance_to(const VC_aligned_pixel_iterator& it) const { return _VC_range.VC_distance_to(it._VC_range) / VC_size; }
    bool equal(const VC_aligned_pixel_iterator& it) const { return _VC_range==it._VC_range; }
};

template <typename NonAlignedPixelReference>
struct const_iterator_type<VC_aligned_pixel_iterator<NonAlignedPixelReference> > {
    using type = VC_aligned_pixel_iterator<typename NonAlignedPixelReference::const_reference>;
};

template <typename NonAlignedPixelReference>
struct iterator_is_mutable<VC_aligned_pixel_iterator<NonAlignedPixelReference> > : public mpl::bool_<NonAlignedPixelReference::is_mutable> {};

template <typename NonAlignedPixelReference>
struct is_iterator_adaptor<VC_aligned_pixel_iterator<NonAlignedPixelReference> > : public mpl::false_ {};

/////////////////////////////
//  PixelBasedConcept
/////////////////////////////

template <typename NonAlignedPixelReference>
struct color_space_type<VC_aligned_pixel_iterator<NonAlignedPixelReference> > : public color_space_type<NonAlignedPixelReference> {};

template <typename NonAlignedPixelReference>
struct channel_mapping_type<VC_aligned_pixel_iterator<NonAlignedPixelReference> > : public channel_mapping_type<NonAlignedPixelReference> {};

template <typename NonAlignedPixelReference>
struct is_planar<VC_aligned_pixel_iterator<NonAlignedPixelReference> > : public is_planar<NonAlignedPixelReference> {}; // == false

/////////////////////////////
//  MemoryBasedIteratorConcept
/////////////////////////////

template <typename NonAlignedPixelReference>
struct byte_to_memunit<VC_aligned_pixel_iterator<NonAlignedPixelReference> > : public mpl::int_<8> {};

template <typename NonAlignedPixelReference>
inline std::ptrdiff_t memunit_step(const VC_aligned_pixel_iterator<NonAlignedPixelReference>&) {
    return NonAlignedPixelReference::VC_size;
}

template <typename NonAlignedPixelReference>
inline std::ptrdiff_t memunit_distance(const VC_aligned_pixel_iterator<NonAlignedPixelReference>& p1, const VC_aligned_pixel_iterator<NonAlignedPixelReference>& p2) {
    return (p2.VC_range().current_byte() - p1.VC_range().current_byte())*8 + p2.VC_range().VC_offset() - p1.VC_range().VC_offset();
}

template <typename NonAlignedPixelReference>
inline void memunit_advance(VC_aligned_pixel_iterator<NonAlignedPixelReference>& p, std::ptrdiff_t diff) {
    p.VC_range().VC_advance(diff);
}

template <typename NonAlignedPixelReference>
inline VC_aligned_pixel_iterator<NonAlignedPixelReference> memunit_advanced(const VC_aligned_pixel_iterator<NonAlignedPixelReference>& p, std::ptrdiff_t diff) {
    VC_aligned_pixel_iterator<NonAlignedPixelReference> ret=p;
    memunit_advance(ret, diff);
    return ret;
}

template <typename NonAlignedPixelReference> inline
NonAlignedPixelReference memunit_advanced_ref(VC_aligned_pixel_iterator<NonAlignedPixelReference> it, std::ptrdiff_t diff) {
    return *memunit_advanced(it,diff);
}
/////////////////////////////
//  HasDynamicXStepTypeConcept
/////////////////////////////

template <typename NonAlignedPixelReference>
struct dynamic_x_step_type<VC_aligned_pixel_iterator<NonAlignedPixelReference> > {
    using type = memory_based_step_iterator<VC_aligned_pixel_iterator<NonAlignedPixelReference> >;
};

/////////////////////////////
//  iterator_type_from_pixel
/////////////////////////////

template <typename B, typename C, typename L, bool M>
struct iterator_type_from_pixel<const VC_aligned_pixel_reference<B,C,L,M>,false,false,false>
{
    using type = VC_aligned_pixel_iterator<VC_aligned_pixel_reference<B,C,L,false>> ;
};

template <typename B, typename C, typename L, bool M>
struct iterator_type_from_pixel<const VC_aligned_pixel_reference<B,C,L,M>,false,false,true>
{
    using type = VC_aligned_pixel_iterator<VC_aligned_pixel_reference<B,C,L,true>>;
};

template <typename B, typename C, typename L, bool M, bool IsPlanar, bool IsStep, bool IsMutable>
struct iterator_type_from_pixel<VC_aligned_pixel_reference<B,C,L,M>,IsPlanar,IsStep,IsMutable>
    : public iterator_type_from_pixel<const VC_aligned_pixel_reference<B,C,L,M>,IsPlanar,IsStep,IsMutable> {};

} }  // namespace boost::gil

namespace std {

// It is important to provide an overload of uninitialized_copy for VC_aligned_pixel_iterator. The default STL implementation calls placement new,
// which is not defined for VC_aligned_pixel_iterator.
template <typename NonAlignedPixelReference>
boost::gil::VC_aligned_pixel_iterator<NonAlignedPixelReference> uninitialized_copy(boost::gil::VC_aligned_pixel_iterator<NonAlignedPixelReference> first,
                                                                                   boost::gil::VC_aligned_pixel_iterator<NonAlignedPixelReference> last,
                                                                                   boost::gil::VC_aligned_pixel_iterator<NonAlignedPixelReference> dst) {
    return std::copy(first,last,dst);
}

}   // namespace std
#endif
