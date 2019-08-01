/**
 * @brief &lt;cpu&gt; element
 * @file nodamushi/svd/Cpu.hpp
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_CPU_HPP__
#define __NODAMUSHI_SVD_CPU_HPP__
# include <ostream>
# include <vector>
# include "nodamushi/svd/node.hpp"

# include "nodamushi/svd/Endian.hpp"

# include "nodamushi/svd/create.hpp"
# include "nodamushi/const_string.hpp"
# include "nodamushi/svd/svd_printer.hpp"

# include "nodamushi/svd/value.hpp"
# include "nodamushi/svd/SAURegionConfigProtect.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief Security Attribution Unit(SAU) &lt;region&gt;
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_cpu.html#elem_region
 * @see SAURegionConfigProtect
 */
template<typename STR=std::string>
struct SAURegionsConfigRegion:public svd_node
{
  //! @brief attribute enabled
  SVD_ATTR(bool,enabled);
  //! @brief attribute name
  SVD_ATTR(STR,name);
  
  //! @brief &lt;base&gt; element
  SVD_VALUE(uint64_t,base);
  //! @brief &lt;limit&gt; element
  SVD_VALUE(uint64_t,limit);
  //! @brief &lt;access&gt;
  SVD_VALUE(SAURegionConfigProtect,access);
};

/**
 * @brief &lt;sauRegionsConfig&gt; Security Attribution Unit(SAU).
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_cpu.html#elem_sauRegionsConfig
 * @see nodamushi::svd::SAURegionsConfigRegion
 */
template<typename STR=std::string>
struct SAURegionsConfig :public svd_node
{
  //! @brief attribute enabled
  SVD_ATTR(bool,enabled);
  //! @brief attribute protectionWhenDisabled
  SVD_ATTR(SAURegionConfigProtect,protectionWhenDisabled);
  //! @brief &lt;region&gt; elements.
  std::vector<SAURegionsConfigRegion<STR>> region;
  /**
   * @brief get &lt;region&gt; object
   * @param i indexn
   * @return region[i]
   */
  SAURegionsConfigRegion<STR>& operator[](size_t i){return region[i];}
  /**
   * @brief get &lt;region&gt; object
   * @param i indexn
   * @return region[i]
   */
  const SAURegionsConfigRegion<STR>& operator[](size_t i)const{return region[i];}
  //! @brief count of &lt;region&gt; elements
  size_t size()const{return region.size();}
  //! @brief count of &lt;region&gt; elements
  size_t length()const{return region.length();}
  
  using iterator =typename  std::vector<SAURegionsConfigRegion<STR>>::iterator;
  using const_iterator =typename  std::vector<SAURegionsConfigRegion<STR>>::iterator;
  //! @brief &lt;region&gt; iterator
  iterator begin(){return region.begin();}
  //! @brief &lt;region&gt; iterator
  iterator begin()const{return region.begin();}
  //! @brief &lt;region&gt; iterator
  iterator end(){return region.end();}
  //! @brief &lt;region&gt; iterator
  iterator end()const{return region.end();}
};

/**
 * @brief &lt;cpu&gt; element
 * @see nodamushi::svd::Device
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_cpu.html
 */
template<typename STR=std::string>struct Cpu:public svd_node
{
  /**
   *  @brief &lt;name&gt; element
   *  @see nodamushi::svd::ARMCPU
   *  @see nodamushi::svd::get_cpu_type()
   */
  SVD_VALUE(STR,name,true);
  //! @brief &lt;revision&gt; element
  SVD_VALUE(STR,revision,true);
  //! @brief &lt;endian&gt; element
  SVD_VALUE(Endian,endian,true);
  //! @brief &lt;mpuPresent&gt; element
  SVD_VALUE(bool,mpuPresent);
  //! @brief &lt;fpuPresent&gt; element
  SVD_VALUE(bool,fpuPresent);
  //! @brief &lt;fpuDP&gt; element
  SVD_VALUE(bool,fpuDP);
  //! @brief &lt;dspPresent&gt; element
  SVD_VALUE(bool,dspPresent);
  //! @brief &lt;icachePresent&gt; element
  SVD_VALUE(bool,icachePresent);
  //! @brief &lt;dcachePresent&gt; element
  SVD_VALUE(bool,dcachePresent);
  //! @brief &lt;itcmPresent&gt; element
  SVD_VALUE(bool,itcmPresent);
  //! @brief &lt;dtcmPresent&gt; element
  SVD_VALUE(bool,dtcmPresent);
  //! @brief &lt;vtorPresent&gt; element
  SVD_VALUE(bool,vtorPresent);
  //! @brief &lt;nvicPrioBits&gt; element
  SVD_VALUE(uint32_t,nvicPrioBits,true);
  //! @brief &lt;vendorSysticConfig&gt; element
  SVD_VALUE(bool,vendorSystickConfig);
  //! @brief &lt;deviceNumInterrupts&gt; element
  SVD_VALUE(uint32_t,deviceNumInterrupts);
  //! @brief &lt;sauNumRegions&gt; element
  SVD_VALUE(uint32_t,sauNumRegions);
  //! @brief &lt;sauRegionsConfig&gt; element
  SVD_VALUE(SAURegionsConfig<STR>,sauRegionsConfig);
};



