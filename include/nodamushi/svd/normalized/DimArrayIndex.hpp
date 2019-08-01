/*!
  @brief normalized dimArrayIndex element
  @file nodamushi/svd/normalized/DimArrayIndex.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_DIMARRAYINDEX_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_DIMARRAYINDEX_HPP__


# include <vector>
# include "nodamushi/svd/node.hpp"

# include "nodamushi/svd/normalized/EnumeratedValue.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{


/**
 * @brief &lt;dimArrayIndex&gt; element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_special.html#elem_dimArrayIndex
 * @see nodamushi::svd::DimArrayIndex
 */
template<typename STRREF>
struct DimArrayIndex:public svd_node
{
  //! @brief &lt;headerEnumName&gt;
  STRREF headerEnumName;
  //! @brief &lt;enumeratedValue&gt;
  std::vector<EnumeratedValue<STRREF>> enumeratedValue;
  
  /**
   * @param T nodamushi::svd::DimArrayIndex
   */
  template<typename T>
  DimArrayIndex(const T& src):
      headerEnumName(src.headerEnumName.get("")),
      enumeratedValue(src.enumeratedValue.get("")),
      value(src.value? *src.value,0),
      isDefault(src.isDefault? *src.isDefault:false){}
};

}}} // end namespace svd

#endif // __NODAMUSHI_SVD_NORMALIZED_DIMARRAYINDEX_HPP__
