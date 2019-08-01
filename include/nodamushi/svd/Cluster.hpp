/*!
  @brief &lt;cluster&gt; element
  @file nodamushi/svd/Cluster.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_CLUSTER_HPP__
#define __NODAMUSHI_SVD_CLUSTER_HPP__

# include <ostream>
# include <vector>
# include <memory>
# include "nodamushi/svd/node.hpp"

# include "nodamushi/svd/reg_or_cluster.hpp"

# include "nodamushi/svd/create.hpp"
# include "nodamushi/const_string.hpp"
# include "nodamushi/svd/svd_printer.hpp"

# include "nodamushi/svd/Register.hpp"
# include "nodamushi/svd/ReadAction.hpp"
# include "nodamushi/svd/dim_type.hpp"
# include "nodamushi/svd/DimArrayIndex.hpp"
# include "nodamushi/svd/value.hpp"
# include "nodamushi/svd/Field.hpp"
# include "nodamushi/svd/WriteConstraint.hpp"
# include "nodamushi/svd/Protection.hpp"
# include "nodamushi/svd/svdvisitor.hpp"
# include "nodamushi/svd/path.hpp"
namespace nodamushi{
namespace svd{

template<typename STR>struct Cluster;

/**
 * @brief &lt;register&gt; or &lt;cluster&gt; list. iterable.
 *  Holds register and cluster in the order they appear in the SVD file.
 * @see nodamushi::svd::Peripheral
 * @see nodamushi::svd::Cluster
 */
template<typename STR>struct Registers:public svd_node
{
  using rc_t = ::nodamushi::svd::reg_or_cluster<Register<STR>,Cluster<STR>>;
  using reg_t = typename rc_t::reg_t;
  using clu_t = typename rc_t::clu_t;
  //! return count of &lt;register&gt; or &lt;cluster&gt;
  size_t length()const{return registers.length();}
  //! return count of &lt;register&gt; or &lt;cluster&gt;
  size_t size()const{return registers.size();}
  //! get &lt;register&gt; or &lt;cluster&gt;
  rc_t& operator[](size_t i){return registers[i];}
  //! get &lt;register&gt; or &lt;cluster&gt;
  const rc_t& operator[](size_t i)const {return registers[i];}
  
  Registers()=default;
  Registers(Registers<STR>&&)=default;

  // deap-copy
  Registers(const Registers<STR>& r):registers{}
  {
    registers.reserve(r.registers.size());
    for(const auto& rc:r.registers){
      if(rc.is_register())
        registers.emplace_back(new reg_t(rc.get_register()));
      else
        registers.emplace_back(new clu_t(rc.get_cluster()));
    }
  }
  using iterator = typename std::vector<rc_t>::iterator;
  using const_iterator = typename std::vector<rc_t>::const_iterator;

  iterator begin(){return registers.begin();}
  iterator end(){return registers.end();}
  const_iterator begin()const{return registers.begin();}
  const_iterator end()const{return registers.end();}
  //! &lt;register&gt; or &lt;cluster&gt; list
  std::vector<rc_t> registers;
};

/**
 *  @brief &lt;cluster&gt; element
 *  @see nodamushi::svd::Peripheral
 *  @see nodamushi::svd::Register
 *  @see nodamushi::svd::normalized::Cluster
 */
template<typename STR>struct Cluster:public svd_node
{
  static constexpr bool IS_REGISTER=false;// to  distinguish between Register and Cluster.
  static constexpr bool IS_CLUSTER =true; // to  distinguish between Register and Cluster.
  using rc_t = ::nodamushi::svd::reg_or_cluster<Register<STR>,Cluster<STR>>;
  using reg_t = typename rc_t::reg_t;
  using clu_t = typename rc_t::clu_t;

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
  //! @brief  &lt;description&gt; this element is required
  SVD_VALUE(STR,description,true);
  //! @brief  &lt;alternateCluster&gt;
  SVD_VALUE(STR,alternateCluster);
  //! @brief  &lt;headerStructName&gt;
  SVD_VALUE(STR,headerStructName);

  //! @brief  &lt;addressOffset&gt;. uint64_t.this element is required
  SVD_VALUE(hex64,addressOffset,true);
  //! @brief  &lt;size&gt;. uint32_t.this element is required
  SVD_VALUE(hex32,size);
  //! @brief  &lt;access&gt;
  SVD_VALUE(Access, access);
  //! @brief  &lt;protection&gt;
  SVD_VALUE(Protection,protection);
  //! @brief  &lt;resteValue&gt;
  SVD_VALUE(hex64,resetValue);
  //! @brief  &lt;resetMask&gt;
  SVD_VALUE(hex64,resetMask);
  
  /**
   * @brief &lt;register&gt; or &lt;cluster&gt;
   * @note cluster does not have a &lt;registers&gt; element.
   */
  Registers<STR> registers;

  //! @return count of &lt;register&gt; or &lt;cluster&gt;
  size_t length()const{return registers.length();}
  /**
   * short cut method. 
   @code
   cluster.registers[x] == cluster[x]
   @endcode
   * @return &lt;register&gt; or &lt;cluster&gt;
   */
  rc_t& operator[](size_t i){return registers[i];}

  /**
   * short cut method. 
   @code
   cluster.registers[x] == cluster[x]
   @endcode
   * @return &lt;register&gt; or &lt;cluster&gt;
   */
  const rc_t& operator[](size_t i)const {return registers[i];}
};
//----------------------------------------------
namespace details{
template<typename SVD,typename STR>
bool createRegisterOrCluster(SVD& c,Registers<STR>& d)
{
  using reg_t = typename Registers<STR>::reg_t;
  using clu_t = typename Registers<STR>::clu_t;

  string_ref n = c.get_name();
  if(n == "register"){
    reg_t *ptr = new reg_t();
    reg_t& rg = *ptr;
    d.registers.emplace_back(ptr);
    create(c,rg);
  }else if(n == "cluster"){
    clu_t *ptr = new clu_t();
    clu_t& cl = *ptr;
    d.registers.emplace_back(ptr);
    create(c,cl);
  }
  return true;
}
}

