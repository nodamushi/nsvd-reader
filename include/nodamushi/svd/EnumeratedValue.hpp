/*!
  @brief &lt;enumeratedValue&gt; element
  @file nodamushi/svd/EnumeratedValue.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_ENUMERATEDVALUE_HPP__
#define __NODAMUSHI_SVD_ENUMERATEDVALUE_HPP__
# include "nodamushi/svd/node.hpp"

# include "nodamushi/svd/create.hpp"
# include "nodamushi/svd/svd_printer.hpp"
# include "nodamushi/svd/value.hpp"
# include "nodamushi/svd/EnumeratedNumber.hpp"
namespace nodamushi{
namespace svd{


/**
 * @brief enemeratedValue element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_enumeratedValue
 */
template<typename STR=std::string>
struct EnumeratedValue:public svd_node
{
  //! @brief &lt;name&gt;
  SVD_VALUE(STR,name);
  //! @brief &lt;description&gt;
  SVD_VALUE(STR,description);
  //! @brief &lt;value&gt;
  SVD_VALUE(EnumeratedNumber,value);
  /**
   * @brief &lt;isDefault&gt;
   * @see is_default()
   */
  SVD_VALUE(bool,isDefault);
  /**
   * @brief get &lt;isDefault&gt; value
   * @return get isDefault value. if isDefault is empty,return false.
   */
  bool is_default()const noexcept{
    if(isDefault)return *isDefault;
    return false;
  }
  /**
   * @brief short cut method.
   * value->get_value()
   * @throw empty_value_access_error
   */
  uint64_t get_value()const{
    return value->get_value();
  }
};
//----------------------------------------------

template<typename SVD,typename STR>
bool create(SVD& svd,EnumeratedValue<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t =true;
    if(n == "name") t = (d.name = c);
    else if(n == "description") t = (d.description = c);
    else if(n == "value") t = (d.value = c);
    else if(n == "isDefault") t = (d.isDefault = c);
    if(!t){
      //TODO error
    }
  }
  return true;
}

template<typename STR>
struct svd_printer<EnumeratedValue<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const EnumeratedValue<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name<<">"<<std::endl;
    int i= indent + 1;

    print_svd_value(o,v.name,i,true);
    print_svd_value(o,v.description,i,true);
    print_svd_value(o,v.value,i,true);
    print_svd_value(o,v.isDefault,i,true);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};










} // end namespace svd
}// end namespace nodamushi

#endif // __NODAMUSHI_SVD_ENUMERATEDVALUE_HPP__
