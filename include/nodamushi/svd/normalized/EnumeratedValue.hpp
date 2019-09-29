/*!
  @brief normalized &lt;enumeratedValue&gt; element
  @file nodamushi/svd/normalized/EnumeratedValue.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_ENUMERATEDVALUE_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_ENUMERATEDVALUE_HPP__

# include <cstdint>
# include "nodamushi/svd/EnumeratedNumber.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{

/**
 * @brief Normalized enumeratedValue
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_enumeratedValue
 * @see nodamushi::svd::EnumeratedValue
 */
template<typename STRREF>struct EnumeratedValue
{
  //! @brief &lt;name&gt;
  STRREF           name;
  //! @brief &lt;description&gt;
  STRREF           description;
  //! @brief &lt;value&gt;
  EnumeratedNumber value;
  //! @brief &lt;isDefault&gt;
  bool             isDefault;


  //! @brief return !isDefault
  operator bool(){return !isDefault;}
  bool is_default()const noexcept{return isDefault;}
  //! @brief value.get_value()
  uint64_t get_value()const noexcept{return value.get_value();}

  EnumeratedValue()=default;
  EnumeratedValue(EnumeratedValue&&)=default;
  EnumeratedValue(const EnumeratedValue&)=default;
  EnumeratedValue& operator=(const EnumeratedValue&)=default;
  EnumeratedValue& operator=(EnumeratedValue&&)=default;

  /**
   * @param T nodamushi::svd::EnumeratedValue
   */
  template<typename T>
  EnumeratedValue(const T& src):
      name(src.name.get("")),
      description(src.description.get("")),
      value(src.value? *src.value:EnumeratedNumber{}),
      isDefault(src.isDefault? *src.isDefault:false){}

  /**
   * @brief compare default and value.
   * @param v compare target
   * @return
   * - minus : less than v
   * - 0     : same
   * - plus  : greater than v
   */
  int compare(const EnumeratedValue& v)const
  {
    if(v.isDefault)return isDefault?0:-1;
    if(isDefault)return 1;
    return value.compare(value);
  }

  bool operator<(const EnumeratedValue& e)const{return  compare(e)<0;}
  bool operator<=(const EnumeratedValue& e)const{return compare(e)<=0;}
  bool operator>(const EnumeratedValue& e)const{return  compare(e)>0;}
  bool operator>=(const EnumeratedValue& e)const{return compare(e)>=0;}

};

}}} // end namespace

#endif // __NODAMUSHI_SVD_NORMALIZED_ENUMERATEDVALUE_HPP__
