/*!
  @brief normalized bit_range
  @file nodamushi/svd/normalized/bit_range.hpp
*/
#ifndef NODAMUSHI_SVD_NORMALIZED_BIT_RANGE_HPP
#define NODAMUSHI_SVD_NORMALIZED_BIT_RANGE_HPP

# include <cstdint>
# include <ostream>

namespace nodamushi{
namespace svd{
namespace normalized{

/**
 * @brief normalized bit range
 */
struct bit_range
{
  constexpr bool operator ==(const bit_range& r)const noexcept{return _lsb == r._lsb && _msb == r._msb;}
  constexpr bool operator !=(const bit_range& r)const noexcept{return _lsb != r._lsb || _msb != r._msb;}
  constexpr bool operator <(const bit_range& r)const noexcept{return _lsb < r._lsb;}
  constexpr bool operator <=(const bit_range& r)const noexcept{return _lsb <= r._lsb;}
  constexpr bool operator >(const bit_range& r)const noexcept{return _lsb > r._lsb;}
  constexpr bool operator >=(const bit_range& r)const noexcept{return _lsb >= r._lsb;}
  //! @brief bit size
  constexpr unsigned int width()const noexcept{return _msb - _lsb + 1;}
  //! @brief bit size
  constexpr unsigned int size()const noexcept{return _msb - _lsb + 1;}
  //! @brief msb
  constexpr unsigned int left()const noexcept{return _msb;}
  //! @brief lsb
  constexpr unsigned int right()const noexcept{return _lsb;}
  constexpr unsigned int msb()const noexcept{return _msb;}
  constexpr unsigned int lsb()const noexcept{return _lsb;}
  //! @brief set msb
  void msb(unsigned int x)noexcept{if(_lsb <= x)_msb = x;else _lsb = x;}
  //! @brief set lsb
  void lsb(unsigned int x)noexcept{if(x <= _msb)_lsb = x;else _msb = x;}
  /*!
   * @brief get the bit number of bit-range.
   *  - [3:3] : get(8) = 1
   *  - [3:1] : get(8) = 4
   */
  uint64_t get(uint64_t v){
    auto w = size();
    if(w == 64) return v;
    v >>=_lsb;
    const uint64_t mask = (((uint64_t)1) << w) -1;
    return v & mask;
  }
  /*!
   * @brief get the bit of bit-range.
   *  - [3:3] : bit(8) = 8
   *  - [3:1] : bit(8) = 8
   *  - [3:3] : bit(0xFF) = 8
   *  - [3:2] : bit(0xFF) = 0xC
   */
  uint64_t bit(uint64_t v){
    auto w = size();
    if(w == 64) return v;
    const uint64_t mask = ((((uint64_t)1) << w) -1) << _lsb;
    return v & mask;
  }

  //constructors
  bit_range(unsigned int x,unsigned int y)
      :_lsb(x<y? x:y),_msb(x <y? y:x){}

  bit_range(const bit_range&)=default;
  bit_range(bit_range&&)=default;

  template<typename T>
  bit_range(const T& n)://don't change name
      _lsb(0),_msb(0)
  {
    unsigned int l=0,w=1;
    __NORMALIZED_DERIVED_FROM_HELPER(lsb);
    __NORMALIZED_DERIVED_FROM_HELPER(msb);
    if(lsb && msb){
      l = *lsb;
      w = *msb +1 -l;
    }else{
      __NORMALIZED_DERIVED_FROM_HELPER(bitRange);
      if(bitRange){
        const auto&  r = *bitRange;
        l = r.lsb;
        w = r.msb +1 -r.lsb;
      }else{
        __NORMALIZED_DERIVED_FROM_HELPER(bitOffset);
        __NORMALIZED_DERIVED_FROM_HELPER(bitWidth);
        if(bitOffset &&bitWidth){
          l = *bitOffset;
          w = *bitWidth;
        }else{
          //TODO ? error
        }
      }
    }
    if(n.dim && n.dim.index!=0){
      __NORMALIZED_DERIVED_FROM_HELPER(dimIncrement);
      size_t inc = 1;
      if(dimIncrement)
        inc = *dimIncrement;
      l += inc * n.dim.index;
    }
    unsigned int m = l + w -1;
    _lsb = l;
    _msb = m;
  }
 private:
  using uint = uint16_t;
  uint _lsb;
  uint _msb;

};

}}}// end namesapce

namespace std{
inline ostream& operator <<(ostream& o ,const ::nodamushi::svd::normalized::bit_range& r)
{
  return o << '[' << r.left() << ':' << r.right() << ']';
}
}

#endif // NODAMUSHI_SVD_NORMALIZED_BITRANGE_HPP