template<typename SVD,typename STR>
bool create(SVD& svd,Registers<STR>& d)
{  
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    details::createRegisterOrCluster(c, d);
  }
  return true;
}

template<typename STR>
struct svd_printer<Registers<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Registers<STR>& v,
      int indent)
  {
    using reg_t = typename Registers<STR>::reg_t;
    using clu_t = typename Registers<STR>::clu_t;

    int i= indent;
    if(!is_empty(name)){
      o << get_indent(indent)<<"<" << name << ">"<<std::endl;
      i++;
    }
    for(const auto& rc:v.registers){
      if(rc.is_register()){
        const reg_t& r = rc.get_register();
        svd_printer<reg_t>::print(o,"register",r,i);
      }else if(rc.is_cluster()){
        const clu_t& c = rc.get_cluster();
        svd_printer<clu_t>::print(o,"cluster",c,i);
      }
      o << std::endl;
    }
    if(!is_empty(name)){
      o << get_indent(indent)<<"</" << name << ">";
    }
    return o;
  }
};




template<typename SVD,typename STR>
bool create(SVD& svd,Cluster<STR>& d)
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
    else if(n == "description") t = (d.description = c);
    else if(n == "alternateCluster") t = (d.alternateCluster = c);
    else if(n == "headerStructName") t = (d.headerStructName = c);
    else if(n == "addressOffset") t = (d.addressOffset = c);
    else if(n == "size") t = (d.size = c);
    else if(n == "access")t = (d.access = c);
    else if(n == "protection") t = (d.protection = c);
    else if(n == "resetValue") t = (d.resetValue = c);
    else if(n == "resetMask") t = (d.resetMask = c);
    else if(n == "register" || n == "cluster"){
      details::createRegisterOrCluster(c, d.registers);
    }
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
struct svd_printer<Cluster<STR>>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const Cluster<STR>& v,
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
    print_svd_value(o,v.description,i,true);
    print_svd_value(o,v.alternateCluster,i,true);
    print_svd_value(o,v.headerStructName,i,true);
    
    print_svd_value(o,v.addressOffset,i,true);
    print_svd_value(o,v.size,i,true);
    print_svd_value(o,v.access,i,true);
    print_svd_value(o,v.protection,i,true);
    print_svd_value(o,v.resetValue,i,true);
    print_svd_value(o,v.resetMask,i,true);

    svd_printer<Registers<STR>>::print(o,"",v.registers,i);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};


//---------- SVD Visitor --------------------
namespace reg_or_cluster_visit{
template<typename CONTROLLER,typename RC,typename VISITOR>
::nodamushi::visitor::result visit(RC& rcvec,VISITOR& v)
{
  using r = ::nodamushi::visitor::result;
  for(auto& rc : rcvec.registers){
    r r;
    if(rc.is_register())
      r = CONTROLLER::apply(rc.get_register(),v);
    else
      r = CONTROLLER::apply(rc.get_cluster(),v);
    if(r != r::CONTINUE) return r;
  }
  return r::CONTINUE;
}}//end reg_or_cluster_visit

__NX_SVD_HANDLE_VISIT(Cluster)
{
  using r = ::nodamushi::visitor::result;
  r ret =  reg_or_cluster_visit::visit<CONTROLLER>(t.registers,v);
  if(ret == r::BREAK)return r::BREAK;
  return r::CONTINUE;
}};

} // end namespace svd
}// end namespace nodamushi
#endif // __NODAMUSHI_SVD_CLUSTER_HPP__
