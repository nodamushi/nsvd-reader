/*!
  @brief normalized AddressBlock
  @file nodamushi/svd/normalized/AddressBlock.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_ADDRESSBLOCK_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_ADDRESSBLOCK_HPP__

# include <cstdint>
# include "nodamushi/svd/AddressBlockUsage.hpp"
# include "nodamushi/svd/Protection.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{

/**
 * @brief normalized &lt;addressBlock&gt;
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_peripherals.html#elem_addressBlock
 * @see nodamushi::svd::AddressBlock
 */
struct AddressBlock:public svd_node
{
  //! @brief the start address of an address block relative to the peripheral baseAddress. 
  uint64_t          offset;
  //! @brief the number of addressUnitBits being covered by this address block.
  uint64_t          size;  
  //! @brief register/buffer/reserved
  AddressBlockUsage usage; 
  //! @brief the protection level
  Protection        protection;

  /**
   * @param T nodamushi::svd::AddressBlock
   */
  template<typename T>  AddressBlock(const T& src):
      offset(src.offset? *src.offset:0),
      size(src.size? *src.size:0),
      usage(src.usage? *src.usage: static_cast<AddressBlockUsage>(0)),
      protection(src.protection? *src.protection:static_cast<Protection>(0)){}
};


}}} // end namespace

#endif // __NODAMUSHI_SVD_NORMALIZED_ADDRESSBLOCK_HPP__
