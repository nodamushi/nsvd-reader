/*!
  @brief normalize algorithm
  @file nodamushi/svd/normalized/normalizer.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef NODAMUSHI_SVD_NORMALIZED_NORMALIZER_HPP
#define NODAMUSHI_SVD_NORMALIZED_NORMALIZER_HPP

# include <vector>
# include <memory>
# include <type_traits>
# include <tuple>
# include <sstream>
# include <unordered_set>


# include "nodamushi/refhash.hpp"
# include "nodamushi/svd/path.hpp"
# include "nodamushi/string_type.hpp"
# include "nodamushi/boxvec.hpp"

# include "nodamushi/svd/normalized/dim_info.hpp"
# include "nodamushi/svd/normalized/dim_helper.hpp"
# include "nodamushi/svd/normalized/node_container.hpp"

# include "nodamushi/svd/normalized/Device.hpp"
# include "nodamushi/svd/normalized/Peripheral.hpp"
# include "nodamushi/svd/normalized/Cluster.hpp"
# include "nodamushi/svd/normalized/Register.hpp"
# include "nodamushi/svd/normalized/Field.hpp"
# include "nodamushi/svd/normalized/Enumeration.hpp"


namespace nodamushi {
namespace svd {
namespace normalized {


namespace details{
template<typename T1,typename T2,bool b>struct if_same_type_of;
template<typename T1,typename T2> struct if_same_type_of<T1,T2,true>
{
  static constexpr const T1 *of(const T2 *t) { return t; }
};
template<typename T1,typename T2> struct if_same_type_of<T1,T2,false>
{
  static constexpr const T1 *of(const T2 *t) { return nullptr; }
};
}

template <typename T1, typename T2>
struct if_same_type
{
  static constexpr const T1 *value(const T2 *ptr)
  {
    return details::if_same_type_of<T1,T2,std::is_same<T1, T2>::value>::of(ptr);
  }
};

template<typename STRREF,typename STR,typename V>
struct normalizer
{
  using names = std::unordered_set<::nodamushi::ref<std::string>,
                                    ::nodamushi::refhash<std::string>>;
  //! dim_info.id type
  using did_t = typename dim_info::id_t;
  //--------------------------------------------------------------------------------
  using D = ::nodamushi::svd::normalized::Device<STRREF>;
  using P = ::nodamushi::svd::normalized::Peripheral<STRREF>;
  using C = ::nodamushi::svd::normalized::Cluster<STRREF>;
  using R = ::nodamushi::svd::normalized::Register<STRREF>;
  using F = ::nodamushi::svd::normalized::Field<STRREF>;
  using E = ::nodamushi::svd::normalized::Enumeration<STRREF>;
  //--------------------------------------------------------------------------------
  using SD = const ::nodamushi::svd::Device<STR,V>;
  using SP = const ::nodamushi::svd::Peripheral<STR>;
  using SC = const ::nodamushi::svd::Cluster<STR>;
  using SR = const ::nodamushi::svd::Register<STR>;
  using SF = const ::nodamushi::svd::Field<STR>;
  using SE = const ::nodamushi::svd::Enumeration<STR>;
  //--------------------------------------------------------------------------------
  // temporary object class
  //--------------------------------------------------------------------------------
  template<typename SVD> struct base
  {
    const SVD& svd;
    std::string name;
    dim_info dim;
    bool   resolved;//! indicate that the derivedFrom property is resolved
    base(const SVD& s,const std::string& n,const dim_info& di):
        svd(s),name(n),dim(di),
        resolved(!s.derivedFrom)
    {}

    base(const SVD& s,const std::string& n):
        svd(s),name(n),dim(),
        resolved(!s.derivedFrom)
    {}

    base(const base<SVD>&) = default;
  };

  struct d;//!< device
  struct p; //!< peripheral
  struct c; //!< cluster
  struct r; //!< register
  struct f; //!< field
  struct e; //!< enumerate


  //-------------------------------------------------------------
  // create children
  //-------------------------------------------------------------
  template <typename ChildType, typename Parent, typename SVD, typename PTR, typename ERROR>
  static void emplace_child(
    Parent &p, const SVD &s, 
    boxvec<ChildType,PTR> &v,
    did_t g,
    names &n, ERROR &e)

  {
    auto dimh = make_dim_helper(s);
    auto base_info =dimh.get_base_info();
    for(size_t i = 0;i < dimh.size;i++){
      std::string ni = dimh.name(i);
      if(n.count(ni)){
        e.fail();
        ChildType::print_name(e);
        e <<  "  " << ni << " already defined in " << p;
        e.endl();
      }else{
        size_t size = v.size();
        base_info.index = i;
        base_info.id= g;
        v.emplace_back(p,s,ni,base_info,e);
        n.emplace(v[size].name);
      }
    }
  }

  template <typename ChildType, typename Parent,typename PTR, typename ERROR>
  static did_t copy_child(
    Parent &p, const ChildType &c, 
    boxvec<ChildType,PTR> &v,
    did_t g,
    names &n, ERROR &e)
  {
    if (n.count(c.name)){
      e.fail();
      ChildType::print_name(e);
      e<< "  " << c.name << " already defined in " << p;
      e.endl();
      return 0;
    }else{
      size_t s = v.size();
      v.emplace_back(c);
      auto& vs = v[s];
      n.emplace(vs.name);
      did_t r =vs.dim.id + g;
      vs.dim.id = r;
      return r;
    }
  }

  template <typename ChildType, typename Parent, typename SVDChildren, typename PTR, typename ERROR>
  static did_t emplace_children(Parent &p,const SVDChildren &vec, boxvec<ChildType,PTR> &dst, ERROR &e)
  {
    names n; // name list
    did_t g = 0;//group id
    for(const auto& svd:vec){
      emplace_child<ChildType>(p, svd, dst,g, n, e);
      g++;
    }
    return g;
  }

  //-----------------------------------------------------------------
  // Define tempolary classes
  //-----------------------------------------------------------------
  struct e:public base<SE>
  {
    template<typename T>
    static void print_name(T& o)
    {
      o << "Enumeration";
    }
    const f* parent;
    const e* derivedFrom;

    e(const f &r, const SE &s) :
        base<SE>(s, std::string(s.name?*s.name:"")),
        parent(&r), derivedFrom(nullptr) {}
    e(const e &) = default;
    e(e&&)=default;
    template <typename ERROR>
    void set_derivedFrom(const e &df,const std::vector<e*>&,ERROR& e) 
    {
      derivedFrom = &df;
      this->resolved = true;
    }
    template <typename T>
    void collect_unresolved(std::vector<T *> &v){}
    void collect_unresolved(std::vector<e *> &v)
    {
      if (!this->resolved)
        v.push_back(this);
    }
  };
  struct f:public base<SF>
  {
    template<typename T>
    static void print_name(T& o)
    {
      o << "Field";
    }

    const r* parent;
    const f* derivedFrom;
    boxvec<e> enumeratedValues;

    template <typename ERROR>
    f(const r &r, const SF &s, const std::string& n,const dim_info& i, ERROR &e) 
        :base<SF>(s, n, i),
         parent(&r), derivedFrom(nullptr),
         enumeratedValues(
             s.enumeratedValues?s.enumeratedValues.get().size():0)
    {
      if(s.enumeratedValues){
        for(const SE& v: s.enumeratedValues.get())
          enumeratedValues.emplace_back(*this,v);
      }
    }
    f(const f &) = default;
    f(f&&)=default;
    template<typename ERROR>
    void set_derivedFrom(const f& df,const std::vector<f*>&,ERROR & e){
      this->resolved = true;
      names n;
      for (const auto &c : enumeratedValues) n.emplace(c.name);

      for (const auto &c : df.enumeratedValues){
        copy_child(*this, c, enumeratedValues, 0,n, e);
      }
    }

    template <typename T>
    void collect_unresolved(std::vector<T *> &v){}

    void collect_unresolved(std::vector<f *> &v)
    {
      if (!this->resolved)
        v.push_back(this);
    }
  };
  struct r:public base<SR>
  {
    template<typename T>
    static void print_name(T& o)
    {
      o << "Register";
    }

    const p* parentp;
    const c* parentc;
    const r* derivedFrom;
    did_t last_group_id;
    boxvec<f> fields;

    template<typename P,typename ERROR>
    r(const P &r,const SR& s,const std::string& n,const dim_info& i,ERROR &e):
        base<SR>(s,n,i)
        ,parentp(if_same_type<p,P>::value(&r))
        ,parentc(if_same_type<c,P>::value(&r))
        ,derivedFrom(nullptr)
        ,last_group_id(0)
        ,fields()
    {
      if(s.fields && s.fields.get().field){
        size_t c = 0;
        const auto& v = s.fields.get().field.get();
        for(const auto& f:v)
          c += get_dim_count(f);
        if(c!=0){
          fields.reserve(c);
          last_group_id =  emplace_children(*this,v,fields,e);
        }
      }
    }
    r(const r &) = default;
    r(r&&)=default;
    template <typename ERROR>
    void set_derivedFrom(const r &df, const std::vector<r*>&, ERROR &e)
    {
      derivedFrom = &df;
      this->resolved = true;
      names n;
      for (const auto &c : fields)
        n.emplace(c.name);
      did_t ll = last_group_id;
      for (const auto &c : df.fields)
      {
        did_t l = copy_child(*this, c, fields,last_group_id, n, e);
        if(ll < l)ll = l;
      }
      last_group_id = ll;
    }
    template <typename T>
    void collect_unresolved(std::vector<T *> &v)
    {
      for (auto &c : fields)
        c.collect_unresolved(v);
    }
    void collect_unresolved(std::vector<r *> &v)
    {
      if(!this->resolved)
        v.push_back(this);
    }
  };
  struct c:public base<SC>
  {
    template<typename T>
    static void print_name(T& o)
    {
      o << "Cluster";
    }

    const p* parentp;
    const c* parentc;
    const c* derivedFrom;
    did_t last_group_id;
    boxvec<r> registers;
    boxvec<c> clusters;

    template<typename P,typename ERROR>
    c(const P& r,const SC& s,const std::string& n,const dim_info& i,ERROR &e):
        base<SC>(s,n,i),
        parentp(if_same_type<p,P>::value(&r)),parentc(if_same_type<c,P>::value(&r)),
        derivedFrom(nullptr),
        last_group_id(0),
        registers(),clusters()
    {
      size_t rc=0,cc=0;
      for(const auto& _rc : s.registers.registers){
        if(_rc.is_register())
          rc += get_dim_count(_rc.get_register());
        else
          cc += get_dim_count(_rc.get_cluster());
      }
      registers.reserve(rc);
      clusters.reserve(cc);

      names ns;
      did_t gi=0;
      for(const auto& _rc:s.registers.registers){
        if(_rc.is_register()){
          auto& ss = _rc.get_register();
          emplace_child(*this,ss,registers,gi,ns,e);
        }else{
          auto& ss = _rc.get_cluster();
          emplace_child(*this,ss,clusters,gi,ns,e);
        }
        gi++;
      }
      last_group_id = gi;
    }
    c(const c &) = default;
    c(c&&)=default;

    template <typename ERROR>
    void set_derivedFrom(const c &df,std::vector<c*>& unresolved, ERROR &e)
    {
      derivedFrom = &df;
      this->resolved = true;
      names n;
      for (const auto &c : registers)
        n.emplace(c.name);
      for (const auto &c : clusters)
        n.emplace(c.name);
      did_t ll = last_group_id;
      for (const auto &c : df.registers){
        did_t l = copy_child(*this, c, registers,last_group_id, n, e);
        if(ll < l)ll = l;
      }
      for (const auto &c : df.clusters){
        did_t l = copy_child(*this, c, clusters, last_group_id, n, e);
        if(ll < l)ll = l;
        if(!c.resolved){
          size_t i = clusters.size() - 1;
          unresolved.push_back(&clusters[i]);
        }
      }
      last_group_id = ll;
    }

    template <typename T>
    void collect_unresolved(std::vector<T *> &v)
    {
      for (auto &c : registers)
        c.collect_unresolved(v);
      for (auto &c : clusters)
        c.collect_unresolved(v);
    }
    void collect_unresolved(std::vector<c *> &v)
    {
      if(!this->resolved)
        v.push_back(this);
      for (auto &c : registers)
        c.collect_unresolved(v);
    }
  };

  struct p:public base<SP>
  {
    template<typename T>
    static void print_name(T& o)
    {
      o << "Peripheral";
    }

    const d* parent;
    const p* derivedFrom;
    did_t last_group_id;
    boxvec<r> registers;
    boxvec<c> clusters;

    template<typename ERROR>
    p(const d& r,const SP& s,const std::string& n,const dim_info& i,ERROR& e):
        base<SP>(s,n,i),
        parent(&r),
        derivedFrom(nullptr),
        last_group_id(0),
        registers(),clusters()
    {
      size_t rc=0,cc=0;
      if(s.registers){
        for(const auto& _rc : s.registers.get().registers){
          if(_rc.is_register())
            rc += get_dim_count(_rc.get_register());
          else
            cc += get_dim_count(_rc.get_cluster());
        }
        registers.reserve(rc);
        clusters.reserve(cc);
        
        names ns;
        did_t gi=0;
        for(const auto& _rc:s.registers.get().registers){
          if(_rc.is_register()){
            const auto& ss = _rc.get_register();
            emplace_child(*this,ss,registers,gi,ns,e);
          }else{
            const auto& ss = _rc.get_cluster();
            emplace_child(*this,ss,clusters,gi,ns,e);
          }
          gi++;
        }
        last_group_id = gi;
      }
    }
    p(const p&)=default;
    p(p&&)=default;

    template <typename ERROR>
    void set_derivedFrom(const p &df,std::vector<p*>&, ERROR &e)
    {
      derivedFrom = &df;
      this->resolved=true;
      names n;
      for (const auto &c : registers)
        n.emplace(c.name);
      for (const auto &c : clusters)
        n.emplace(c.name);
      did_t ll = last_group_id;
      for (const auto &c : df.registers){
        did_t l = copy_child(*this, c, registers,last_group_id, n, e);
        if(ll < l)ll = l;
      }
      for (const auto &c : df.clusters){
        did_t l =copy_child(*this, c, clusters,last_group_id, n, e);
        if(ll < l)ll = l;
      }
      last_group_id = ll;
    }
    template<typename T>
    void collect_unresolved(std::vector<T*>& v)
    {
      for (auto &c : registers)
        c.collect_unresolved(v);
      for (auto &c : clusters)
        c.collect_unresolved(v);
    }
    void collect_unresolved(std::vector<p *> &v)
    {
      if (!this->resolved)
        v.push_back(this);
    }
  };
  
  struct d
  {
    const SD& svd;
    boxvec<p> peripherals;
    template<typename ERROR>
    d(const SD& s,ERROR &e):svd(s), peripherals{}
    {
      size_t c=0;
      if(s.peripherals && s.peripherals.get().peripheral)
        for(const auto& p:s.peripherals.get().peripheral.get())
          c += get_dim_count(p);
      if(c!=0){
        peripherals.reserve(c);
        emplace_children<p>(*this,s.peripherals.get().peripheral.get(),peripherals,e);
      }
    }
    d(const d&)=delete;
    d(d&&)=default;

    template <typename ERROR>
    void set_derivedFrom(const d &,const std::vector<d*>&, ERROR &){}
  };

  //--------------------------------------------------------------------------------
  // path of node
  //--------------------------------------------------------------------------------
  static path<substring> get_path(const p& p){
    return {p.name};
  }
  static path<substring> get_path(const c& c){
    if(c.parentc)
      return get_path(*c.parentc).append(path<substring>(c.name));
    else
      return get_path(*c.parentp).append(path<substring>(c.name));
  }
  static path<substring> get_path(const r& r){
    if(r.parentc)
      return get_path(*r.parentc).add(path<substring>(r.name));
    else
      return get_path(*r.parentp).add(path<substring>(r.name));
  }
  static path<substring> get_path(const f& f){
    return get_path(*f.parent).append(path<substring>(f.name));
  }
  static path<substring> get_path(const e& e){
    return get_path(*e.parent).append(path<substring>(e.name));
  }

  //--------------------------------------------------------------------------------
  // find node
  //--------------------------------------------------------------------------------

  template<typename T1,typename T2>
  static void find(string_ref name,const T1& t,const T2** ret){}

  // helper implements
  template<typename LIST,typename T> static void _find(string_ref name,const LIST& list,const T** ret) 
  {
    for(const T& o:list)
      if(o.name == name){
        if(o.resolved)
          *ret = &o;
        return;
      }
  }
  static void find(string_ref n,const d& s,const p** r){_find(n,s.peripherals,r);}
  static void find(string_ref n,const p& s,const c** r){_find(n,s.clusters,r);}
  static void find(string_ref n,const p& s,const r** r){_find(n,s.registers,r);}
  static void find(string_ref n,const c& s,const c** r){_find(n,s.clusters,r);}
  static void find(string_ref n,const c& s,const r** r){_find(n,s.registers,r);}
  static void find(string_ref n,const r& s,const f** r){_find(n,s.fields,r);}
  static void find(string_ref n,const f& s,const e** r){_find(n,s.enumeratedValues,r);}
  
  // find name in parent of t, and set to ret
  template<typename P>
  static void findp(string_ref name,const P& t,const P** ret){
    if(t.parent)find(name,*t.parent,ret);
  }
  static void findp(string_ref name,const r& t,const r** ret){
    if(t.parentp)find(name,*t.parentp,ret);
    else         find(name,*t.parentc,ret);
  }
  static void findp(string_ref name,const c& t,const c** ret){
    if(t.parentp)find(name,*t.parentp,ret);
    else         find(name,*t.parentc,ret);
  }

  //--------------------------------------------------------------------------------
  // find derivedFrom
  //--------------------------------------------------------------------------------

  // name is single path
  template<typename PSTR,typename T>
  static const T* find_derivedFrom_s(const ::nodamushi::svd::path<PSTR>& name,const T& t)
  {
    const T *ret = nullptr;
    findp(name[0],t,&ret);
    return ret;
  }

  // PARENT is peripheral or cluster
  template<typename PSTR,typename PARENT,typename T>
  static void find_derivedFrom_r(const ::nodamushi::svd::path<PSTR>& p,
                                 const PARENT& parent,const T** ret)
  {
    const auto s = p.size();
    if(s > 3) return;

    if(s == 1){
      find(p[0],parent,ret);
      return;
    }

    const r* reg = nullptr;
    find(p[0],parent,&reg);
    if(!reg)return; // not found

    if(s == 2){
      find(p[1],reg,ret);
      return;
    }

    const f* fe = nullptr;
    find(p[1],reg,&fe);
    if(!fe)return;

    find(p[2],fe,ret); // TODO
  }

  // multi path
  template<typename PSTR,typename T>
  static void find_derivedFrom_m(const ::nodamushi::svd::path<PSTR>& path,
                                 const d& d,const T& t,const T** ret)
  {
    const auto s = path.size();
    const p* peri = nullptr;
    find(path[0],d,&peri);
    if(!peri) return; // not found
    if(s == 2){
      find(path[1],*peri,ret);
      return;
    }

    const c* clu = nullptr;
    find(path[1],*peri,&clu);
    if(!clu) find_derivedFrom_r(path.subpath(1),*peri,ret);
    else{
      size_t i = 2;
      const c* cp;
      while(!clu){
        cp = clu;
        clu = nullptr;

        if(i == s){
          find(path[i],*cp,ret);
          return;
        }

        find(path[i],*cp,&clu);
        i++;
      }
      find_derivedFrom_r(path.subpath(i),*cp,ret);
    }
  }

  //--------------------------------------------------------------------------------
  //  derivedFrom
  //--------------------------------------------------------------------------------
  template <typename T,typename ERROR>
  static bool set_derivedFrom(d &d, T &t, std::vector<T*>& unresolved, ERROR &e)
  {
    if(t.svd.derivedFrom){
      const auto& dp = *t.svd.derivedFrom;
      const T *ret  = nullptr;
      if(dp.single())ret = find_derivedFrom_s(dp,t);
      else           find_derivedFrom_m(dp,d,t,&ret);
      if(!ret)return false;
      const T& df = *ret;
      t.set_derivedFrom(df, unresolved, e);
    }
    return true;
  }
  template <typename T, typename ERROR>
  static bool set_derivedFrom_all(d &d, std::vector<T *>& unresolved, ERROR &e)
  {
    if(unresolved.empty())
      return true;
    bool found=false;
    do{
      found = false;
      auto it = unresolved.begin();
      while(it != unresolved.end()){
        T& t=**it;
        if(set_derivedFrom(d,t,unresolved,e)){
          found = true;
          it = unresolved.erase(it);
        }else
          ++it;
      }
    } while (found && unresolved.size() > 0);
    return unresolved.empty();
  }

  template <typename T,typename ERROR>
  static bool _set_derivedFrom(d &d, ERROR &err)
  {
    std::vector<T *> v;
    for (auto &c : d.peripherals){
      c.collect_unresolved(v);
    }

    if (!set_derivedFrom_all(d, v, err)){
      for (const auto *c : v){
        err.fail() 
            << (*c)
            << "  derivedFrom:" 
            << (c->svd.derivedFrom.get())
            << " is not found" 
            ;
        err.endl();
      }
      return false;
    }
    return true;
  }

  template <typename ERROR>
  static bool set_derivedFrom(d &d, ERROR &err)
  {
    bool ret = true;
    ret |= _set_derivedFrom<p>(d, err);
    ret |= _set_derivedFrom<c>(d, err);
    ret |= _set_derivedFrom<r>(d, err);
    ret |= _set_derivedFrom<f>(d, err);
    ret |= _set_derivedFrom<e>(d, err);
    return ret;
  }

  //--------------------------------------------------------------------------------
  //  Build
  //--------------------------------------------------------------------------------

  struct error
  {
    bool failed;
    std::stringstream ss;
    error():failed(false),ss(){}
    error& fail() { failed = true; return *this;}

    template<typename T>
    error& operator<< (const T& t)
    {
      ss << t;
      return *this;
    }
    error& endl(){
      ss << std::endl;
      return *this;
    }
    error& operator<< (const p &p)
    {
      ss << "peripheral: " << get_path(p);
      return *this;
    }
    error& operator<< (const c &p)
    {
      ss << "cluster: " << get_path(p);
      return *this;
    }
    error& operator<< (const f &p)
    {
      ss << "field:" << get_path(p);
      return *this;
    }
    error& operator<<(const r &p)
    {
      ss << "register:" << get_path(p);
      return *this;
    }
    error& operator<<(const e &p)
    {
      ss << "enumerate:" << get_path(p);
      return *this;
    }
    error& operator<<(const d &)
    {
      ss << "device:";
      return *this;
    }

  };

  normalizer(const SD &s) : 
      err(), 
      device(s, err), 
      ok(set_derivedFrom(device, err) && !err.failed)
  {
  }

  explicit operator bool() const { return ok; }
  std::string get_error() const { return err.ss.str(); }
  
  error err;
  d device;
  bool ok;
};

}// normalized

// // nodamushi::svd::normalize::normalize(svd)

/**
 * @brief normalize nodamushi::svd::Device.
 * @param d nodamushi::svd::Device
 * @return smart pointer of nodamushi::svd::normalize::Device<STRREF>.
 * @param STRREF normalized text type.(nodamushi::svd::normalize::Device<STRREF>)
 *  The defauto type is std::string in C++14 and std::string_view in C++17.
 * @param STR nodamushi::svd::Device<STR,V>
 * @param V nodamushi::svd::Device<STR,V>
 */
template<
# if __cplusplus >= 201703
  typename STRREF=std::string_view,
# else
  typename STRREF=std::string,
# endif
typename STR,typename V>
static ::nodamushi::svd::normalized::node_ptr<::nodamushi::svd::normalized::Device<STRREF>>
normalize(const ::nodamushi::svd::Device<STR,V>& d)
{
  namespace norm=::nodamushi::svd::normalized;
  using D = norm::Device<STRREF>;
  norm::normalizer<STRREF,STR,V> n(d);
  if(n){
    auto ptr = norm::make_node_ptr<D>(n.device);
    ptr->update_parent(ptr);
    return ptr;
  }else{
    return {};
  }
}



}}  // nodamushi::svd
#endif // NODAMUSHI_SVD_NORMALIZED_NORMALIZER_HPP
