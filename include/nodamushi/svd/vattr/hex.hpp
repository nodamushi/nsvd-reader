/*!
  @brief hex attribute
  @file nodamushi/svd/vattr/hex.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_VATTR_HEX_HPP__
#define __NODAMUSHI_SVD_VATTR_HEX_HPP__

# include <type_traits>
# include "nodamushi/svd/vattr.hpp"
namespace nodamushi{
namespace svd{

/**
 * print as hexa decimal number
 */
struct hex;

/**
 * print as hexa decimal number(uint64_t)
 */
using hex64 = vattr<uint64_t,hex>;
/**
 * print as hexa decimal number(uint32_t)
 */
using hex32 = vattr<uint32_t,hex>;
/**
 * print as hexa decimal number(uint16_t)
 */
using hex16 = vattr<uint16_t,hex>;
/**
 * print as hexa decimal number(uint8_t)
 */
using hex8  = vattr<uint8_t,hex>;


} // end namespace svd
}// end namespace nodamushi

#endif // __NODAMUSHI_SVD_HEX_NODAMUSHI_VATTR_SVD_HPP__
