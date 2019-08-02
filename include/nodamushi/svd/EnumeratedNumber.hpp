/*!
  @brief EnumeratedNumber
  @file nodamushi/svd/EnumeratedNumber.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef __NODAMUSHI_SVD_ENUMERATEDNUMBER_HPP__
#define __NODAMUSHI_SVD_ENUMERATEDNUMBER_HPP__

# include <ostream>
# include <algorithm>
# include "nodamushi/to_int.hpp"
# include "nodamushi/svd/value_setter_helper.hpp"
namespace nodamushi{
namespace svd{

/*!
 * @brief Decode and represent enemeratedValue.value
 *
 * @par
 * format:
 *
 * - [0-9]+           :decimal
 * - 0x[0-9A-Fa-f]+   :hexadecimal
 * - 0b[01x]+         :binary
 * - #[01x]+          :binary
 * @par
 *   binary format is able to contains ``don't care'' bit information.
 *
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_enumeratedValue
 */
struct EnumeratedNumber
{
 private:
  //! radix 10 : dec,16:hex, 2:bin,-2: #(bin)
  int base;
  uint64_t value;
  uint64_t dont_care;

 public:
  /*!
   * @brief get value
   * @return value
   */
  operator uint64_t()const{return value;}
  /*!
   * @brief get don't care
   * @return don't care bit information.
   * @par
   *   get_dont_care() &(1 << y) != 0  : bit y is don't care
   */
  uint64_t get_dont_care()const{return dont_care;}
  /*!
   * @brief get value
   * @return value
   */
  uint64_t get_value()const{return value;}
  /*!
   * @brief value & don't care
   * @return value & don't care
   */
  uint64_t get_masked_value()const{return value&~dont_care;}

  /*!
   * @brief radix. 10 / 16 / 2
   * @return radix. 10 / 16 / 2
   */
  int get_base()const{return base==-2?2:base;}
  /*!
   * @brief compare with value and don't care.
   *
   * #1001 == #xxxx
   */
  bool operator==(const EnumeratedNumber& e)const
  {
    return ((value^e.value) & ~(dont_care  | e.dont_care)) == 0;
  }
  /*!
   * @brief compare with value and don't care
   */
  bool operator==(uint64_t e)const
  {
    return ((value^e) & ~dont_care) == 0;
  }
  /*!
   * @brief compare with value & don't care
   */
  bool operator!=(const EnumeratedNumber& e)const
  {
    return !operator==(e);
  }
  /*!
   * @brief compare with value & don't care
   */
  bool operator!=(uint64_t e)const
  {
    return !operator==(e);
  }
  /**
   * @brief compare without don't care bits
   * @param e compare target
   * @return
   * - minus : less than e
   * - 0     : same
   * - plus  : greater than e
   */
  int compare(const EnumeratedNumber& e)const
  {
    auto m = ~(dont_care  | e.dont_care);
    return (value&m) - (e.value & m);
  }
  /**
   * @brief compare without don't care bits
   * @param e compare target
   * @return
   * - minus : less than e
   * - 0     : same
   * - plus  : greater than e
   */
  int compare(uint64_t e)const
  {
    return get_masked_value() - (e & dont_care);
  }
  bool operator<(const EnumeratedNumber& e)const{return  compare(e)<0;}
  bool operator<=(const EnumeratedNumber& e)const{return compare(e)<=0;}
  bool operator>(const EnumeratedNumber& e)const{return  compare(e)>0;}
  bool operator>=(const EnumeratedNumber& e)const{return compare(e)>=0;}
  bool operator<(uint64_t e)const{return  compare(e)<0;}
  bool operator<=(uint64_t e)const{return compare(e)<=0;}
  bool operator>(uint64_t e)const{return  compare(e)>0;}
  bool operator>=(uint64_t e)const{return compare(e)>=0;}




