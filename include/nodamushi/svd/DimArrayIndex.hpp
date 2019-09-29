/*!
  @brief dimArrayIndex element
  @file nodamushi/svd/DimArrayIndex.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_DIMARRAYINDEX_HPP
#define NODAMUSHI_SVD_DIMARRAYINDEX_HPP

# include "nodamushi/svd/elements.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief &lt;dimArrayIndex&gt; element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_special.html#elem_dimArrayIndex
 */
template<typename STR>
struct DimArrayIndex:public svd_node
{
  //! @brief &lt;headerEnumName&gt;
  SVD_VALUE(STR,headerEnumName);
  //! @brief &lt;enumeratedValue&gt;
  SVD_VALUE(std::vector<EnumeratedValue<STR>>,enumeratedValue);
};
//----------------------------------------------

template<typename SVD,typename STR>
bool create(SVD& svd,DimArrayIndex<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "headerEnumName")t = ( d.headerEnumName = c);
    else if(n == "enumeratedValue"){
      auto& v =d.enumeratedValue.get_for_init(); 
      auto i = v.size();
      v.emplace_back();
      EnumeratedValue<STR>& ev = v[i];
      create(c,ev);
    }
    else c.unknown_element(svd_element::DimArrayIndex);
    if(!t){
      c.illegal_value(svd_element::DimArrayIndex);
    }
  }
  return true;
}

template<typename STR>
struct svd_printer<DimArrayIndex<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const DimArrayIndex<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name<<">"<<std::endl;
    int i= indent + 1;

    print_svd_value(o,v.headerEnumName,i,true);
    print_svd_value(o,v.enumeratedValue,i,true);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};







} // end namespace svd
}// end namespace nodamushi

#endif // NODAMUSHI_SVD_DIMARRAYINDEX_HPP
