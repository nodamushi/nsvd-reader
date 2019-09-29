/*!
  @brief normalized peripheral element
  @file nodamushi/svd/normalized/Peripheral.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_PERIPHERAL_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_PERIPHERAL_HPP__

# include <vector>
# include <memory>
# include <string>

# include "nodamushi/svd/path.hpp"
# include "nodamushi/svd/normalized/node_container.hpp"
# include "nodamushi/svd/normalized/dim_info.hpp"
# include "nodamushi/svd/normalized/AddressBlock.hpp"
# include "nodamushi/svd/normalized/Interrupt.hpp"
# include "nodamushi/svd/normalized/nullable.hpp"

# include "nodamushi/svd/normalized/derived_from_helper.hpp"


namespace nodamushi{
namespace svd{
namespace normalized{

/**
 * @brief normalized peripheral element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_peripherals.html#elem_peripheral
 * @see nodamushi::svd::Peripheral
 */
template<typename STRREF>struct Peripheral
{
  using this_t = Peripheral<STRREF>;
  using p_ptr  = parent_ptr<Device<STRREF>>;
  using Register=::nodamushi::svd::normalized::Register<STRREF>;
  using Cluster =::nodamushi::svd::normalized::Cluster<STRREF>;
  using Interrupt=::nodamushi::svd::normalized::Interrupt<STRREF>;
  using Field=::nodamushi::svd::normalized::Field<STRREF>;

 private:
  p_ptr parent;
 public:
  //! @brief derivedFrom attribute
  path<STRREF>                derivedFrom;
  //! @brief &lt;name&gt;
  std::string                 name;
  //! @brief dimemsion information object.
  dim_info                    dim;
  //! @brief &lt;version&gt;
  STRREF                      version;
  //! @brief &lt;description&gt;
  STRREF                      description;
  //! @brief &lt;alternatePeripheral&gt;
  STRREF                      alternatePeripheral;
  //! @brief &lt;groupName&gt;
  STRREF                      groupName;
  /**
     @brief &lt;prependToName&gt;
     @note
     prependToName and appendToName are ignored in Regsiter.name and path.
     @see has_register_name_appendix()
     @see nodamushi::svd::normalized::Register.name
     @see nodamushi::svd::normalized::Register.get_name_with_appendix()
  */
  STRREF                      prependToName;
  /**
     @brief &lt;appendToName&gt;
     @note
     prependToName and appendToName are ignored in Regsiter.name and path.
     @see has_register_name_appendix()
     @see nodamushi::svd::normalized::Register.name
     @see nodamushi::svd::normalized::Register.get_name_with_appendix()
  */
  STRREF                      appendToName;
  //! @brief &lt;headerStructName&gt;
  STRREF                      headerStructName;
  //! @brief &lt;disableCondition&gt;
  STRREF                      disableCondition;
  //! @brief base address
  uint64_t                    baseAddress;
  /**
   * @brief &lt;size&gt;
   * @see get_size()
   */
  nullable<uint32_t>          size;
  /*!
    @brief &lt;access&gt;
    @see get_access()
   */
  nullable<Access>            access;
  /*!
    @brief &lt;protection&gt;
    @see get_protection()
   */
  nullable<Protection>        protection;
  /*!
    @brief &lt;resetValue&gt;
    @see get_resetValue()
   */
  nullable<uint64_t>          resetValue;
  /*!
    @brief &lt;resetMask&gt;
    @see get_resetMask()
   */
  nullable<uint64_t>          resetMask;
  //! @brief &lt;addressBlock&gt; emenents
  std::vector<AddressBlock>   addressBlock;
  /**
    @brief &lt;interrupt&gt; elements.
    @note This element does not inherit information from derivedFrom.
   */
  std::vector<Interrupt>      interrupt;

  /**
    @brief &lt;cluster&gt; elements list.This list is sorted by the addressOffset.
    @par
    Cluster class does not prohibit copying,
    but basically it should be treated with a reference.
    @code
    auto& c = parent.clusters[x];
    @endcode
   */
  list<Cluster>               clusters;
  /*!
    @brief &lt;register&gt; elements list.This list is sorted by the addressOffset.
    @par
    Register class does not prohibit copying,
    but basically it should be treated with a reference.
    @code
    auto& r = parent.registers[x];
    @endcode
   */
  list<Register>              registers;
  //-------------------------------------------
  /**
   * @brief find path element
   * @param p path
   * @param pindex the first index of serach path. default is 0.
   * @return element pointer
   */
  template<typename STR>
  node_ptr<void> find(::nodamushi::svd::path<STR>& p,size_t pindex=0)
  {
    const size_t ps = p.size();
    if(ps > pindex){
      string_ref n = p[pindex];
      if(auto c = __find_helper(clusters,n)){
        if(ps == pindex +1)return c;
        else               return c->find(p,pindex+1);
      }
      if(auto c = __find_helper(registers,n)){
        if(ps == pindex +1)return c;
        else               return c->find(p,pindex+1);
      }
    }
    return {};
  }
  /**
   * @brief find cluster element
   * @param p path
   * @param pindex the first index of serach path. default is 0.
   * @return element pointer
   */
  template<typename STR>
  node_ptr<Cluster> find_cluster(::nodamushi::svd::path<STR>& p,size_t pindex=0)
  {
    const size_t ps = p.size();
    if(ps > pindex){
      string_ref n = p[pindex];
      if(auto c = __find_helper(clusters,n)){
        if(ps == pindex +1)return c;
        else               return c->find_cluster(p,pindex+1);
      }
    }
    return {};
  }
  /**
   * @brief find register element
   * @param p path
   * @param pindex the first index of serach path. default is 0.
   * @return element pointer
   */
  template<typename STR>
  node_ptr<Register> find_register(::nodamushi::svd::path<STR>& p,size_t pindex=0)
  {
    const size_t ps = p.size();
    if(ps > pindex){
      string_ref n = p[pindex];
      if(ps > pindex + 1){
        if(auto c = __find_helper(clusters,n))
          return c->find_register(p,pindex+1);
      }else if(ps == pindex + 1){
        return __find_helper(registers,n);
      }
    }
    return {};
  }
  /**
   * @brief find field element
   * @param p path
   * @param pindex the first index of serach path. default is 0.
   * @return element pointer
   */
  template<typename STR>
  node_ptr<Field> find_field(::nodamushi::svd::path<STR>& p,size_t pindex=0)
  {
    const size_t ps = p.size();
    if(ps > pindex+1){
      string_ref n = p[pindex];
      if(ps > pindex + 2){
        if(auto c = __find_helper(clusters,n))
          return c->find_field(p,pindex+1);
      }else if(ps == pindex + 2){
        if(auto c = __find_helper(registers,n))
          return c->find_field(p,pindex+1);
      }
    }
    return {};
  }

  //-------------------------------------------

  /*!
   * @brief  prependToName or appendToName is not empty
   */
  bool has_register_name_appendix()const noexcept
  {
    return !prependToName.empty() || !appendToName.empty();
  }
  //! @brief get parent Device pointer
  auto get_parent()const noexcept{return parent.lock();}
  //! @brief get parent Device pointer
  auto get_parent()noexcept{return parent.logk();}
  //! @brief get baseAddress
  uint64_t get_address()const{return baseAddress;}
  //! @brief resolve the value of size and return it.
  uint32_t get_size()const{
    return size.get<this_t,get_default_size<this_t>>(this);
  }
  //! @brief resolve the value of resetValue and return it.
  uint64_t get_resetValue()const{
    return resetValue.get<this_t,get_default_resetValue<this_t>>(this);
  }
  //! @brief resolve the value of resetMask and return it.
  uint64_t get_resetMask()const{
    return resetMask.get<this_t,get_default_resetMask<this_t>>(this);
  }
  //! @brief resolve the value of access and return it.
  Access get_access()const{
    return access.get<this_t,get_default_access<this_t>>(this);
  }
  //! @brief resolve the value of protection and return it.
  Protection get_protection()const{
    return protection.get<this_t,get_default_protection<this_t>>(this);
  }
  //! @brief get path
  ::nodamushi::svd::path<> get_path()const
  {
    ::nodamushi::svd::path<> p;
    p.add_name(name);
    return p;
  }


  template<typename T>
  Peripheral(const T& n): // don't change name
      parent(),
      derivedFrom(),
      name(n.name),
      dim(n.dim),
      __NORMALIZED_DERIVED_FROM(version), // see derived_from_helper.hpp
      __NORMALIZED_DERIVED_FROM(description),
      __NORMALIZED_DERIVED_FROM(alternatePeripheral),
      __NORMALIZED_DERIVED_FROM(groupName),
      __NORMALIZED_DERIVED_FROM(prependToName),
      __NORMALIZED_DERIVED_FROM(appendToName),
      __NORMALIZED_DERIVED_FROM(headerStructName),
      __NORMALIZED_DERIVED_FROM(disableCondition),
      __NORMALIZED_DERIVED_FROM(baseAddress),
      __NORMALIZED_DERIVED_FROM(size),
      __NORMALIZED_DERIVED_FROM(access),
      __NORMALIZED_DERIVED_FROM(protection),
      __NORMALIZED_DERIVED_FROM(resetValue),
      __NORMALIZED_DERIVED_FROM(resetMask),
      addressBlock{},
      interrupt{},
      clusters(n.clusters.size()),
      registers(n.registers.size())
  {
    if(n.svd.derivedFrom)
      derivedFrom = *n.svd.derivedFrom;

    // add dim offset
    if(n.dim&& n.dim.index!=0){
      __NORMALIZED_DERIVED_FROM_HELPER(dimIncrement);
      size_t inc = 1;
      if(dimIncrement)
        inc = *dimIncrement;
      baseAddress += inc * n.dim.index;
    }

    //----------------------------------------
    // addressBlock
    //----------------------------------------
    size_t addrBlockSize = 0;

    if(n.svd.addressBlock)
      addrBlockSize = n.svd.addressBlock.get().size();

    if(n.derivedFrom){
      const auto* ptr = n.derivedFrom;
      while(ptr){
        const auto& v = ptr->svd.addressBlock;
        if(v) addrBlockSize += v.get().size();
        ptr = ptr->derivedFrom;
      }
    }

    addressBlock.reserve(addrBlockSize);

    if(n.svd.addressBlock){
      for(const auto& s:*n.svd.addressBlock)
        addressBlock.emplace_back(s);
    }

    if(n.derivedFrom){
      const auto* ptr = n.derivedFrom;
      while(ptr){
        if(ptr->svd.addressBlock){
          for(const auto& s:ptr->svd.addressBlock.get())
            addressBlock.emplace_back(s);
        }
        ptr = ptr->derivedFrom;
      }
    }

    //----------------------------------------
    // interrupt
    //   derivedFrom property is ignored
    //----------------------------------------
    if(n.svd.interrupt){
      auto& d = interrupt;
      const auto& v = *n.svd.interrupt;
      d.reserve(v.size());
      for(const auto& s:v)
        d.emplace_back(s);
    }
    //----------------------------------------
    // children
    for(const auto& c:n.clusters){
      clusters.emplace_back(c);
    }
    for(const auto& c:n.registers)
      registers.emplace_back(c);
    // sort by address
    clusters.sort([](const Cluster& a,const Cluster& b)->bool
                  {return a.addressOffset < b.addressOffset;});
    registers.sort([](const Register& a,const Register& b)->bool
                   {return a.addressOffset < b.addressOffset;});
  }

  //! @brief always return nullptr
  constexpr void* get_parent2()const noexcept{return nullptr;}

  void update_parent(p_ptr& new_parent,node_ptr<this_t>& me)
  {
    parent = new_parent;
    update_parent_of_children(clusters,me);
    update_parent_of_children(registers,me);
  }

  void sort()
  {
    std::sort(registers.ptr_begin(),registers.ptr_end(),
              [](auto x,auto y){
                const auto xa = x->addressOffset;
                const auto ya = y->addressOffset;
                return xa < ya;
              });

    std::sort(clusters.ptr_begin(),clusters.ptr_end(),
              [](auto x,auto y){
                const auto xa = x->addressOffset;
                const auto ya = y->addressOffset;
                return xa < ya;
              });
    for(auto& r:registers)
      r.sort();
    for(auto& c:clusters)
      c.sort();
  }

};


//---------- Visitor --------------------
__NX_NORM_HANDLE_VISIT(Peripheral)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  r ret;
  ret = CONTROLLER::apply(t.interrupt,v);
  if(ret == r::BREAK)return ret;
  if(ret == r::SKIP)return r::CONTINUE;

  ret = CONTROLLER::apply(t.addressBlock,v);
  if(ret == r::BREAK)return ret;
  if(ret == r::SKIP)return r::CONTINUE;

  // visit register / cluster
  ret = CONTROLLER::apply(t.clusters,v);
  if(ret == r::BREAK)return ret;
  if(ret == r::SKIP)return r::CONTINUE;

  ret =  CONTROLLER::apply(t.registers,v);
  if(ret == r::BREAK)return ret;
  return r::CONTINUE;
}};
//--------------------------------------------


}}} // end namespace svd

#endif // __NODAMUSHI_SVD_NORMALIZED_PERIPHERAL_HPP__
