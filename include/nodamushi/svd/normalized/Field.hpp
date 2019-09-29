/*!
  @brief normalized field element
  @file nodamushi/svd/normalized/Field.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef __NODAMUSHI_SVD_NORMALIZED_FIELD_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_FIELD_HPP__

# include "nodamushi/svd/normalized/node_container.hpp"
# include "nodamushi/svd/normalized/nullable.hpp"
# include "nodamushi/svd/normalized/bit_range.hpp"
# include "nodamushi/svd/normalized/derived_from_helper.hpp"
# include "nodamushi/svd/normalized/dim_info.hpp"

# include "nodamushi/svd/Access.hpp"
# include "nodamushi/svd/ReadAction.hpp"
# include "nodamushi/svd/ModifiedWriteValues.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{

/**
 * @brief normalized field element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_field
 * @see nodamushi::svd::Field
 */
template<typename STRREF>struct Field
{
  using this_t = Field<STRREF>;
  using Peripheral= ::nodamushi::svd::normalized::Peripheral<STRREF>;
  using Register= ::nodamushi::svd::normalized::Register<STRREF>;
  using Enumeration = ::nodamushi::svd::normalized::Enumeration<STRREF>;

  using p_ptr = parent_ptr<Register>;


private:
  p_ptr parent;
public:
  //! @brief derivedFrom Path
  path<STRREF>                  derivedFrom;
  //! @brief name of this field
  std::string                   name;
  //! @brief dimemtion information object.
  dim_info                      dim;
  //! @brief bit range
  bit_range                     bit;
  //! @brief description
  STRREF                        description;
  /**
    @brief &lt;access&gt; type.
    @see get_access()
   */
  nullable<Access>              access;
  /**
    @brief &lt;modifiedWriteValues&gt;
    @see get_modifiedWriteValues()
   */
  nullable<ModifiedWriteValues> modifiedWriteValues;
  /**
    @brief &lt;readAction&gt;
    @see get_readAction()
   */
  nullable<ReadAction>          readAction;
  /**
    @brief &lt;enumeratedValues&gt; list
    Enumeration class does not prohibit copying,
    but basically it should be treated with a reference.
    @code
    auto& e = field.enemeratedValues[0];
    @endcode
   */
  list<Enumeration>     enumeratedValues;
  //-------------------------------------------
  /**
   * @brief get lsb bit index.
   * @return bit.lsb()
   */
  unsigned int lsb()const{return bit.lsb();}
  /**
   * @brief get msb bit index.
   * @return bit.msb()
   */
  unsigned int msb()const{return bit.msb();}

  /**
    @brief get parent register pointer
    @return parent register pointer
  */
  node_ptr<Register const> get_parent()const noexcept{return parent.lock();}
  /**
    @brief get parent register pointer
    @return parent register pointer
  */
  node_ptr<Register> get_parent()noexcept{return parent.lock();}

  //! @brief find the Peripheral to which this field belongs.
  node_ptr<Peripheral const> get_peripheral()const noexcept{return find_parent_peripheral(*this);}
  //! @brief find the Peripheral to which this field belongs.
  node_ptr<Peripheral> get_peripheral()noexcept{return find_parent_peripheral(*this);}

  //! @brief resolve the value of access and return it.
  Access get_access()const{
    return access.get<this_t,get_default_fieldaccess<this_t>>(this);
  }
  //! @brief resolve the value of modifiedWriteValues and return it.
  ModifiedWriteValues get_modifiedWriteValues()const{
    return modifiedWriteValues.get<this_t,get_default_access<this_t>>(this);
  }
  //! @brief resolve the value of readAction and return it.
  ReadAction get_readAction()const{
    return readAction.get<this_t,get_default_readAction<this_t>>(this);
  }
  /**
   * @brief get reset value
   * @return reset value
   */
  uint64_t get_resetValue(){
    if(auto p = get_parent()){
      return bit.get(p->get_resetValue() & p->get_resetMask());
    }
    return 0;
  }

  //-------------------------------------------
  /**
   * @brief find path element
   * @param p path
   * @param pindex the first index of serach path. default is 0.
   * @return element pointer
   */
  template<typename STR>
  node_ptr<Enumeration> find(const ::nodamushi::svd::path<STR>& p,size_t pindex=0)
  {
    const size_t ps = p.size();
    if(ps == pindex+1){
      string_ref n = p[pindex];
      auto i = enumeratedValues.ptr_begin(),e = enumeratedValues.ptr_end();
      while(i!=e){
        auto c = *i;
        if(c->name == n)
          return c;
        ++i;
      }
    }
    return {};
  }
  //-----------------------------------------------------------------

  /**
   * @brief create new enumeration
   */
  Enumeration& new_enumeratedValues()
  {
    size_t s = enumeratedValues;
    enumeratedValues.emplace_back();
    return enumeratedValues[s];
  }

  //-----------------------------------------------------------------
  Field()=default;
  /**
   * @param T pre normalized Field
   */
  template<typename T>
  Field(const T& n): // don't change name
      parent(),
      derivedFrom(),
      name(n.name),
      dim(n.dim),
      bit(n),
      __NORMALIZED_DERIVED_FROM(description),
      __NORMALIZED_DERIVED_FROM(access),
      __NORMALIZED_DERIVED_FROM(modifiedWriteValues),
      __NORMALIZED_DERIVED_FROM(readAction)
  {
    if(n.svd.derivedFrom)
      derivedFrom = *n.svd.derivedFrom;
    for(const auto& c:n.enumeratedValues)
      enumeratedValues.emplace_back(c);
  }

  void update_parent(p_ptr& new_parent,node_ptr<this_t>& me)
  {
    parent = new_parent;
  }
  //! @brief allways return nullptr
  constexpr void* get_parent2()const noexcept{return nullptr;}


  void sort()
  {
    for(auto& e:enumeratedValues)
      e.sort();
  }

};
//---------- Visitor --------------------
__NX_NORM_HANDLE_VISIT(Field)
{
  namespace nv = ::nodamushi::visitor;
  using r = nv::result;
  r ret;
  ret =  CONTROLLER::apply(t.enumeratedValues,v);
  if(ret == r::BREAK)return ret;
  return r::CONTINUE;
}};
//--------------------------------------------


}}} // end namespace svd

#endif // __NODAMUSHI_SVD_NORMALIZED_FIELD_HPP__
