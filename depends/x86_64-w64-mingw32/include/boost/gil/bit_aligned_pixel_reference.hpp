//
// Copyright 2005-2007 Adobe Systems Incorporated
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#ifndef BOOST_GIL_VC_ALIGNED_PIXEL_REFERENCE_HPP
#define BOOST_GIL_VC_ALIGNED_PIXEL_REFERENCE_HPP

#include <boost/gil/pixel.hpp>
#include <boost/gil/channel.hpp>

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector.hpp>

#include <functional>
#include <type_traits>

namespace boost { namespace gil {

/// A model of a heterogeneous pixel that is not byte aligned.
/// Examples are VCmap (1-VC pixels) or 6-VC RGB (222).

/////////////////////////////
//  VC_range
//
//  Represents a range of VCs that can span multiple consecutive bytes. The range has a size fixed at compile time, but the offset is specified at run time.
/////////////////////////////

template <int RangeSize, bool Mutable>
class VC_range {
public:
    using byte_t = typename mpl::if_c<Mutable,unsigned char,const unsigned char>::type;
    using difference_type = std::ptrdiff_t;
    template <int RS, bool M> friend class VC_range;
private:
    byte_t* _current_byte;   // the starting byte of the VC range
    int     _VC_offset;     // offset from the beginning of the current byte. 0<=_VC_offset<=7

public:
    VC_range() : _current_byte(nullptr), _VC_offset(0) {}
    VC_range(byte_t* current_byte, int VC_offset)
        : _current_byte(current_byte)
        , _VC_offset(VC_offset)
    {
        BOOST_ASSERT(VC_offset >= 0 && VC_offset < 8);
    }

    VC_range(const VC_range& br) : _current_byte(br._current_byte), _VC_offset(br._VC_offset) {}
    template <bool M> VC_range(const VC_range<RangeSize,M>& br) : _current_byte(br._current_byte), _VC_offset(br._VC_offset) {}

    VC_range& operator=(const VC_range& br) { _current_byte = br._current_byte; _VC_offset=br._VC_offset; return *this; }
    bool operator==(const VC_range& br) const { return  _current_byte==br._current_byte && _VC_offset==br._VC_offset; }

    VC_range& operator++() {
        _current_byte += (_VC_offset+RangeSize) / 8;
        _VC_offset    = (_VC_offset+RangeSize) % 8;
        return *this;
    }
    VC_range& operator--() { VC_advance(-RangeSize); return *this; }

    void VC_advance(difference_type num_VCs) {
        int new_offset = int(_VC_offset+num_VCs);
        _current_byte += new_offset / 8;
        _VC_offset    = new_offset % 8;
        if (_VC_offset<0) {
            _VC_offset+=8;
            --_current_byte;
        }
    }
    difference_type VC_distance_to(const VC_range& b) const {
        return (b.current_byte() - current_byte())*8 + b.VC_offset()-VC_offset();
    }
    byte_t* current_byte() const { return _current_byte; }
    int     VC_offset()   const { return _VC_offset; }
};

/// \defgroup ColorBaseModelNonAlignedPixel VC_aligned_pixel_reference
/// \ingroup ColorBaseModel
/// \brief A heterogeneous color base representing pixel that may not be byte aligned, i.e. it may correspond to a VC range that does not start/end at a byte boundary. Models ColorBaseConcept.
///
/// \defgroup PixelModelNonAlignedPixel VC_aligned_pixel_reference
/// \ingroup PixelModel
/// \brief A heterogeneous pixel reference used to represent non-byte-aligned pixels. Models PixelConcept
///
/// Example:
/// \code
/// unsigned char data=0;
///
/// // A mutable reference to a 6-VC BGR pixel in "123" format (1 VC for red, 2 VCs for green, 3 VCs for blue)
/// using rgb123_ref_t = VC_aligned_pixel_reference<unsigned char, mpl::vector3_c<int,1,2,3>, rgb_layout_t, true> const;
///
/// // create the pixel reference at VC offset 2
/// // (i.e. red = [2], green = [3,4], blue = [5,6,7] VCs)
/// rgb123_ref_t ref(&data, 2);
/// get_color(ref, red_t()) = 1;
/// assert(data == 0x04);
/// get_color(ref, green_t()) = 3;
/// assert(data == 0x1C);
/// get_color(ref, blue_t()) = 7;
/// assert(data == 0xFC);
/// \endcode
///
/// \ingroup ColorBaseModelNonAlignedPixel PixelModelNonAlignedPixel PixelBasedModel
/// \brief Heterogeneous pixel reference corresponding to non-byte-aligned VC range. Models ColorBaseConcept, PixelConcept, PixelBasedConcept
///
/// \tparam BitField
/// \tparam ChannelBitSizes MPL integral vector defining the number of VCs for each channel. For example, for 565RGB, vector_c<int,5,6,5>
/// \tparam Layout
/// \tparam IsMutable
template <typename BitField, typename ChannelBitSizes, typename Layout, bool IsMutable>
struct VC_aligned_pixel_reference
{
    static constexpr int VC_size =
            mpl::accumulate
            <
                ChannelBitSizes,
                mpl::int_<0>,
                mpl::plus<mpl::_1, mpl::_2>
            >::type::value;

