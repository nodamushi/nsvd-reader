/*!
  @brief &lt;enumerationValues&gt; element
  @file nodamushi/svd/Enumeration.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_ENUMERATION_HPP__
#define __NODAMUSHI_SVD_ENUMERATION_HPP__

# include <ostream>
# include <vector>
# include "nodamushi/svd/node.hpp"


# include "nodamushi/svd/create.hpp"
# include "nodamushi/const_string.hpp"
# include "nodamushi/svd/svd_printer.hpp"
# include "nodamushi/svd/EnumUsage.hpp"
# include "nodamushi/svd/EnumeratedValue.hpp"
# include "nodamushi/svd/path.hpp"
# include "nodamushi/svd/svdvisitor.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief &lt;enumeratedValues&gt; element. iterable
 * @note This class name is Enumeration because &lt;enumeratedValues&gt; element type is defined as enumerationType in the SVD Schema File.
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_enumeratedValues
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/schema_1_2_gr.html
 */
template<typename STR>
struct Enumeration:public svd_node
{
  //! @brief derivedFrom attribute
  SVD_ATTR(path<std::string>,derivedFrom);
  //! @brief &lt;name&gt;
  SVD_VALUE(STR,name);
  //! @brief &lt;headerEnumName&gt;
  SVD_VALUE(STR,headerEnumName);
  //! @brief &lt;usage&gt;
  SVD_VALUE(EnumUsage,usage);
  //! @brief &lt;enumeratedValue&gt; list
  SVD_VALUE(std::vector<EnumeratedValue<STR>>,enumeratedValue,true);
  
  //--------------------------------------------------------------
  /**
   * @brief &lt;enumeratedValue&gt; is empty
   * @return &lt;enumeratedValue&gt; is empty
   */
  bool empty()const noexcept{
    if(enumeratedValue) return enumeratedValue->empty();
    return true;
  }
  /**
   * @brief count of &lt;enumeratedValue&gt; 
   * @return count of &lt;enumeratedValue&gt;
   */
  size_t size()const noexcept{
    if(enumeratedValue) return enumeratedValue->size();
    return 0;
  }
  /**
   * @brief count of &lt;enumeratedValue&gt; 
   * @return count of &lt;enumeratedValue&gt;
   */
  size_t length()const noexcept{
    if(enumeratedValue) return enumeratedValue->size();
    return 0;
  }
  using iterator = typename std::vector<EnumeratedValue<STR>>::iterator;
  using const_iterator = typename std::vector<EnumeratedValue<STR>>::const_iterator;
  //! @brief enumeratedValue iterator.
  iterator begin() noexcept{
    if(enumeratedValue) return enumeratedValue->begin();
    return {};
  }
  //! @brief enumeratedValue iterator.
  iterator end() noexcept{
    if(enumeratedValue) return enumeratedValue->end();
    return {};
  }
  //! @brief enumeratedValue iterator.
  const_iterator begin()const noexcept{
    if(enumeratedValue) return enumeratedValue->begin();
    return {};
  }
  //! @brief  enumeratedValue iterator.
  const_iterator end()const noexcept{
    if(enumeratedValue) return enumeratedValue->end();
    return {};
  }
  
};
//----------------------------------------------

template<typename SVD,typename STR>
bool create(SVD& svd,Enumeration<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "derivedFrom")t = (d.derivedFrom = c);
    else if(n == "name") t = (d.name = c);
    else if(n == "headerEnumName") t = (d.headerEnumName = c);
    else if(n == "usage") t = (d.usage = c);
    else if(n == "enumeratedValue"){ 
      auto& vec=d.enumeratedValue.get_for_init();
      size_t s = vec.size();
      vec.emplace_back();
      auto& e = vec[s];
      create(c,e);
    }else if(c.is_attribute()){
      create(c,d);
    }
    if(!t){
      //TODO error
    }
  }
  return true;
}

template<typename STR>
struct svd_printer<Enumeration<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Enumeration<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name;
    print_svd_value(o,v.derivedFrom,1);
    o << ">" << std::endl;

    int i= indent + 1;
    print_svd_value(o,v.name,i,true);
    print_svd_value(o,v.headerEnumName,i,true);
    print_svd_value(o,v.usage,i,true);
    print_svd_value(o,v.enumeratedValue,i,true);
    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};

//---------- SVD Visitor --------------------
__NX_SVD_HANDLE_VISIT(Enumeration)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  // visit enumerated value
  if(t.enumeratedValue){
    r ret= CONTROLLER::apply(t.enumeratedValue.get(),v);
    if(ret == r::BREAK)return r::BREAK;
  }
  return r::CONTINUE;
}};


} // end namespace svd
}// end namespace nodamushi

#endif // __NODAMUSHI_SVD_ENUMERATION_HPP__
