/*!
  @brief normalized Device
  @file nodamushi/svd/normalized/Device.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_DEVICE_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_DEVICE_HPP__

# include <ostream>
# include <memory>
# include "nodamushi/svd/normalized/node_container.hpp"
# include "nodamushi/svd/normalized/Cpu.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{

/**
 * @brief normalized Device
 * @par
 * Root class. Use nodamushi::svd::normalized() function to create this intance.
 * @code
 * namespace svd = nodamushi::svd;
 * namespace norm= nodamushi::svd::normalized;
 * svd::Device<> deviece(svd_reader);
 * //
 * // normalize device.
 * // the result pointer is wrapped by shared_ptr.
 * // 
 * norm::node_ptr<Device<>> ptr = svd::normalize(device); 
 * std::cout << ptr->peripheral[0].name << std::endl;
 * @endcode
 * @param STRREF string type.
 * @par 
 * In C++14, the default text type is 'std::string'.
 * In C++17, the default text type is 'std::string_view'. 
 * std::string_view refers to the string of svd::Device.
 *
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_device.html
 * @see nodamushi::svd::Device
 * @see nodamushi::svd::normalized::normalize()
 */
template<typename STRREF>struct Device
{
  using this_t = Device<STRREF>;
  using Peripheral = ::nodamushi::svd::normalized::Peripheral<STRREF>;
  using Register=::nodamushi::svd::normalized::Register<STRREF>;
  using Cluster =::nodamushi::svd::normalized::Cluster<STRREF>;
  using Field=::nodamushi::svd::normalized::Field<STRREF>;
  using Cpu     =::nodamushi::svd::normalized::Cpu<STRREF>;

  //! @brief &lt;vendor&gt;
  STRREF           vendor;
  //! @brief &lt;vendorID&gt;
  STRREF           vendorID;
  //! @brief &lt;name&gt;
  STRREF           name;
  //! @brief &lt;series&gt;
  STRREF           series;
  //! @brief &lt;version&gt;
  STRREF           version;
  //! @brief &lt;description&gt;
  STRREF           description;
  //! @brief &lt;licenseText&gt;
  STRREF           licenseText;
  //! @brief &lt;headerSystemFilename&gt;
  STRREF           headerSystemFilename;
  //! @brief &lt;headerDefinitionsPrefix&gt;
  STRREF           headerDefinitionsPrefix;
  //! @brief &lt;addressUnitBits&gt;
  uint32_t         addressUnitBits;
  //! @brief &lt;width&gt;
  uint32_t         width;
  //! @brief &lt;size&gt;
  uint32_t         size;
  //! @brief &lt;access&gt;
  Access           access;
  //! @brief &lt;protection&gt;
  Protection       protection;
  //! @brief &lt;resetValue&gt;
  uint64_t         resetValue;
  //! @brief &lt;resetMask&gt;
  uint64_t         resetMask;
  //! @brief &lt;cpu&gt;
  Cpu              cpu;

  /*!
    @brief peripheral list
    Peripheral class does not prohibit copying, 
    but basically it should be treated with a reference.
    @code
    auto& p = device->peripherals[0];
    @endcode
   */
  list<Peripheral> peripherals;
  
  //----------------------------------------------------------
  /**
   * @brief find path element
   * @param p path
   * @return element pointer
   * Note: register path is NOT contains peripheral.prependToName and appendToName
   */
  template<typename STR>
  node_ptr<void> find(const ::nodamushi::svd::path<STR>& p)
  {
    if(p){
      string_ref n = p[0];
      if(auto c=__find_helper(peripherals,n)){
          if(p.size() == 1)return c;
          else             return c->find(p,1);
      }
    }
    return {};
  }
  /**
   * @brief find path element
   * @param p path
   * @return element pointer
   * Note: register path is NOT contains peripheral.prependToName and appendToName
   */
  node_ptr<void> find(string_ref p)
  {
    ::nodamushi::svd::path<> pa(p);
    return find(pa);
  }

  /**
   * @brief find peripheral
   * @param p path
   * @return element pointer
   */
  template<typename STR>
  node_ptr<Peripheral> find_peripheral(const ::nodamushi::svd::path<STR>& p)
  {
    if(p.size() == 1){
      string_ref n = p[0];
      return __find_helper(peripherals,n);
    }
    return {};
  }
  /**
   * @brief find peripheral
   * @param p path
   * @return element pointer
   */
  node_ptr<Peripheral> find_peripheral(string_ref p)
  {
    ::nodamushi::svd::path<> pa(p);
    return find_peripheral(pa);
  }

  /**
   * @brief find peripheral
   * @param p path
   * @return element pointer
   */
  template<typename STR>
  node_ptr<Cluster> find_cluster(const ::nodamushi::svd::path<STR>& p)
  {
    if(p.size() > 1){
      string_ref n = p[0];
      if(auto c=__find_helper(peripherals,n))
        return c->find_cluster(p,1);
    }
    return {};
  }
  /**
   * @brief find peripheral
   * @param p path
   * @return element pointer
   */
  node_ptr<Cluster> find_cluster(string_ref p)
  {
    ::nodamushi::svd::path<> pa(p);
    return find_cluster(pa);
  }
  
  /**
   * @brief find register
   * @param p path
   * @return element pointer
   */
  template<typename STR>
  node_ptr<Register> find_register(const ::nodamushi::svd::path<STR>& p)
  {
    if(p.size() > 1){
      string_ref n = p[0];
      if(auto c=__find_helper(peripherals,n))
        return c->find_register(p,1);
    }
    return {};
  }
  /**
   * @brief find register
   * @param p path
   * @return element pointer
   */
  node_ptr<Register> find_register(string_ref p)
  {
    ::nodamushi::svd::path<> pa(p);
    return find_register(pa);
  }
  
  /**
   * @brief find field
   * @param p path
   * @return element pointer
   */
  template<typename STR>
  node_ptr<Field> find_field(const ::nodamushi::svd::path<STR>& p)
  {
    if(p.size() > 2){
      string_ref n = p[0];
      if(auto c=__find_helper(peripherals,n))
        return c->find_field(p,1);
    }
    return {};
  }
  /**
   * @brief find field
   * @param p path
   * @return element pointer
   */
  node_ptr<Field> find_field(string_ref p)
  {
    ::nodamushi::svd::path<> pa(p);
    return find_field(pa);
  }
  
  //----------------------------------------------------------
  /**
   * @brief Don't use this constructor.this constructor is for normalizer class.
   * @param n pre normalized data
   */
  template<typename NORMALIZED>
  Device(const NORMALIZED& n)
      :vendor(n.svd.vendor.get("")),
       vendorID(n.svd.vendorID.get("")),
       name(n.svd.name.get("")),
       series(n.svd.series.get("")),
       version(n.svd.version.get("")),
       description(n.svd.description.get("")),
       licenseText(n.svd.licenseText.get("")),
       headerSystemFilename(n.svd.headerSystemFilename.get("")),
       headerDefinitionsPrefix(n.svd.headerDefinitionsPrefix.get("")),
       addressUnitBits(n.svd.addressUnitBits.get(8)), // default is 8?
       width(n.svd.width.get(32)), // default is 32?
       size(n.svd.size.get(32)),   // default is 32?
       access(n.svd.access.get(Access{})),
       protection(n.svd.protection.get(Protection{})),
       resetValue(n.svd.resetValue.get(0)),
       resetMask(n.svd.resetMask.get(~(uint64_t)0)),
       cpu(n.svd.cpu),
       peripherals(n.peripherals.size())
  {
    for(const auto& p:n.peripherals)
      peripherals.emplace_back(p);
  }
  
  /**
   * set this object pointer.
   */
  void update_parent(node_ptr<Device>& this_ptr)
  {
    if(this_ptr.get() == this){
      update_parent_of_children(peripherals, this_ptr);
    }
  }
  //! @brief allways return 0
  uint64_t get_address()const{return 0;}
  //! @brief return size
  uint32_t get_size()const{return size;}
  //! @brief return resetValue
  uint64_t get_resetValue()const{ return resetValue; }
  //! @brief return resetMask
  uint64_t get_resetMask()const{ return resetMask;}
  //! @brief return access
  Access get_access()const{ return access;}
  //! @brief return protection
  Protection get_protection()const{ return protection;}
  //! @brief allways return nullptr
  constexpr const void* get_parent()const noexcept{return nullptr;}
  //! @brief allways return nullptr
  constexpr const void* get_parent2()const noexcept{return nullptr;}
};



//---------- Visitor --------------------
__NX_NORM_HANDLE_VISIT(Device)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  // visit Peripheral
  r ret;
  ret = CONTROLLER::apply(t.cpu,v);
  if(ret==r::BREAK)return r::BREAK;
  else if(ret == r::SKIP)return r::CONTINUE;
  ret = CONTROLLER::apply(t.peripherals,v);
  if(ret==r::BREAK)return r::BREAK;
  return r::CONTINUE;
}};
//----------------------------------------

}}}

#endif // __NODAMUSHI_SVD_NORMALIZED_DEVICE_HPP__
