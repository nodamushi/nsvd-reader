/*!
  @brief &lt;interrupt&gt; element
  @file nodamushi/svd/Interrupt.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_INTERRUPT_HPP
#define NODAMUSHI_SVD_INTERRUPT_HPP

# include "nodamushi/svd/elements.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief &lt;interrupt&gt; element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_peripherals.html#elem_interrupt
 */
template<typename STR>struct Interrupt:public svd_node
{
  //! @brief &lt;name&gt; name of interrupt
  SVD_VALUE(STR,name,true);
  //! @brief &lt;description&gt;
  SVD_VALUE(STR,description);
  //! @brief &lt;value&gt;interrupt index
  SVD_VALUE(uint64_t,value,true);
};
//----------------------------------------------

template<typename SVD,typename STR>
bool create(SVD& svd,Interrupt<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "name")t = (d.name = c);
    else if(n == "description")t = (d.description = c);
    else if(n == "value")t = (d.value =c);
    else c.unknown_element(svd_element::Interrupt);
    if(!t){
      c.illegal_value(svd_element::Interrupt);
    }
  }
  return true;
}

template<typename STR>
struct svd_printer<Interrupt<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Interrupt<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name<<">"<<std::endl;
    int i= indent + 1;

    print_svd_value(o,v.name,i,true);
    print_svd_value(o,v.description,i,true);
    print_svd_value(o,v.value,i,true);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};



} // end namespace svd
}// end namespace nodamushi

#endif // NODAMUSHI_SVD_INTERRUPT_HPP