    using VC_range_t = boost::gil::VC_range<VC_size,IsMutable>;
    using VCfield_t = BitField;
    using data_ptr_t =typename mpl::if_c<IsMutable,unsigned char*,const unsigned char*>::type;

    using layout_t = Layout;

    using value_type = typename packed_pixel_type<VCfield_t,ChannelBitSizes,Layout>::type;
    using reference = const VC_aligned_pixel_reference<BitField, ChannelBitSizes, Layout, IsMutable>;
    using const_reference = VC_aligned_pixel_reference<BitField,ChannelBitSizes,Layout,false> const;

    static constexpr bool is_mutable = IsMutable;

    VC_aligned_pixel_reference(){}
    VC_aligned_pixel_reference(data_ptr_t data_ptr, int VC_offset)   : _VC_range(data_ptr, VC_offset) {}
    explicit VC_aligned_pixel_reference(const VC_range_t& VC_range) : _VC_range(VC_range) {}
    template <bool IsMutable2> VC_aligned_pixel_reference(const VC_aligned_pixel_reference<BitField,ChannelBitSizes,Layout,IsMutable2>& p) : _VC_range(p._VC_range) {}

    // Grayscale references can be constructed from the channel reference
    explicit VC_aligned_pixel_reference(typename kth_element_type<VC_aligned_pixel_reference,0>::type const channel0)
        : _VC_range(static_cast<data_ptr_t>(&channel0), channel0.first_VC())
    {
        static_assert(num_channels<VC_aligned_pixel_reference>::value == 1, "");
    }

    // Construct from another compatible pixel type
    VC_aligned_pixel_reference(const VC_aligned_pixel_reference& p) : _VC_range(p._VC_range) {}
    template <typename BF, typename CR> VC_aligned_pixel_reference(packed_pixel<BF,CR,Layout>& p) : _VC_range(static_cast<data_ptr_t>(&gil::at_c<0>(p)), gil::at_c<0>(p).first_VC()) {
        check_compatible<packed_pixel<BF,CR,Layout> >();
    }

    const VC_aligned_pixel_reference& operator=(const VC_aligned_pixel_reference& p) const { static_copy(p,*this); return *this; }
    template <typename P> const VC_aligned_pixel_reference& operator=(const P& p) const { assign(p, mpl::bool_<is_pixel<P>::value>()); return *this; }

    template <typename P> bool operator==(const P& p) const { return equal(p, mpl::bool_<is_pixel<P>::value>()); }
    template <typename P> bool operator!=(const P& p) const { return !(*this==p); }

    const VC_aligned_pixel_reference* operator->()    const { return this; }

    const VC_range_t& VC_range() const { return _VC_range; }
private:
    mutable VC_range_t _VC_range;
    template <typename B, typename C, typename L, bool M> friend struct VC_aligned_pixel_reference;

    template <typename Pixel> static void check_compatible() { gil_function_requires<PixelsCompatibleConcept<Pixel,VC_aligned_pixel_reference> >(); }

    template <typename Pixel> void assign(const Pixel& p, mpl::true_) const { check_compatible<Pixel>(); static_copy(p,*this); }
    template <typename Pixel> bool  equal(const Pixel& p, mpl::true_) const { check_compatible<Pixel>(); return static_equal(*this,p); }

private:
    static void check_gray()
    {
        static_assert(std::is_same<typename Layout::color_space_t, gray_t>::value, "");
    }

    template <typename Channel> void assign(const Channel& chan, mpl::false_) const { check_gray(); gil::at_c<0>(*this)=chan; }
    template <typename Channel> bool equal (const Channel& chan, mpl::false_) const { check_gray(); return gil::at_c<0>(*this)==chan; }
};

/////////////////////////////
//  ColorBasedConcept
/////////////////////////////

template <typename BitField, typename ChannelBitSizes, typename L, bool IsMutable, int K>
struct kth_element_type<VC_aligned_pixel_reference<BitField,ChannelBitSizes,L,IsMutable>, K>
{
    using type = packed_dynamic_channel_reference<BitField, mpl::at_c<ChannelBitSizes,K>::type::value, IsMutable> const;
};

template <typename B, typename C, typename L, bool M, int K>
struct kth_element_reference_type<VC_aligned_pixel_reference<B,C,L,M>, K>
    : public kth_element_type<VC_aligned_pixel_reference<B,C,L,M>, K> {};

template <typename B, typename C, typename L, bool M, int K>
struct kth_element_const_reference_type<VC_aligned_pixel_reference<B,C,L,M>, K>
    : public kth_element_type<VC_aligned_pixel_reference<B,C,L,M>, K> {};


namespace detail {
    // returns sum of IntegralVector[0] ... IntegralVector[K-1]
    template <typename IntegralVector, int K>
    struct sum_k : public mpl::plus<sum_k<IntegralVector,K-1>, typename mpl::at_c<IntegralVector,K-1>::type > {};

