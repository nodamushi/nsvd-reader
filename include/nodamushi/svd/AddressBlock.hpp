/*!
  @brief &lt;addressBlock&gt; elment
  @file nodamushi/svd/AddressBlock.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_ADDRESSBLOCK_HPP
#define NODAMUSHI_SVD_ADDRESSBLOCK_HPP

# include "nodamushi/svd/elements.hpp"

namespace nodamushi{
namespace svd{


/**
 * @brief &lt;addressBlock&gt; element.
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_peripherals.html#elem_addressBlock
 * @see nodamushi::svd::Peripheral
 */
struct AddressBlock:public svd_node
{
  //! @brief &lt;offset&gt; the start address of an address block relative to the peripheral baseAddress. 
  SVD_VALUE(hex64,offset,true);
  //! @brief &lt;size&gt; the number of addressUnitBits being covered by this address block.
  SVD_VALUE(hex64,size,true);
  //! @brief &lt;usage&gt; registers/buffer/reserved
  SVD_VALUE(AddressBlockUsage,usage,true);
  //! @brief &lt;protection&gt;
  SVD_VALUE(Protection,protection);
};

//----------------------------------------------

//! build AddressBlock
template<typename SVD>bool create(SVD& svd,AddressBlock& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "offset")t = (d.offset = c);
    else if(n == "size")t = (d.size = c);
    else if(n == "usage")t = (d.usage = c);
    else if(n == "protection")t = (d.protection = c);
    else c.unknown_element(svd_element::AddressBlock);
    if(!t){
      c.illegal_value(svd_element::AddressBlock);
    }
  }
  return true;
}
//! AddressBlock printer
template<>struct svd_printer<AddressBlock>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const AddressBlock& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name<<">"<<std::endl;
    int i= indent + 1;

    print_svd_value(o,v.offset,i,true);
    print_svd_value(o,v.size,i,true);
    print_svd_value(o,v.usage,i,true);
    print_svd_value(o,v.protection,i,true);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};



} // end namespace svd
}// end namespace nodamushi

#endif // NODAMUSHI_SVD_ADDRESSBLOCK_HPP
