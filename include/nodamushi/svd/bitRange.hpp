/*!
 * @brief &lt;field&gt;.&lt;bitRange&gt; element
 * @file nodamushi/svd/bitRange.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_BITRANGE_HPP
#define NODAMUSHI_SVD_BITRANGE_HPP

# include <cstdint>
# include <ostream>
# include "nodamushi/svd/value_setter_helper.hpp"
namespace nodamushi{
namespace svd{
/**
 * @brief &lt;field&gt;.&lt;bitRange&gt; element
 * @par
 * format: [&lt;msb&gt;:&lt;lsb&gt;]
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_field
 */
struct bitRange
{
  //! lsb: the bit position of the least significant bit within the register.
  uint32_t lsb;
  //! msb: the bit position of the most significant bit within the register. 
  uint32_t msb;
  
  //-----------------------------------------------
  bitRange():lsb(0),msb(0){}
  bitRange(uint32_t l,uint32_t m):lsb(l<m?l:m),msb(l<m?m:l){}
  
  bool operator==(const bitRange& b)const noexcept{
    return lsb == b.lsb && msb == b.msb;
  }
  bool operator!=(const bitRange& b)const noexcept{
    return lsb != b.lsb || msb != b.msb;
  }

  /**
   * set lsb and msb.
   * @param src text. format:[&lt;msb&gt;:&lt;lsb&gt;]
   * @return success
   */
  template<typename STR> bool set(const STR& src)
  {
    // decode [<msb>:<lsb>]
    size_t l = src.length();
    if(l <5||src[0] !='[' ||src[l-1] !=']'){return false;}
    
    size_t msb_b=1,msb_e,colon,lsb_b,lsb_e;
    while(is_space(src[msb_b]))
      msb_b++;
    
    msb_e = msb_b;
    while(!is_space(src[msb_e]) && src[msb_e] != ':' )
      msb_e++;
    colon = msb_e;
    while(src[colon] != ':' )
      colon++;
    lsb_b = colon + 1;
    while(is_space(src[lsb_b]))
      lsb_b++;
    lsb_e = lsb_b;
    while(!is_space(src[lsb_e]) && src[lsb_e] != ']' )
      lsb_e++;
    uint32_t msb=0;
    uint32_t lsb=0;
    const char* ptr = src.data();
    if(!to_int(ptr + msb_b, ptr + msb_e,msb)){
      return false;
    }
    if(!to_int(ptr + lsb_b, ptr + lsb_e,lsb)){
      return false;
    }
    this->lsb = lsb;
    this->msb = msb;
    return true;
  }


 private:
  static constexpr bool is_space(char c)
  {
    return ' ' == c || '\t' == c || '\r' == c || '\n' == c;
  }
  static constexpr bool to_int(const char* begin,const char* const end,uint32_t& i)
  {
    while(begin!=end)
    {
      char c = *begin;
      ++begin;
      if('0' <= c && c <='9')
        i = i * 10 + (c -'0');
      else
        return false;
    }
    return true;
  }

};


template<>struct value_setter_helper<bitRange>
{  
  static constexpr bool value=true;
  
  static bool set(const std::string& src,bitRange& dst){ return dst.set(src); }
# if __cplusplus >= 201703
  static bool set(std::string_view src,bitRange& dst){return dst.set(src);}
# endif

};




}}// end namespace nodamushi::svd


namespace std{
inline ostream& operator<<(ostream& o,const nodamushi::svd::bitRange& r)
{
  o << '['<<r.msb <<':' <<r.lsb<<']';
  return o;
}
} // end namespace std

#endif // NODAMUSHI_SVD_BITRANGE_HPP