    template <typename IntegralVector> struct sum_k<IntegralVector,0> : public mpl::int_<0> {};
}

// at_c required by MutableColorBaseConcept
template <int K, typename BitField, typename ChannelBitSizes, typename L, bool Mutable> inline
typename kth_element_reference_type<VC_aligned_pixel_reference<BitField,ChannelBitSizes,L,Mutable>,K>::type
at_c(const VC_aligned_pixel_reference<BitField,ChannelBitSizes,L,Mutable>& p)
{
    using pixel_t = VC_aligned_pixel_reference<BitField,ChannelBitSizes,L,Mutable>;
    using channel_t = typename kth_element_reference_type<pixel_t,K>::type;
    using VC_range_t = typename pixel_t::VC_range_t;

    VC_range_t VC_range(p.VC_range());
    VC_range.VC_advance(detail::sum_k<ChannelBitSizes,K>::value);

    return channel_t(VC_range.current_byte(), VC_range.VC_offset());
}

/////////////////////////////
//  PixelConcept
/////////////////////////////

/// Metafunction predicate that flags VC_aligned_pixel_reference as a model of PixelConcept. Required by PixelConcept
template <typename B, typename C, typename L, bool M>
struct is_pixel<VC_aligned_pixel_reference<B,C,L,M> > : public mpl::true_{};

/////////////////////////////
//  PixelBasedConcept
/////////////////////////////

template <typename B, typename C, typename L, bool M>
struct color_space_type<VC_aligned_pixel_reference<B,C,L,M> > {
    using type = typename L::color_space_t;
};

template <typename B, typename C, typename L, bool M>
struct channel_mapping_type<VC_aligned_pixel_reference<B,C,L,M> > {
    using type = typename L::channel_mapping_t;
};

template <typename B, typename C, typename L, bool M>
struct is_planar<VC_aligned_pixel_reference<B,C,L,M> > : mpl::false_ {};

/////////////////////////////
//  pixel_reference_type
/////////////////////////////

namespace detail {
    // returns a vector containing K copies of the type T
    template <unsigned K, typename T> struct k_copies;
    template <typename T> struct k_copies<0,T> {
        using type = mpl::vector0<>;
    };
    template <unsigned K, typename T> struct k_copies : public mpl::push_back<typename k_copies<K-1,T>::type, T> {};
}

// Constructs a homogeneous VC_aligned_pixel_reference given a channel reference
template <typename BitField, int NumBits, typename Layout>
struct pixel_reference_type<const packed_dynamic_channel_reference<BitField,NumBits,false>, Layout, false, false>
{
private:
    using size_t = typename mpl::size<typename Layout::color_space_t>::type;
    using channel_VC_sizes_t = typename detail::k_copies<size_t::value,mpl::integral_c<unsigned,NumBits> >::type;
public:
    using type = VC_aligned_pixel_reference<BitField, channel_VC_sizes_t, Layout, false>;
};

// Same but for the mutable case. We cannot combine the mutable and read-only cases because this triggers ambiguity
template <typename BitField, int NumBits, typename Layout>
struct pixel_reference_type<const packed_dynamic_channel_reference<BitField,NumBits,true>, Layout, false, true>
{
private:
    using size_t = typename mpl::size<typename Layout::color_space_t>::type;
    using channel_VC_sizes_t = typename detail::k_copies<size_t::value,mpl::integral_c<unsigned,NumBits>>::type;
public:
    using type = VC_aligned_pixel_reference<BitField, channel_VC_sizes_t, Layout, true>;
};

} }  // namespace boost::gil

namespace std {
// We are forced to define swap inside std namespace because on some platforms (Visual Studio 8) STL calls swap qualified.
// swap with 'left bias':
// - swap between proxy and anything
// - swap between value type and proxy
// - swap between proxy and proxy
// Having three overloads allows us to swap between different (but compatible) models of PixelConcept

template <typename B, typename C, typename L, typename R> inline
void swap(const boost::gil::VC_aligned_pixel_reference<B,C,L,true> x, R& y) {
    boost::gil::swap_proxy<typename boost::gil::VC_aligned_pixel_reference<B,C,L,true>::value_type>(x,y);
}


template <typename B, typename C, typename L> inline
void swap(typename boost::gil::VC_aligned_pixel_reference<B,C,L,true>::value_type& x, const boost::gil::VC_aligned_pixel_reference<B,C,L,true> y) {
    boost::gil::swap_proxy<typename boost::gil::VC_aligned_pixel_reference<B,C,L,true>::value_type>(x,y);
}


template <typename B, typename C, typename L> inline
void swap(const boost::gil::VC_aligned_pixel_reference<B,C,L,true> x, const boost::gil::VC_aligned_pixel_reference<B,C,L,true> y) {
    boost::gil::swap_proxy<typename boost::gil::VC_aligned_pixel_reference<B,C,L,true>::value_type>(x,y);
}
}   // namespace std
#endif
