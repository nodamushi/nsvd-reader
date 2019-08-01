/*!
  @brief container class decralation
  @file nodamushi/svd/normalized/node_container.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_NODE_CONTAINER_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_NODE_CONTAINER_HPP__

# include <vector>
# include <memory>
# include <string>


# include "nodamushi/svd/normalized/nullable.hpp"
# include "nodamushi/svd/normalized/dim_info.hpp"
# include "nodamushi/svd/normalized/dim_helper.hpp"

# include "nodamushi/svd/Access.hpp"
# include "nodamushi/svd/ReadAction.hpp"
# include "nodamushi/svd/Protection.hpp"
# include "nodamushi/svd/DataType.hpp"
# include "nodamushi/svd/ModifiedWriteValues.hpp"

# include "nodamushi/boxvec.hpp"
# include "nodamushi/svd/normalized/normalized_visitor.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{

// ----------------------------------------------------------------------
// * interface
//  get_address()   :  return absolute address
//  get_offset()    :  return relative address in the peripheral
//  get_protection():  return protection
//  get_access()    :  return get access
//  get_resetValue():  return reset value
//  get_resetMask() :  return reset mask.default value is ~0
//  get_parent()    :  return parent node pointer.
//                        if it has no parent , return void*.
//  get_parent2()   :  return cluster parent.
//                        if it has no cluster parent, return void*.
// ----------------------------------------------------------------------

template<typename STRREF> struct Device;
template<typename STRREF> struct Peripheral;
template<typename STRREF> struct Cluster;
template<typename STRREF> struct Register;
template<typename STRREF> struct Field;
template<typename STRREF> struct Enumeration;
template<typename STRREF> struct Cpu;
template<typename STRREF> struct SAURegionsConfig;

template<typename T>      using node_ptr  = std::shared_ptr<T>;
template<typename Parent> using parent_ptr= std::weak_ptr<Parent>;
template<typename Child>  using list      = ::nodamushi::boxvec<Child,node_ptr<Child>>;

template<typename T,typename... ARGS>
node_ptr<T> make_node_ptr(ARGS&&... args)
{
  return std::make_shared<T>(std::forward<ARGS>(args)...);
}


template<typename PTR>
struct parent_method_call
{
  template<typename T>
  static uint32_t get_size(T p){return p->get_size();}
  template<typename T>
  static Protection get_protection(T p){return p->get_protection();}
  template<typename T>
  static Access get_access(T p){return p->get_access();}
  template<typename T>
  static uint64_t get_resetValue(T p){return p->get_resetValue();}
  template<typename T>
  static uint64_t get_resetMask(T p){return p->get_resetMask();}
  template<typename T>
  static ModifiedWriteValues get_modifiedWriteValues(T p){return p->get_modifiedWriteValues();}
  template<typename T>
  static ReadAction get_readAction(T p){return p->get_readAction();}
};


//----------------------- helper methods ------------------------------
template<typename T,typename You>
void update_parent_of_children(list<T>& l,node_ptr<You>& you)
{
  parent_ptr<You> _you(you);
  auto itr = l.ptr_begin();
  auto end = l.ptr_end();
  while(itr != end){
    auto& ptr = *itr;
    ptr->update_parent(_you,ptr);
    ++itr;
  }
}
template<typename O,typename S>
void path_helper(const O& o,::nodamushi::svd::path<S>& path)
{
  if(auto p = o.get_parent()){
    path_helper(*p,path);
  }else if(auto p=o.get_parent2()){
    path_helper(*p,path);
  }
  path.add_name(o.name);
}
template<typename O,typename S>
void path_helper(const Peripheral<O>& o,::nodamushi::svd::path<S>& path)
{
  path.add_name(o.name);
}


template<typename STRREF,template<class>class O>
node_ptr<Peripheral<STRREF>> find_parent_peripheral(O<STRREF>& o){
  if(auto p = o.get_parent()){
    return find_parent_peripheral(p);
  }else if(auto p=o.get_parent2()){
    return find_parent_peripheral(p);
  }
  return {};
}
template<typename STRREF,template<class>class O>
node_ptr<const Peripheral<STRREF>> find_parent_peripheral(const O<STRREF>& o){
  if(auto p = o.get_parent()){
    return find_parent_peripheral(p);
  }else if(auto p=o.get_parent2()){
    return find_parent_peripheral(p);
  }
  return {};
}

template<typename STRREF,template<class>class O>
node_ptr<Peripheral<STRREF>> find_parent_peripheral(node_ptr<O<STRREF>>& o){
  if(auto p = o->get_parent()){
    return find_parent_peripheral(p);
  }else if(auto p=o->get_parent2()){
    return find_parent_peripheral(p);
  }
  return {};
}

template<typename STRREF,template<class>class O>
node_ptr<const Peripheral<STRREF>> find_parent_peripheral(node_ptr<const O<STRREF>>& o){
  if(auto p = o->get_parent()){
    return find_parent_peripheral(p);
  }else if(auto p=o->get_parent2()){
    return find_parent_peripheral(p);
  }
  return {};
}

template<typename STRREF>
node_ptr<Peripheral<STRREF>> find_parent_peripheral(node_ptr<Peripheral<STRREF>>& o){return o;}
template<typename STRREF>
node_ptr<Peripheral<STRREF>> find_parent_peripheral(void* o){return {};}
template<typename STRREF>
node_ptr<const Peripheral<STRREF>> find_parent_peripheral(node_ptr<const Peripheral<STRREF>>& o){return o;}
template<typename STRREF>
node_ptr<const Peripheral<STRREF>> find_parent_peripheral(const void* o){return {};}

//--------------- Address -------------------------------------------
template<typename O>
uint64_t calc_address(const O& o,uint64_t offset = 0)
{
  offset += o.addressOffset;
  if(auto ptr = o.get_parent())
    return calc_address(*ptr,offset);
  else if(auto ptr = o.get_parent2())
    return calc_address(*ptr,offset);
  return offset;
}
template<typename X>
uint64_t constexpr calc_address(const Device<X>& d,uint64_t offset=0){return offset;}
template<typename X>
uint64_t constexpr calc_address(const Peripheral<X>& p,uint64_t offset=0){return p.baseAddress+offset;}

template<typename O>
uint64_t calc_offset(const O& o,uint64_t offset = 0)
{
  offset += o.addressOffset;
  if(auto ptr = o.get_parent())
    return calc_offset(*ptr,offset);
  else if(auto ptr = o.get_parent2())
    return calc_offset(*ptr,offset);
  return offset;
}
template<typename X>
uint64_t constexpr calc_offset(const Device<X>& d,uint64_t offset=0){return offset;}
template<typename X>
uint64_t constexpr calc_offset(const Peripheral<X>& p,uint64_t offset=0){return offset;}

//------------------------------------------------------------------

template<typename O>uint32_t get_default_size(const O& o)noexcept
{
  if(auto ptr = o.get_parent()){
    return parent_method_call<decltype(ptr)>::get_size(ptr);
  }else if(auto ptr = o.get_parent2()){
    return parent_method_call<decltype(ptr)>::get_size(ptr);
  }
  return 32;
}

template<typename O>Access get_default_access(const O& o)noexcept
{
  if(auto ptr = o.get_parent()){
    return parent_method_call<decltype(ptr)>::get_access(ptr);
  }else if(auto ptr = o.get_parent2()){
    return parent_method_call<decltype(ptr)>::get_access(ptr);
  }
  return static_cast<Access>(0);
}

template<typename O>Access get_default_fieldaccess(const O& o)noexcept
{
  if(auto ptr = o.get_parent()){
    return parent_method_call<decltype(ptr)>::get_access(ptr);
  }
  return static_cast<Access>(0);
}

template<typename O>Protection get_default_protection(const O& o)noexcept
{
  if(auto ptr = o.get_parent()){
    return parent_method_call<decltype(ptr)>::get_protection(ptr);
  }else if(auto ptr = o.get_parent2()){
    return parent_method_call<decltype(ptr)>::get_protection(ptr);
  }
  return static_cast<Protection>(0);
}

template<typename O>uint64_t get_default_resetValue(const O& o)noexcept
{
  if(auto ptr = o.get_parent()){
    return parent_method_call<decltype(ptr)>::get_resetValue(ptr);
  }else if(auto ptr = o.get_parent2()){
    return parent_method_call<decltype(ptr)>::get_resetValue(ptr);
  }
  return 0;
}

template<typename O>uint64_t get_default_resetMask(const O& o)
{
  if(auto ptr = o.get_parent()){
    return parent_method_call<decltype(ptr)>::get_resetMask(ptr);
  }else if(auto ptr = o.get_parent2()){
    return parent_method_call<decltype(ptr)>::get_resetMask(ptr);
  }
  //TODO error?
  return ~(uint64_t)0;
}

template<typename O> ModifiedWriteValues get_default_modifiedWriteValues(const O& o){
  if(auto ptr = o.get_parent()){
    return parent_method_call<decltype(ptr)>::get_modifiedWriteValues(ptr);
  }
  return static_cast<ModifiedWriteValues>(0);
}

template<typename O> ReadAction get_default_readAction(const O& o){
  if(auto ptr = o.get_parent()){
    return parent_method_call<decltype(ptr)>::get_readAction(ptr);
  }
  return static_cast<ReadAction>(0);
}



template<typename O>
DataType get_default_dataType(const O& r)noexcept
{
  auto s = r.get_size();
  if(s <= 8)return DataType::UINT8;  //0 - 8
  else if(s <= 16)return DataType::UINT16;//9 -16
  else if(s <= 32)return DataType::UINT32;//17 - 32
  else if(s <= 64)return DataType::UINT64;//33 - 64
  return DataType::UNDEFINED;
}

template<typename R>
node_ptr<R> __find_helper(list<R>& v,string_ref n)
{
  if(v.size()!=0){
    auto i= v.ptr_begin(),e =v.ptr_end();
    while(i!=e){
      auto c = *i;
      if(c->name == n)
        return c;
      ++i;
    }
  }
  return {};
}

// for void*
template<>struct parent_method_call<void*>
{
  static constexpr uint64_t get_address(const void*){return 0;}
  static constexpr uint32_t get_size(const void*){return 0;}
  static constexpr Protection get_protection(const void*){return static_cast<Protection>(0);}
  static constexpr Access get_access(const void*){return static_cast<Access>(0);}
  static constexpr uint64_t get_resetValue(const void*){return 0;}
  static constexpr uint64_t get_resetMask(const void*){return ~0;}
  static constexpr ModifiedWriteValues get_modifiedWriteValues(const void*){return static_cast<ModifiedWriteValues>(0);}
  static constexpr ReadAction get_readAction(const void*){return static_cast<ReadAction>(0);}
};



template<typename STRREF,typename STR,typename V>
struct normalizer;
}}}

#endif // __NODAMUSHI_SVD_NORMALIZED_NODE_CONTAINER_HPP__
