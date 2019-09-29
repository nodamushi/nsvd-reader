/*!
 * @brief &lt;register&gt; element
 * @file nodamushi/svd/Register.hpp
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_REGISTER_HPP
#define NODAMUSHI_SVD_REGISTER_HPP

# include "nodamushi/svd/elements.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief &lt;register&gt; element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_register
 * @see nodamushi::svd::Peripheral
 * @see nodamushi::svd::Cluster
 * @see nodamushi::svd::Field
 * @see nodamushi::svd::normalized::Register
 */
template<typename STR>struct Register:public svd_node
{
  static constexpr bool IS_REGISTER=true;// to  distinguish between Register and Cluster.
  static constexpr bool IS_CLUSTER =false;
  //! @brief derivedFrom attribute
  SVD_ATTR(path<std::string>,derivedFrom);
  //! @brief &lt;dim&gt;
  SVD_VALUE(dim_type,dim);
  //! @brief &lt;dimIncrement&gt;
  SVD_VALUE(dimInc_type,dimIncrement);
  //! @brief &lt;dimIndex&gt;
  SVD_VALUE(dimIndex,dimIndex);
  //! @brief  &lt;dimName&gt;
  SVD_VALUE(dimName<STR>,dimName);
  //! @brief  &lt;dimArrayIndex&gt;
  SVD_VALUE(DimArrayIndex<STR>,dimArrayIndex);
  
  //! @brief  &lt;name&gt; this element is required.
  SVD_VALUE(STR,name,true);
  //! @brief  &lt;displayName&gt;
  SVD_VALUE(STR,displayName);
  //! @brief  &lt;description&gt;
  SVD_VALUE(STR,description);
  //! @brief  &lt;alternateGroup&gt;
  SVD_VALUE(STR,alternateGroup);
  //! @brief  &lt;alternateRegister&gt;
  SVD_VALUE(STR,alternateRegister);
  //! @brief  &lt;addressOffset&gt;
  SVD_VALUE(hex64,addressOffset,true);
  //! @brief  &lt;size&gt;
  SVD_VALUE(hex32,size);
  //! @brief  &lt;access&gt;
  SVD_VALUE(Access, access);
  //! @brief  &lt;protection&gt;
  SVD_VALUE(Protection,protection);
  //! @brief  &lt;resetValue&gt;
  SVD_VALUE(hex64,resetValue);
  //! @brief  &lt;resetMask&gt;
  SVD_VALUE(hex64,resetMask);
  //! @brief  &lt;datatype&gt;
  SVD_VALUE(DataType,dataType);
  //! @brief  &lt;modifiedWriteValues&gt;
  SVD_VALUE(ModifiedWriteValues,modifiedWriteValues);
  //! @brief  &lt;writeConstraint&gt;
  SVD_VALUE(WriteConstraint,writeConstraint);
  //! @brief  &lt;readAction&gt;
  SVD_VALUE(ReadAction,readAction);
  //! @brief  &lt;fields&gt;
  SVD_VALUE(Fields<STR>,fields);
};
//----------------------------------------------

template<typename SVD,typename STR>
bool create(SVD& svd,Register<STR>& d)
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
    else if(n == "displayName") t = (d.displayName = c);
    else if(n == "description") t = (d.description = c);
    else if(n == "alternateGroup") t = (d.alternateGroup = c);
    else if(n == "alternateRegister") t = (d.alternateRegister = c);
    else if(n == "addressOffset") t = (d.addressOffset = c);
    else if(n == "size") t = (d.size = c);
    else if(n == "access")t = (d.access = c);
    else if(n == "protection") t = (d.protection = c);
    else if(n == "resetValue") t = (d.resetValue = c);
    else if(n == "resetMask") t = (d.resetMask = c);
    else if(n == "dataType") t = (d.dataType = c);
    else if(n == "modifiedWriteValues") t = (d.modifiedWriteValues = c);
    else if(n == "writeConstraint")t = (d.writeConstraint = c);
    else if(n == "readAction") t = (d.readAction = c);
    else if(n == "fields")t = (d.fields = c);
    else if(c.is_attribute()){
      create(c,d);
    }
    else c.unknown_element(svd_element::Register);
    if(!t){
      c.illegal_value(svd_element::Register);
    }
  }
  return true;
}

template<typename STR>
struct svd_printer<Register<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Register<STR>& v,
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
    print_svd_value(o,v.displayName,i,true);
    print_svd_value(o,v.description,i,true);
    print_svd_value(o,v.alternateGroup,i,true);
    print_svd_value(o,v.alternateRegister,i,true);
    
    print_svd_value(o,v.addressOffset,i,true);
    print_svd_value(o,v.size,i,true);
    print_svd_value(o,v.access,i,true);
    print_svd_value(o,v.protection,i,true);
    print_svd_value(o,v.resetValue,i,true);
    print_svd_value(o,v.resetMask,i,true);
    print_svd_value(o,v.dataType,i,true);
    print_svd_value(o,v.modifiedWriteValues,i,true);
    print_svd_value(o,v. writeConstraint,i,true);

    print_svd_value(o,v.readAction,i,true);
    print_svd_value(o,v.fields,i,true);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};


//---------- SVD Visitor --------------------
__NX_SVD_HANDLE_VISIT(Register)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  // visit Field
  if(t.fields && t.fields->field){
    r ret = CONTROLLER::apply(t.fields->field.get(),v);
    if(ret == r::BREAK)return r::BREAK;
  }
  return r::CONTINUE;
}};

} // end namespace svd
}// end namespace nodamushi

#endif // NODAMUSHI_SVD_REGISTER_HPP
