/*!
  @brief &lt;device&gt; svd file root element
  @file nodamushi/svd/Device.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_DEVICE_HPP__
#define __NODAMUSHI_SVD_DEVICE_HPP__

# include <ostream>
# include "nodamushi/svd/node.hpp"

# include "nodamushi/svd/Access.hpp"
# include "nodamushi/svd/Peripheral.hpp"
# include "nodamushi/svd/Cpu.hpp"

# include "nodamushi/svd/create.hpp"
# include "nodamushi/const_string.hpp"
# include "nodamushi/svd/svd_printer.hpp"
# include "nodamushi/svd/Protection.hpp"
# include "nodamushi/svd/value.hpp"
# include "nodamushi/svd/svdvisitor.hpp"

# include "nodamushi/svd/normalized/Device.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief user define vender extension builder
 @code
 template<typename SVD>
 void vendorextensions_builder<YOUR_TYPE>::build(SVD& svd,YOUR_TYPE& dst)
 {
   dst.xxx = yyy;
 }
 @endcode
 */
template<typename VendorExtensions>struct vendorextensions_builder
{
  /**
   * @brief set data from svd file 
   * @param svd SVD xml data.
   * @param dst output data
   */
  template<typename SVD>static void build(SVD& svd,VendorExtensions& dst);
};


/**
 * @brief Device.
 @par
 &lt;device&gt; is a root element of the SVD file.

 @par
 This class represents SVD (xml) information as it is.
 Therefore, there is a possibility that each element does not have value because it dose not be defined in the source SVD file. 
 
 @code
 // samplecode
 
 // read SVD file with boost (* include "nodamushi/svd/boost.hpp")
 nodamushi::svd::boost_svd_reader svd_reader(filename);
 // read and create Device 
 nodamushi::svd::Device<> device(svd_reader);

 // get cpu name
 if(device.cpu){
   std::cout << device.cpu->name.get("Undefined") << std::endl;
 }else{
   std::cout << "CPU element is not defined" << std::endl;
 }

 // set value
 device.description = "My Device";

 // print SVD(xml)
 std::cout << device <<std::endl;

 // access to Peripheral
 if(device.peripherals && device.peripherals->peripheral){
   for(const auto& p : device.peripherals->peripheral.get()){
     std::cout << p.name.get("undefined") << std::endl;
   }
 }
 @endcode
 * @par
 * It's hard to write code that accesses in loops.
 * For simplicity, a visitor pattern class have be prepared to access all major elements.
 * @par
 * see sample code 'sample/visitor_sample.cpp'
 * @param STR string type. default type is std::string
 * @param VendorExtensions. default type is void
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_device.html
 * @see nodamushi::svd::normalized::Device
 * @see nodamushi::svd::normalize()
 */
template<typename STR=std::string,typename VendorExtensions=void>
struct Device:public svd_node
{
  Device()=default;
  Device(const Device&)=delete; // move only
  Device& operator=(Device&&)=default;
  Device(Device&&)=default;
  Device& operator=(const Device&)=delete;
  
  //! attribute: schemaVersion
  SVD_ATTR(STR,schemaVersion,true);
  //! attribute: xmlns:xs
  SVD_TYPE(STR,true,true,xmlns:xs) xs;
  //! attribute: noNamespaceSchemaLocation
  SVD_TYPE(STR,true,true,xs:noNamespaceSchemaLocation) noNamespaceSchemaLocation;
  //------------------------------------------------

  //! @brief &lt;vendor&gt;
  SVD_VALUE(STR,vendor);
  //! @brief &lt;vendorID&gt;
  SVD_VALUE(STR,vendorID);
  //! @brief &lt;name&gt;
  SVD_VALUE(STR,name,true);
  //! @brief &lt;seriese&gt;
  SVD_VALUE(STR,series,true);
  //! @brief &lt;version&gt;
  SVD_VALUE(STR,version,true);
  //! @brief &lt;description&gt;
  SVD_VALUE(STR,description,true);
  //! @brief &lt;licenseText&gt;
  SVD_VALUE(STR,licenseText);
  //! @brief &lt;cpu&gt;
  SVD_VALUE(Cpu<STR>,cpu);
  //! @brief &lt;headerSystemFilename&gt;
  SVD_VALUE(STR,headerSystemFilename);
  //! @brief &lt;headerDefinitionsPrefix&gt;
  SVD_VALUE(STR,headerDefinitionsPrefix);
  //! @brief &lt;addressUnitBits&gt;
  SVD_VALUE(uint32_t,addressUnitBits,true);
  //! @brief &lt;width&gt;
  SVD_VALUE(uint32_t,width,true);
  //! @brief &lt;size&gt;
  SVD_VALUE(uint32_t,size);
  //! @brief &lt;access&gt;
  SVD_VALUE(Access,access);
  //! @brief &lt;protection&gt;
  SVD_VALUE(Protection,protection);
  //! @brief &lt;resetValue&gt;
  SVD_VALUE(hex64,resetValue);
  //! @brief &lt;resetMask&gt;
  SVD_VALUE(hex64,resetMask);
  //! @brief &lt;peripherals&gt;
  SVD_VALUE(Peripherals<STR>,peripherals,true);
  //! @brief &lt;vendorExtensions&gt;
  SVD_VALUE(VendorExtensions,vendorExtensions);
  
