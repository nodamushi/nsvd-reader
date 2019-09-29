/*!
  @brief &lt;field&gt; &lt;fields&gt; element
  @file nodamushi/svd/Field.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef NODAMUSHI_SVD_FIELD_HPP
#define NODAMUSHI_SVD_FIELD_HPP

# include "nodamushi/svd/elements.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief &lt;field&gt; element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_field
 */
template<typename STR>struct Field:public svd_node
{
  //! @brief derivedFrom attribute
  SVD_ATTR(path<std::string>,derivedFrom);
  //! @brief &lt;dim&gt;
  SVD_VALUE(dim_type,dim);
  //! @brief &lt;dimIncrement&gt;
  SVD_VALUE(dimInc_type,dimIncrement);
  //! @brief &lt;dimIndex&gt;
  SVD_VALUE(dimIndex,dimIndex);
  //! @brief &lt;dimName&gt;
  SVD_VALUE(dimName<STR>,dimName);
  //! @brief &lt;dimArrayIndex&gt;
  SVD_VALUE(DimArrayIndex<STR>,dimArrayIndex);
  //! @brief &lt;name&gt;
  SVD_VALUE(STR,name,true);
  //! @brief &lt;description&gt;
  SVD_VALUE(STR,description);
  //! @brief &lt;lsb&gt;
  SVD_VALUE(uint32_t,lsb);
  //! @brief &lt;msb&gt;
  SVD_VALUE(uint32_t,msb);
  //! @brief &lt;bitOffset&gt;
  SVD_VALUE(uint32_t,bitOffset);
  //! @brief &lt;bitWidth&gt;
  SVD_VALUE(uint32_t,bitWidth);
  //! @brief &lt;bitRange&gt;
  SVD_VALUE(bitRange,bitRange);
  //! @brief &lt;access&gt;
  SVD_VALUE(Access,access);
  //! @brief &lt;modifiedWriteValues&gt;
  SVD_VALUE(ModifiedWriteValues,modifiedWriteValues);
  //! @brief &lt;readAction&gt;
  SVD_VALUE(ReadAction,readAction);
  
  //! @brief &lt;enumeration&gt; minOccurs=0,maxOccurs=2
  SVD_VALUE(std::vector<Enumeration<STR>>,enumeratedValues);
};

/**
 * @brief &lt;fields&gt; element. iterable
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_fields
 */
template<typename STR>struct Fields:public svd_node
{
  //! @brief &lt;field&gt; list
  SVD_VALUE(::nodamushi::boxvec<Field<STR>>,field);
  //! @brief &lt;field&gt; is empty.
  bool empty()const noexcept{
    if(field)return field->empty();
    return true;
  }
  //! @brief count of &lt;field&gt;
  size_t size()const noexcept{
    if(field)return field->size();
    return 0;
  }
  //! @brief count of &lt;field&gt;
  size_t length()const noexcept{
    if(field)return field->size();
    return 0;
  }
  using iterator = typename ::nodamushi::boxvec<Field<STR>>::iterator;
  using citerator = typename ::nodamushi::boxvec<Field<STR>>::const_iterator;
  //! @brief &lt;field&gt; iterator
  iterator begin(){
    if(field)return field->begin();
    return {};
  }
  //! @brief &lt;field&gt; iterator
  citerator begin()const{
    if(field)return field->cbegin();
    return {};
  }
  //! @brief &lt;field&gt; iterator
  iterator end(){
    if(field)return field->end();
    return {};
  }
  //! @brief &lt;field&gt; iterator
  citerator end()const{
    if(field)return field->cend();
    return {};
  }
};
//----------------------------------------------


template<typename SVD,typename STR>
bool create(SVD& svd,Field<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "derivedFrom")t = (d.derivedFrom = c);
    else if(n == "dim")t = (d.dim = c);
    else if(n == "dimIncrement")t = (d.dimIncrement = c);
    else if(n == "dimIndex")t = (d.dimIndex = c);
    else if(n == "dimName")t = (d.dimName = c);
    else if(n == "dimArrayIndex")t = (d.dimArrayIndex = c);
    else if(n == "name") t = (d.name = c);
    else if(n == "description")t = (d.description = c);
    else if(n == "lsb") t = (d.lsb = c);
    else if(n == "msb") t = (d.msb = c);
    else if(n == "bitOffset") t = (d.bitOffset = c);
    else if(n == "bitWidth") t = (d.bitWidth = c);
    else if(n == "bitRange") t = (d.bitRange = c);
    else if(n == "access") t = (d.access = c);
    else if(n == "modifiedWriteValues") t = (d.modifiedWriteValues = c);
    else if(n == "readAction") t = (d.readAction = c);
    else if(n == "enumeratedValues")t = (d.enumeratedValues = c);
    else if(c.is_attribute()){
      create(c,d);
    }
    else c.unknown_element(svd_element::Field);
    if(!t){
      c.illegal_value(svd_element::Field);
    }
  }
  return true;
}

template<typename STR>
struct svd_printer<Field<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Field<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name;
    print_svd_value(o,v.derivedFrom,1);
    o << ">" << std::endl;


    int i= indent + 1;

    print_svd_value(o,v.dim,i,true);
    print_svd_value(o,v.dimIncrement,i,true);
    print_svd_value(o,v.dimIndex,i,true);
    print_svd_value(o,v.dimName,i,true);
    print_svd_value(o,v.dimArrayIndex,i,true);

    print_svd_value(o,v.name,i,true);
    print_svd_value(o,v.description,i,true);
    print_svd_value(o,v.lsb,i,true);
    print_svd_value(o,v.msb,i,true);
    print_svd_value(o,v.bitOffset,i,true);
    print_svd_value(o,v.bitWidth,i,true);
    print_svd_value(o,v.bitRange,i,true);
  
    print_svd_value(o,v.access,i,true);
  
    print_svd_value(o,v.modifiedWriteValues,i,true);
    print_svd_value(o,v.readAction,i,true);
    print_svd_value(o,v.enumeratedValues,i,true);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};



template<typename SVD,typename STR>
bool create(SVD& svd,Fields<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    if(n == "field")d.field = c;
    else c.unknown_element(svd_element::Fields);
  }
  return true;
}

template<typename STR>
struct svd_printer<Fields<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Fields<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name << ">" <<std::endl;
    int i= indent + 1;
    print_svd_value(o,v.field,i,true);
    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};
//---------- SVD Visitor --------------------
__NX_SVD_HANDLE_VISIT(Field)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  // visit enumerated values
  if(t.enumeratedValues){
    r ret= CONTROLLER::apply(t.enumeratedValues.get(),v);
    if(ret == r::BREAK)return r::BREAK;
  }
  return r::CONTINUE;
}};



} // end namespace svd
}// end namespace nodamushi

#endif // NODAMUSHI_SVD_FIELD_HPP
