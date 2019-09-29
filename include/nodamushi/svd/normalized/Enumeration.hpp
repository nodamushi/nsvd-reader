/*!
  @brief Normalized enumerationValues element
  @file nodamushi/svd/normalized/Enumeration.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_ENUMERATION_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_ENUMERATION_HPP__

# include <string>

# include "nodamushi/svd/EnumUsage.hpp"
# include "nodamushi/svd/normalized/EnumeratedValue.hpp"
# include "nodamushi/svd/normalized/derived_from_helper.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{

/**
 * @brief normalized &lt;enumeratedValues&gt; element. iterable
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_enumeratedValues
 * @see nodamushi::svd::Enumeration
 */
template<typename STRREF>struct Enumeration
{
  using Field = ::nodamushi::svd::normalized::Field<STRREF>;
  using EnumeratedValue = ::nodamushi::svd::normalized::EnumeratedValue<STRREF>;

  //! @brief &lt;name&gt;
  std::string name;
  //! @brief &lt;headerEnumName&gt;
  STRREF headerEnumName;
  //! @brief &lt;usage&gt;
  EnumUsage usage;
  //! @brief &lt;enumeratedValue&gt; list
  std::vector<EnumeratedValue> enumeratedValue;

  Enumeration()=default;
  Enumeration(Enumeration&&)=default;
  Enumeration(const Enumeration&)=default;

  template<typename T>
  Enumeration(const T& n): // don't change name
      name(n.name),
      __NORMALIZED_DERIVED_FROM(headerEnumName),
      __NORMALIZED_DERIVED_FROM(usage),
      enumeratedValue()
  {
    __NORMALIZED_DERIVED_FROM_HELPER(enumeratedValue);
    if(enumeratedValue){
      const auto& v = *enumeratedValue;
      auto& d=this->enumeratedValue;
      d.reserve(v.size());
      for(const auto& c:v)
        d.emplace_back(c);
      sort();
    }
  }

  //--------------------------------------------------------------
  /**
   * @brief sort enumeratedValue by enumratedValue.value.
   * @note enumratedValue whose isDefault member is true will be last
   */
  void sort()
  {
    std::sort(enumeratedValue.begin(),enumeratedValue.end());
  }

  /**
   * @brief &lt;enumeratedValue&gt; is empty
   * @return &lt;enumeratedValue&gt; is empty
   */
  bool empty()const noexcept{return enumeratedValue.empty();}
  /**
   * @brief count of &lt;enumeratedValue&gt;
   * @return count of &lt;enumeratedValue&gt;
   */
  size_t size()const noexcept{return enumeratedValue.size();}
  /**
   * @brief count of &lt;enumeratedValue&gt;
   * @return count of &lt;enumeratedValue&gt;
   */
  size_t length()const noexcept{return enumeratedValue.size();}
  using iterator = typename std::vector<EnumeratedValue>::iterator;
  using const_iterator = typename std::vector<EnumeratedValue>::const_iterator;
  //! @brief enumeratedValue iterator.
  iterator begin() noexcept{return enumeratedValue.begin();}
  //! @brief enumeratedValue iterator.
  iterator end() noexcept{return enumeratedValue.end();}
  //! @brief enumeratedValue iterator.
  const_iterator begin()const noexcept{return enumeratedValue.begin();}
  //! @brief enumeratedValue iterator.
  const_iterator end()const noexcept{return enumeratedValue.end();}

};
//---------- Visitor --------------------
__NX_NORM_HANDLE_VISIT(Enumeration)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  r ret;
  ret =  CONTROLLER::apply(t.enumeratedValue,v);
  if(ret == r::BREAK)return ret;
  return r::CONTINUE;
}};
//--------------------------------------------



}}} // end namespace svd

#endif // __NODAMUSHI_SVD_NORMALIZED_ENUMERATION_HPP__
