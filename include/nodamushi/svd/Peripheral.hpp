/*!
 * @brief &lt;peripheral&gt; element
 * @file nodamushi/svd/Peripheral.hpp
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_PERIPHERAL_HPP__
#define __NODAMUSHI_SVD_PERIPHERAL_HPP__

# include <ostream>
# include <vector>
# include <memory>
# include <type_traits>
# include "nodamushi/svd/node.hpp"

# include "nodamushi/boxvec.hpp"
# include "nodamushi/svd/path.hpp"

# include "nodamushi/svd/Access.hpp"
# include "nodamushi/svd/Interrupt.hpp"
# include "nodamushi/svd/DimArrayIndex.hpp"
# include "nodamushi/svd/AddressBlock.hpp"
# include "nodamushi/svd/Cluster.hpp"
# include "nodamushi/svd/Protection.hpp"

# include "nodamushi/svd/create.hpp"
# include "nodamushi/const_string.hpp"
# include "nodamushi/svd/svd_printer.hpp"

# include "nodamushi/svd/value.hpp"

# include "nodamushi/svd/dim_type.hpp"
# include "nodamushi/svd/svdvisitor.hpp"
namespace nodamushi{
namespace svd{


/**
 * @brief &lt;peripheral&gt; element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_peripherals.html#elem_peripheral
 * @see nodamushi::svd::normalized::Peripheral
 */
template<typename STR>struct Peripheral:public svd_node
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
  //! @brief &lt;version&gt;
  SVD_VALUE(STR,version);
  //! @brief &lt;description&gt;
  SVD_VALUE(STR,description);
  //! @brief &lt;alternatePeripheral&gt;
  SVD_VALUE(STR,alternatePeripheral);
  //! @brief &lt;description&gt;
  SVD_VALUE(STR,groupName);
  SVD_VALUE(STR,prependToName);
  SVD_VALUE(STR,appendToName);
  //! @brief &lt;headerStructName&gt;
  SVD_VALUE(STR,headerStructName);
  //! @brief &lt;disableCondition&gt;
  SVD_VALUE(STR,disableCondition);

  SVD_VALUE(hex64,baseAddress,true);
  SVD_VALUE(hex32,size);

  SVD_VALUE(Access, access);
  SVD_VALUE(Protection,protection);
  SVD_VALUE(hex64,resetValue);
  SVD_VALUE(hex64,resetMask);
  SVD_VALUE(std::vector<AddressBlock>,addressBlock);
  SVD_VALUE(std::vector<Interrupt<STR>>,interrupt);
  SVD_VALUE(Registers<STR>,registers);
};

/**
 * @brief &lt;peripherals&gt; element. iterable
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_peripherals.html
 * @see nodamushi::svd::Peripheral
 */
template<typename STR>struct Peripherals:public svd_node
{
  //! @brief &lt;peripheral&gt; list
  SVD_VALUE(boxvec<Peripheral<STR>>,peripheral);
  //! @brief &lt;peripheral&gt; is empty
  bool empty()const noexcept{
    if(peripheral)return peripheral->empty();
    return true;
  }
  //! @brief count of &lt;peripheral&gt; 
  size_t size()const noexcept{
    if(peripheral)return peripheral->size();
    return 0;
  }
  //! @brief count of &lt;peripheral&gt; 
  size_t length()const noexcept{
    if(peripheral)return peripheral->size();
    return 0;
  }
  using iterator = typename ::nodamushi::boxvec<Peripheral<STR>>::iterator;
  using citerator = typename ::nodamushi::boxvec<Peripheral<STR>>::const_iterator;
  //! @brief &lt;peripheral&gt; iterator
  iterator begin(){
    if(peripheral)return peripheral->begin();
    return {};
  }
  //! @brief &lt;peripheral&gt; iterator
  citerator begin()const{
    if(peripheral)return peripheral->cbegin();
    return {};
  }
  //! @brief &lt;peripheral&gt; iterator
  iterator end(){
    if(peripheral)return peripheral->end();
    return {};
  }
  //! @brief &lt;peripheral&gt; iterator
  citerator end()const{
    if(peripheral)return peripheral->cend();
    return {};
  }
};

