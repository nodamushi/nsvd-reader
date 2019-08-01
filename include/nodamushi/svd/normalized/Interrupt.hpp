/*!
  @brief normalized interrupt element
  @file nodamushi/svd/normalized/Interrupt.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_INTERRUPT_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_INTERRUPT_HPP__

# include <cstdint>

namespace nodamushi{
namespace svd{
namespace normalized{

/**
 * @brief normalized interrupt element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_peripherals.html#elem_interrupt
 * @see nodamushi::svd::Interrupt
 */
template<typename STRREF>struct Interrupt
{
  //! @brief &lt;name&gt; name of interrupt
  STRREF   name;
  //! @brief &lt;description&gt;
  STRREF   description;
  //! @brief &lt;value&gt;interrupt index
  uint64_t value;
  
  //! @param T nodamushi::svd::Interrupt
  template<typename T>
  Interrupt(const T& src):
      name(src.name.get("")),
      description(src.description.get("")),
      value(src.value.get(0)){}
};

}}} // end namespace svd

#endif // __NODAMUSHI_SVD_NORMALIZED_INTERRUPT_HPP__