template<typename SVD,typename STR>
bool create(SVD& svd,SAURegionsConfigRegion<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "enabled") t = (d.enabled = c);
    else if(n == "name") t = (d.name = c);
    else if(n == "base") t = (d.base = c);
    else if(n == "limit") t = (d.limit = c);
    else if(n == "access") t = (d.access = c);
    else if(c.is_attribute()){
      create(c,d);
    }
    if(!t){
      //TODO error
    }
  }
  return true;
}
template<typename SVD,typename STR>
bool create(SVD& svd,SAURegionsConfig<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "enabled") t = (d.enabled = c);
    else if(n == "protectionWhenDisabled") t = (d.protectionWhenDisabled = c);
    else if(n == "region"){
      size_t i = d.region.size();
      d.region.emplace_back();
      create(c,d.region[i]);
    }else if(c.is_attribute()){
      create(c,d);
    }
    if(!t){
      //TODO error
    }
  }
  return true;
}

template<typename SVD,typename STR>
bool create(SVD& svd,Cpu<STR>& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "name") t = (d.name = c);
    else if(n == "revision") t = (d.revision = c);
    else if(n == "endian") t = (d.endian = c);
    else if(n == "mpuPresent")t = (d.mpuPresent = c);
    else if(n == "fpuPresent") t = (d.fpuPresent = c);
    else if(n == "fpuDP") t = (d.fpuDP = c);
    else if(n == "dspPresent") t = (d.dspPresent = c);
    else if(n == "icachePresent") t = (d.icachePresent = c);
    else if(n == "dcachePresent") t = (d.dcachePresent = c);
    else if(n == "itcmPresent") t = (d.itcmPresent = c);
    else if(n == "dtcmPresent") t = (d.dtcmPresent = c);
    else if(n == "vtorPresent") t = (d.vtorPresent = c);
    else if(n == "nvicPrioBits") t = (d.nvicPrioBits = c);
    else if(n == "vendorSystickConfig") t = (d.vendorSystickConfig = c);
    else if(n == "deviceNumInterrupts") t = (d.deviceNumInterrupts = c);
    else if(n == "sauNumRegions") t = (d.sauNumRegions = c);
    else if(n == "sauRegionsConfig")t = (d.sauRegionsConfig=c);
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
struct svd_printer<SAURegionsConfigRegion<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const SAURegionsConfigRegion<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name;
    print_svd_value(o,v.enabled,1);
    print_svd_value(o,v.name,1);
    o << ">" << std::endl;

    int i = indent + 1;

  
    print_svd_value(o,v.base,i,true);
    print_svd_value(o,v.limit,i,true);
    print_svd_value(o,v.access,i,true);
    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};


template<typename STR>
struct svd_printer<SAURegionsConfig<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const SAURegionsConfig<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name;
    print_svd_value(o,v.enabled,1);
    print_svd_value(o,v.protectionWhenDisabled,1);
    o << ">" << std::endl;

    int i = indent + 1;

    for(const auto& r:v.region)
      svd_printer<SAURegionsConfigRegion<STR>>::print(o,"region",r,i);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};



template<typename STR>
struct svd_printer<Cpu<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Cpu<STR>& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name << ">" << std::endl;
    int i = indent + 1;

    print_svd_value(o,v.name,i,true);
    print_svd_value(o,v.revision,i,true);
    print_svd_value(o,v.endian,i,true);
    print_svd_value(o,v.mpuPresent,i,true);
    print_svd_value(o,v.fpuPresent,i,true);
    print_svd_value(o,v.fpuDP,i,true);
    print_svd_value(o,v.dspPresent,i,true);
    print_svd_value(o,v.icachePresent,i,true);
    print_svd_value(o,v.dcachePresent,i,true);
    print_svd_value(o,v.itcmPresent,i,true);
    print_svd_value(o,v.dtcmPresent,i,true);
    print_svd_value(o,v.vtorPresent,i,true);
    print_svd_value(o,v.nvicPrioBits,i,true);
    print_svd_value(o,v.vendorSystickConfig,i,true);
    print_svd_value(o,v.deviceNumInterrupts,i,true);
    print_svd_value(o,v.sauNumRegions,i,true);
    print_svd_value(o,v.sauRegionsConfig,i,true);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};




} // end namespace svd
}// end namespace nodamushi

#endif // __NODAMUSHI_SVD_CPU_HPP__