  /**
   * @brief constructor
   * @param svd SVD reader
   */
  template<typename SVD> Device(SVD& svd):Device(){ build(*this,svd);}

 private:
  template<typename SVD>
  static void build(Device& d,SVD& svd)
  {
    if(svd.get_name() != "device"){
      if(svd){
        decltype(svd.next_child()) c = svd.next_child();
        return build(d,c);
      }
    }else  create(svd,d);
  }
};


//---------------------------------------------------------------------------------
template<typename VendorExtensions,bool attribute,bool required,char... name>
struct vendorextensions_builder<value<VendorExtensions,attribute,required,name...>>
{
  template<typename SVD>
  static void build(SVD& svd,value<VendorExtensions,false,false,name...>& d)
  {
    vendorextensions_builder<VendorExtensions>::build(svd,d.get_for_init());
  }
};
template<bool attribute,bool required,char... name>
struct vendorextensions_builder<value<void,attribute,required,name...>>
{
  template<typename SVD>
  static void build(SVD& svd,value<void,false,false,name...>& d){}
};


template<typename SVD,typename STR,typename VendorExtensions>
bool create(SVD& svd,Device<STR,VendorExtensions>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "schemaVersion") t = (d.schemaVersion=c);
    else if(n == d.xs.get_name()) t = (d.xs = c);
    else if(n == d.noNamespaceSchemaLocation.get_name()) 
      t = (d.noNamespaceSchemaLocation = c);
    else if(n == "vendor")t = (d.vendor=c);
    else if(n == "vendorID") t = (d.vendorID=c);
    else if(n == "name") t = (d.name=c);
    else if(n == "series") t = (d.series=c);
    else if(n == "version") t = (d.version=c);
    else if(n == "description") t = (d.description=c);
    else if(n == "licenseText") t = (d.licenseText=c);
    else if(n == "cpu")  create(c,d.cpu.get_for_init());
    else if(n == "headerSystemFilename") t = (d.headerSystemFilename = c);
    else if(n == "headerDefinitionsPrefix") t = (d.headerDefinitionsPrefix = c);
    else if(n == "addressUnitBits") t = (d.addressUnitBits = c);
    else if(n == "width") t = (d.width = c);
    else if(n == "size") t = (d.size = c);
    else if(n == "access") t = (d.access = c);
    else if(n == "protection") t = (d.protection = c);
    else if(n == "resetValue") t = (d.resetValue = c);
    else if(n == "resetMask") t = (d.resetMask = c);
    else if(n == "peripherals")t = (d.peripherals = c);
    else if(n == "vendorExtensions"){
      vendorextensions_builder<decltype(d.vendorExtensions)>::build(c,d.vendorExtensions);
    }else if(c.is_attribute()){
      create(c,d);
    }
    if(!t){
      //TODO error
    }
  }
  return true;
}
//---------------------------------------------------------------------------------
template<typename STR,typename V>
struct svd_printer<Device<STR,V>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Device<STR,V>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name;
    print_svd_value(o,v.schemaVersion,1);
    print_svd_value(o,v.xs,1);
    print_svd_value(o,v.noNamespaceSchemaLocation,1);
    o << ">" << std::endl;

    int i = indent + 1;

    print_svd_value(o,v.vendor,i,true);
    print_svd_value(o,v.vendorID,i,true);
    print_svd_value(o,v.name,i,true);
    print_svd_value(o,v.series,i,true);
    print_svd_value(o,v.version,i,true);
    print_svd_value(o,v.description,i,true);
    print_svd_value(o,v.licenseText,i,true);
    print_svd_value(o,v.cpu,i,true);
    print_svd_value(o,v.headerSystemFilename,i,true);
    print_svd_value(o,v.headerDefinitionsPrefix,i,true);
    print_svd_value(o,v.addressUnitBits,i,true);
    print_svd_value(o,v.width,i,true);
    print_svd_value(o,v.size,i,true);
    print_svd_value(o,v.access,i,true);
    print_svd_value(o,v.protection,i,true);
    print_svd_value(o,v.resetValue,i,true);
    print_svd_value(o,v.resetMask,i,true);
    print_svd_value(o,v.peripherals,i,true);
    print_svd_value(o,v.vendorExtensions,i,true);
    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};


//---------- SVD Visitor --------------------
__NX_SVD_HANDLE_VISIT2(Device)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  // visit Peripheral
  if(t.cpu){
    auto ret = CONTROLLER::apply(t.cpu.get(),v);
    if(ret==r::BREAK)return r::BREAK;
    else if(ret == r::SKIP)return r::CONTINUE;
  }
  if(t.peripherals && (*t.peripherals).peripheral){
    auto ret= CONTROLLER::apply(t.peripherals.get().peripheral.get(),v);
    if(ret==r::BREAK)return r::BREAK;
  }
  return r::CONTINUE;
}};

}} // end namespace nodamushi::svd


namespace std{
template<typename STR,typename VendorExtensions>
std::ostream& operator<<(std::ostream& o,const nodamushi::svd::Device<STR,VendorExtensions>& v)
{
  using namespace nodamushi::svd;
  o << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl << std::endl;
  return svd_printer<Device<STR,VendorExtensions>>::print(o,"device",v,0);
}
} // end namespace std

#endif // __NODAMUSHI_SVD_DEVICE_HPP__