  EnumeratedNumber():base(10),value(0),dont_care(0){}
  EnumeratedNumber(const EnumeratedNumber&)=default;
  EnumeratedNumber(EnumeratedNumber&&)=default;
  EnumeratedNumber& operator=(const EnumeratedNumber&)=default;
  EnumeratedNumber& operator=(EnumeratedNumber&&)=default;

  EnumeratedNumber(uint64_t v):
      base(10),
      value(v),
      dont_care(0){}
  EnumeratedNumber(
# if __cplusplus >= 201703
      std::string_view str
# else
      const std::string& str
# endif
                   ):
      base(10),
      value(0),
      dont_care(0)
  {
    operator=(str);
  }

  void operator=(
# if __cplusplus >= 201703
      std::string_view str
# else
      const std::string& str
# endif
                 )
  {
    // 0xHEX
    // Decimal
    // 0bBIN  (* x = don't care)
    // #BIN   (* x = don't care)
    auto l = str.length();
    int rad = 10;
    size_t pos = 0;
    if(str[0] == '#'){
      rad = -2;
      pos = 1;
    }else if(l > 2 && str[0] == '0'){
      if(str[1] == 'x' || str[1] == 'X'){
        rad = 16;
        pos = 2;
      }else if(str[1] == 'b' || str[1] == 'B'){
        rad = 2;
        pos = 2;
      }
    }
    if(rad >= 10){
      value = ::nodamushi::to_int<uint64_t>(str,pos,l,rad);
      dont_care = 0;
    }else{
      uint64_t val = 0;
      uint64_t don = 0;
      uint64_t add = 1;
      for(size_t p = l;p!=pos;p--,add <<= 1){
        char c = str[p-1];
        switch(c)
        {
          case '0':break;
          case '1':val |= add;break;
          case 'x':
          case 'X':
            don |= add;break;
        }
      }
      value=val;
      dont_care = don;
    }
    base = rad;
  }
  /**
   * @brief update value only
   */
  EnumeratedNumber& operator=(uint64_t v)noexcept{
    value = v;
    return *this;
  }
  /*!
   * @brief update contents.
   * @param v value
   * @param dont_care don't care bit information.(1: don't core, 0: care)
   * @param base radix
   */
  void set(uint64_t v,uint64_t dont_care,int base=10)noexcept{
    value = v;
    this->dont_care = dont_care;
    if(base == 16 || base == 2 || base == -2){
      this->base = base;
    }else{
      this->base = 10;
    }
  }

  void print(std::ostream& o)const noexcept
  {
    if(base == 2 || base == -2){
      uint64_t v = value;
      uint64_t m = dont_care;
      uint64_t h = std::max(highest1bit(v),highest1bit(m));
      if(h == 0){
        o << (base == 2? "0b0":"#0");
        return;
      }
      if(base == 2) o << "0b";
      else          o << '#';
      while(h!=0){
        if((m & h)!=0)
          o << 'x';
        else if((v&h)!=0)
          o << '1';
        else
          o << '0';
        h >>= 1;
      }
    }else{
      auto f = o.flags();
      if(base == 16)
        o <<"0x"<< std::hex << std::uppercase << value;
      else
        o << std::dec << value;
      o.flags(f);
    }
  }
 private:
  static uint64_t highest1bit(uint64_t x)
  {
    x |= x>>1;
    x |= x>>2;
    x |= x>>4;
    x |= x>>8;
    x |= x>>16;
    x |= x>>32;
    return x - (x >> 1);
  }
};



template<>struct value_setter_helper<EnumeratedNumber>
{
  static constexpr bool value=true;
  static bool set(const std::string& src,EnumeratedNumber& dst){dst = src;return true;}
# if __cplusplus >= 201703
  static bool set(const std::string_view& src,EnumeratedNumber& dst){dst = src;return true;}
# endif
};


}}// end namespace nodamushi




namespace std{
inline ostream& operator<<(ostream& o,const ::nodamushi::svd::EnumeratedNumber& e)
{
  e.print(o);
  return o;
}
}


#endif // __NODAMUSHI_SVD_ENUMERATEDNUMBER_HPP__
