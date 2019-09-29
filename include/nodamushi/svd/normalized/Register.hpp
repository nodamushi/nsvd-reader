/*!
  @brief normalized register
  @file nodamushi/svd/normalized/Register.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_NORMALIZED_REGISTER_HPP
#define NODAMUSHI_SVD_NORMALIZED_REGISTER_HPP

# include <sstream>
# include "nodamushi/svd/normalized/node_container.hpp"
# include "nodamushi/svd/normalized/WriteConstraint.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{


/*
 * Normalized register class.
 * if STRREF is string_view or something like it,
 *  the reference source of string is the value of  nodamushi::svd::Register instance member.
 *
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_register
 */

template<typename STRREF>struct Register
{
  using Field  = ::nodamushi::svd::normalized::Field<STRREF>;
  using Cluster= ::nodamushi::svd::normalized::Cluster<STRREF>;
  using Peripheral= ::nodamushi::svd::normalized::Peripheral<STRREF>;
  using this_t = Register<STRREF>;
  using p_ptr  = parent_ptr<Peripheral>;
  using p2_ptr = parent_ptr<Cluster>;
private:
  p_ptr  parent;  //!< perihperal parent pointer
  p2_ptr parent2; //!< cluster parent pointer

public:

  //! @brief derivedFrom attribute
  path<STRREF>         derivedFrom;
  /**
   * @brief &lt;name&gt; of this register without peripheral.prependToName and peripheral.appendToName
   * @see get_name_with_appendix()
   */
  std::string          name;
  //! @brief &lt;displayName&gt;
  std::string          displayName;
  //! @brief dimemsion information object.
  dim_info             dim;
  //! @brief &lt;description&gt;
  STRREF               description;
  //! @brief &lt;alternateGroup&gt;
  STRREF               alternateGroup;
  //! @brief &lt;alternateRegister&gt;
  STRREF               alternateRegister;
  /**
    @brief &lt;addressOffset&gt;
    @par
    If you nead an absolute address, call the get_address() method.
    If you nead a relative address from the baseAddress of peripheral,call the get_address_offset() method.

    @see get_address()
    @see get_address_offset()
   */
  uint64_t             addressOffset;
  /**
    @brief &lt;size&gt;
    @see get_size()
   */
  nullable<uint32_t>   size;
  /**
    @brief &lt;access&gt;
    @see get_access()
   */
  nullable<Access>     access;
  /**
    @brief &lt;protection&gt;
    @see get_protection()
   */
  nullable<Protection> protection;
  /**
    @brief &lt;resetValue&gt;
    @see get_resetValue()
   */
  nullable<uint64_t>   resetValue;
  /**
    @brief &lt;resetMask&gt;
    @see get_resetMask()
   */
  nullable<uint64_t>   resetMask;
  /**
    @brief &lt;dataType&gt;
    @see get_dataType()
   */
  nullable<DataType>   dataType;
  //! @brief &lt;modifiedWriteValues&gt;
  ModifiedWriteValues  modifiedWriteValues;
  //! @brief &lt;readAction&gt;
  ReadAction           readAction;
  //! @brief &lt;writeConstraint&gt;
  WriteConstraint      writeConstraint;
  /*!
    @brief &lt;field&gt; elements list. This list is sorted by lsb of field.
    Field class does not prohibit copying,
    but basically it should be treated with a reference.
    @code
    auto& f = reg.fields[x];
    @endcode
   */
  list<Field>          fields;

  //-----------------------------------------------------------
  /**
   * @brief get name with peripheral.prependToName and peripheral.appendToName
   */
  std::string get_name_with_appendix()const noexcept{
    auto p = get_peripheral();
    if(p && p->has_register_name_appendix()){
      std::stringstream s;
      if(! p->prependToName.empty())
        s << p->prependToName;
      if(p->appendToName.empty())
        s << name;
      else{
        bool arr=dim.is_array() && name.length() > 3;
        if(arr){
          size_t l = name.length()-3;
          while(l != 1){
            if(name[l] == '[')break;
            l--;
          }
          if(name[l] == '['){
            s.write(name.data(),l);
          }else{
            s << name;
            arr = false;
          }
        }else
          s << name;
        s << p->appendToName;
        if(arr)
          s <<'['<<dim.index << ']';
      }
      return s.str();
    }
    return name;
  }



  /**
    @brief get parent cluster pointer
    @return parent peripheral pointer or nullptr.
    @see get_parent2()
  */
  node_ptr<Peripheral> get_parent() noexcept{return parent.lock();}
  //! @return parent peripheral pointer or nullptr.
  node_ptr<Peripheral const> get_parent()const noexcept{return parent.lock();}

  /**
    @brief get parent cluster pointer
    @return parent cluster pointer or nullptr.
    @see get_parent()
  */
  node_ptr<Cluster>    get_parent2() noexcept{return parent2.lock();}
  //! @brief parent cluster pointer or nullptr.
  node_ptr<Cluster const>    get_parent2()const noexcept{return parent2.lock();}
  //! @brief find the Peripheral to which this register belongs.
  node_ptr<Peripheral const> get_peripheral()const noexcept{return find_parent_peripheral(*this);}
  //! @brief find the Peripheral to which this register belongs.
  node_ptr<Peripheral> get_peripheral()noexcept{return find_parent_peripheral(*this);}
  //! @brief calculate an absolute address
  uint64_t get_address()const{ return calc_address(*this);}
  //! @brief  a relative address from the baseAddress of the peripheral
  uint64_t get_address_offset()const{ return calc_offset(*this);}
  //! @brief get  path
  ::nodamushi::svd::path<> get_path()const
  {
    ::nodamushi::svd::path<> p;
    path_helper(*this,p);
    return p;
  }
  /**
    @brief resolve the dataType.
    The default type is determined by the size.(size=8 : uint8_t,size=32: uint32_t)
   */
  DataType get_dataType()const{
    return dataType.get<this_t,get_default_dataType<this_t>>(this);
  }
  //! @brief  resolve the value of size and return it.
  uint32_t get_size()const{
    return size.get<this_t,get_default_size<this_t>>(this);
  }
  //! @brief  resolve the value of resetValue and return it.
  uint64_t get_resetValue()const{
    return resetValue.get<this_t,get_default_resetValue<this_t>>(this);
  }
  //! @brief  resolve the value of resetMask and return it.
  uint64_t get_resetMask()const{
    return resetMask.get<this_t,get_default_resetMask<this_t>>(this);
  }
  //! @brief  resolve the value of access and return it.
  Access get_access()const{
    return access.get<this_t,get_default_access<this_t>>(this);
  }
  //! @brief  resolve the value of protection and return it.
  Protection get_protection()const{
    return protection.get<this_t,get_default_protection<this_t>>(this);
  }
  //! @brief  resolve the value of modifiedWriteValues and return it.
  ModifiedWriteValues get_modifiedWriteValues()const{return modifiedWriteValues;}
  ReadAction get_readAction()const{return readAction;}

  //-------------------------------------------
  /**
   * @brief find path element
   * @param p path
   * @param pindex the first index of serach path. default is 0.
   * @return element pointer
   */
  template<typename STR>
  node_ptr<void> find(const ::nodamushi::svd::path<STR>& p,size_t pindex=0)
  {
    const size_t ps = p.size();
    if(ps > pindex){
      string_ref n = p[pindex];
      if(auto c = __find_helper(fields,n)){
        if(ps == pindex + 1)return c;
        else                return c->find(p,pindex+1);
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
    if(ps == pindex+1){
      string_ref n = p[pindex];
      return __find_helper(fields,n);
    }
    return {};
  }

  //-------------------------------------------


  template<typename T>
  Register(const T& n)://don't change name
      parent(),
      parent2(),
      derivedFrom(),
      name(n.name),
      displayName(make_dim_displayName_helper(n.svd)(n.dim.index)),
      dim(n.dim),
      __NORMALIZED_DERIVED_FROM(description),
      __NORMALIZED_DERIVED_FROM(alternateGroup),
      __NORMALIZED_DERIVED_FROM(alternateRegister),
      __NORMALIZED_DERIVED_FROM(addressOffset),
      __NORMALIZED_DERIVED_FROM(size),
      __NORMALIZED_DERIVED_FROM(access),
      __NORMALIZED_DERIVED_FROM(protection),
      __NORMALIZED_DERIVED_FROM(resetValue),
      __NORMALIZED_DERIVED_FROM(resetMask),
      __NORMALIZED_DERIVED_FROM(dataType),
      __NORMALIZED_DERIVED_FROM(modifiedWriteValues),
      __NORMALIZED_DERIVED_FROM(readAction),
      __NORMALIZED_DERIVED_FROM_MEMBER(writeConstraint),
      fields(n.fields.size())
  {
    if(n.svd.derivedFrom)
      derivedFrom = n.svd.derivedFrom.get();

    // add dim offset
    if(n.dim && n.dim.index!=0){
      __NORMALIZED_DERIVED_FROM_HELPER(dimIncrement);
      size_t inc = 1;
      if(dimIncrement)
        inc = *dimIncrement;
      addressOffset += inc * n.dim.index;
    }

    for(const auto& f:n.fields)
      fields.emplace_back(f);
    // sort by lsb
    fields.sort([](const Field& a,const Field& b)->bool
                {return a.lsb() < b.lsb();});

  }
  void update_parent(p_ptr& new_parent,node_ptr<this_t>& me)
  {
    parent = new_parent;
    parent2= {};
    update_parent_of_children(fields,me);
  }
  void update_parent(p2_ptr& new_parent,node_ptr<this_t>& me)
  {
    parent2= new_parent;
    parent = {};
    update_parent_of_children(fields,me);
  }
  /**
   * @brief sort field by lsb, and call Field.sort
   */
  void sort()
  {
    std::sort(fields.ptr_begin(),fields.ptr_end(),
              [](auto x,auto y){
                const auto lsbx = x->lsb();
                const auto lsby = y->lsb();
                return lsbx < lsby;
              });
    for(auto& f:fields)
      f.sort();
  }

};
//---------- Visitor --------------------
__NX_NORM_HANDLE_VISIT(Register)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  r ret;

  // visit register / cluster
  ret = CONTROLLER::apply(t.fields,v);
  if(ret == r::BREAK)return ret;
  if(ret == r::SKIP)return r::CONTINUE;
  return r::CONTINUE;
}};
//--------------------------------------------


}}} // end namespace svd

#endif // NODAMUSHI_SVD_NORMALIZED_REGISTER_HPP