//----------------------------------------------
template<typename SVD,typename STR>
bool create(SVD& svd,Peripheral<STR>& d)
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
    else if(n == "name")t = (d.name = c);
    else if(n == "version")t = (d.version = c);
    else if(n == "description")t = (d.description = c);
    else if(n == "alternatePeripheral")t = (d.alternatePeripheral = c);
    else if(n == "groupName")t = (d.groupName = c);
    else if(n == "prependToName")t = (d.prependToName = c);
    else if(n == "appendToName")t = (d.appendToName = c);
    else if(n == "headerStructName")t = (d.headerStructName = c);
    else if(n == "disableCondition")t = (d.disableCondition = c);
    else if(n == "baseAddress")t = (d.baseAddress = c);
    else if(n == "size")t = (d.size = c);
    else if(n == "access")t = (d.access=c);
    else if(n == "protection")t = (d.protection = c);
    else if(n == "resetValue")t = (d.resetValue = c);
    else if(n == "resetMask")t = (d.resetMask = c);
    else if(n == "addressBlock")t = (d.addressBlock = c);
    else if(n == "interrupt")t = (d.interrupt = c);
    else if(n == "registers")t = (d.registers = c);
    else if(c.is_attribute()){
      create(c,d);
    }
    if(!t){
      //TODO error
    }
  }
  return true;
}

template<typename STR>
struct svd_printer<Peripheral<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Peripheral<STR>& v,
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
    print_svd_value(o,v.version,i,true);
    print_svd_value(o,v.description,i,true);
    print_svd_value(o,v.alternatePeripheral,i,true);
    print_svd_value(o,v.groupName,i,true);
    print_svd_value(o,v.prependToName,i,true);
    print_svd_value(o,v.appendToName,i,true);
    print_svd_value(o,v.headerStructName,i,true);
    print_svd_value(o,v.disableCondition,i,true);

    print_svd_value(o,v.baseAddress,i,true);
    print_svd_value(o,v.size,i,true);

    print_svd_value(o,v.access,i,true);
    print_svd_value(o,v.protection,i,true);
    print_svd_value(o,v.resetValue,i,true);
    print_svd_value(o,v.resetMask,i,true);
    print_svd_value(o,v.addressBlock,i,true);
    print_svd_value(o,v.interrupt,i,true);
    print_svd_value(o,v.registers,i,true);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};


//---------- SVD Visitor --------------------
__NX_SVD_HANDLE_VISIT(Peripheral)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  if(t.interrupt){
    auto ret = CONTROLLER::apply(t.interrupt.get(),v);
    if(ret == r::BREAK)return r::BREAK;
    if(ret == r::SKIP)return r::CONTINUE;
  }
  if(t.addressBlock){
    auto ret = CONTROLLER::apply(t.addressBlock.get(),v);
    if(ret == r::BREAK)return r::BREAK;
    if(ret == r::SKIP)return r::CONTINUE;
  }

  // visit register / cluster
  if(t.registers){
    auto ret = reg_or_cluster_visit::visit<CONTROLLER>(*t.registers,v);
    if(ret == r::BREAK)return r::BREAK;
  }
  return r::CONTINUE;
}};
//--------------------------------------------


template<typename SVD,typename STR>
bool create(SVD& svd,Peripherals<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    if(n == "peripheral")d.peripheral = c;
  }
  return true;
}

template<typename STR>
struct svd_printer<Peripherals<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Peripherals<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name << ">" <<std::endl;
    int i= indent + 1;
    print_svd_value(o,v.peripheral,i,true);
    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};



} // end namespace svd
}// end namespace nodamushi

#endif // __NODAMUSHI_SVD_PERIPHERAL_HPP__
