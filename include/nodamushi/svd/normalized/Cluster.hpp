/*!
  @brief normalized cluster
  @file nodamushi/svd/normalized/Cluster.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_CLUSTER_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_CLUSTER_HPP__

# include "nodamushi/svd/normalized/node_container.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{

/*
 * @brief Normalized cluster class.
 * if STRREF is string_view or something like it,
 *  the reference source of string is the value of  nodamushi::svd::Cluster instance member.
 *
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_cluster
 * @see nodamushi::svd::Cluster
 */
template<typename STRREF>struct Cluster
{
  using this_t = Cluster<STRREF>;
  using p_ptr  = parent_ptr<Peripheral<STRREF>>;
  using p2_ptr = parent_ptr<Cluster<STRREF>>;
  using Register=::nodamushi::svd::normalized::Register<STRREF>;
  using Field  = ::nodamushi::svd::normalized::Field<STRREF>;
  using Peripheral= ::nodamushi::svd::normalized::Peripheral<STRREF>;


private:
  //! peripheral parent
  p_ptr  parent;
  //! cluster parent
  p2_ptr parent2;
public:
  //! @brief derivedFrom attribute
  path<STRREF>           derivedFrom;
  //! @brief &lt;name&gt;
  std::string            name;
  //! @brief dimemsion information object.
  dim_info               dim;
  //! @brief &lt;description&gt;
  STRREF                 description;
  /*!
    @brief &lt;alternateCluster&gt;
  */
  STRREF                 alternateCluster;
  //! @brief &lt;headerStructName&gt;
  STRREF                 headerStructName;
  /*!
    @brief &lt;addressOffset&gt;.
    @par
    If you nead an absolute address, call the get_address() method.
    If you nead a relative address from the baseAddress of peripheral,call the get_offset() method.

    @note
    Although the CMSIS-SVD document describes the addressOffset as a relative address from the baseAddress of peripheral, it is probably wrong.
    In this library, the addressOffset is treated as a relative path from the parent address.
    The parent is a peripheral or a cluster.

    @see get_address()
    @see get_address_offset()
   */
  uint64_t               addressOffset;

  /*!
    @brief &lt;size&gt; default bit-width of registers.
    @see get_size()
   */
  nullable<uint32_t>     size;
  /*!
    @brief &lt;access&gt;
    @see get_access()
   */
  nullable<Access>       access;
  /*!
    @brief &lt;protection&gt;
    @see get_protection()
   */
  nullable<Protection>   protection;
  /*!
    @brief &lt;resetValue&gt;
    @see get_resetValue()
   */
  nullable<uint64_t>     resetValue;
  /*!
    @brief &lt;resetMask&gt;
    @see get_resetMask()
   */
  nullable<uint64_t>     resetMask;

  /*!
    @brief &lt;cluster&gt; elements list. This list is sorted by the addressOffset.
    @par
    Cluster class does not prohibit copying,
    but basically it should be treated with a reference.
    @code
    auto& c = parent.clusters[x];
    @endcode
   */
  list<Cluster>          clusters;
  /*!
    @brief &lt;register&gt; elements list. This list is sorted by the addressOffset.

    @par
    Register class does not prohibit copying,
    but basically it should be treated with a reference.
    @code
    auto& r = parent.registers[x];
    @endcode
   */
  list<Register>         registers;
  //-----------------------------------------------------------
  /*!
    @brief get parent peripheral pointer
    @return parent peripheral pointer or nullptr.
    @see get_parent2()
  */
  node_ptr<Peripheral> get_parent() noexcept{return parent.lock();}
  /*!
    @brief get parent cluster pointer
    @return parent cluster pointer or nullptr.
    @see get_parent()
  */
  node_ptr<Cluster>    get_parent2() noexcept{return parent2.lock();}
  /*!
    @brief get parent peripheral pointer
    @return parent peripheral pointer or nullptr.
    @see get_parent2()
  */
  node_ptr<Peripheral const> get_parent()const noexcept{return parent.lock();}

  /*!
    @brief get parent cluster pointer
    @return parent cluster pointer or nullptr.
    @see get_parent()
  */
  node_ptr<Cluster const>    get_parent2()const noexcept{return parent2.lock();}

  //! @brief get the Peripheral to which this cluster belongs.
  node_ptr<Peripheral const> get_peripheral()const noexcept{return find_parent_peripheral(*this);}
  //! @brief get the Peripheral to which this cluster belongs.
  node_ptr<Peripheral> get_peripheral()noexcept{return find_parent_peripheral(*this);}

  /**
     @brief calculate an absolute address.
     @return an absolute address
  */
  uint64_t get_address()const{ return calc_address(*this);}
  //! @brief a relative address from the baseAddress of the Peripheral to which this cluster belongs
  uint64_t get_address_offset()const{ return calc_offset(*this);}
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
  //! @brief this cluster path
  ::nodamushi::svd::path<> get_path()const
  {
    ::nodamushi::svd::path<> p;
    path_helper(*this,p);
    return p;
  }

  //-------------------------------------------
  /**
   * @brief This constructor is for normalizer.
   * @param T pre normalized Cluster
   */
  template<typename T>
  Cluster(const T& n):// 'n' don't change name
      parent(),
      parent2(),
      derivedFrom(),
      name(n.name),
      dim(n.dim),
      __NORMALIZED_DERIVED_FROM(description),
      __NORMALIZED_DERIVED_FROM(alternateCluster),
      __NORMALIZED_DERIVED_FROM(headerStructName),
      __NORMALIZED_DERIVED_FROM(addressOffset),
      __NORMALIZED_DERIVED_FROM(size),
      __NORMALIZED_DERIVED_FROM(access),
      __NORMALIZED_DERIVED_FROM(protection),
      __NORMALIZED_DERIVED_FROM(resetValue),
      __NORMALIZED_DERIVED_FROM(resetMask),
      clusters(n.clusters.size()),
      registers(n.registers.size())
  {
    if(n.svd.derivedFrom)
      derivedFrom = *n.svd.derivedFrom;
    // add dim offset
    if(n.dim && n.dim.index!=0){
      __NORMALIZED_DERIVED_FROM_HELPER(dimIncrement);
      size_t inc = 1;
      if(dimIncrement)
        inc = *dimIncrement;
      addressOffset += inc * n.dim.index;
    }

    for(const auto& c:n.clusters)
      clusters.emplace_back(c);
    for(const auto& c:n.registers)
      registers.emplace_back(c);
    // sort by address
    clusters.sort([](const Cluster& a,const Cluster& b)->bool
                  {return a.addressOffset < b.addressOffset;});
    registers.sort([](const Register& a,const Register& b)->bool
                   {return a.addressOffset < b.addressOffset;});

  }
  /**
    @brief set the peripheral parent
    @param new_parent peripheral parent pointer
    @param me this instance pointer
   */
  void update_parent(p_ptr& new_parent,node_ptr<this_t>& me)
  {
    parent = new_parent;
    parent2= {};
    update_parent_of_children(clusters,me);
    update_parent_of_children(registers,me);
  }

  /**
    @brief set the cluster parent
    @param new_parent cluster parent pointer
    @param me this instance pointer
   */
  void update_parent(p2_ptr& new_parent,node_ptr<this_t>& me)
  {
    parent2= new_parent;
    parent = {};
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
__NX_NORM_HANDLE_VISIT(Cluster)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  r ret;
  // visit register / cluster
  ret = CONTROLLER::apply(t.clusters,v);
  if(ret == r::BREAK)return r::BREAK;
  if(ret == r::SKIP)return r::CONTINUE;

  ret =  CONTROLLER::apply(t.registers,v);
  if(ret == r::BREAK)return r::BREAK;
  return r::CONTINUE;
}};
//--------------------------------------------

}}} // end namespace
#endif // __NODAMUSHI_SVD_NORMALIZED_CLUSTER_HPP__
