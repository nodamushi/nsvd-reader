/**
 * @brief Register / Cluster
 * @file nodamushi/svd/reg_or_cluster.hpp
 */
/**
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_REGISTERORCLUSTER_HPP__
#define __NODAMUSHI_REGISTERORCLUSTER_HPP__

# include <memory>
# include <functional>
# include <exception>

namespace nodamushi{
namespace svd{

/**
 * @brief unique_ptr&lt;Register&gt; or unique_ptr&lt;Cluster&gt;
 */
template<typename REG,typename CLUSTER>struct reg_or_cluster
{
  using reg_t = REG;
  using clu_t = CLUSTER;
  using deleter = std::function<void(void*)>;

  reg_or_cluster(const reg_or_cluster&)=delete;
  reg_or_cluster& operator=(const reg_or_cluster&) = delete;
  // move only
  reg_or_cluster(reg_or_cluster&&)=default;
  reg_or_cluster& operator=(reg_or_cluster&&)=default;

  reg_or_cluster(reg_t* r):
      reg(true),p(r,[](void *v){delete static_cast<reg_t*>(v);}){}
  reg_or_cluster(clu_t* c):
      reg(false),p(c,[](void *v){delete static_cast<clu_t*>(v);}){}
  
  /**
   * @return is register pointer
   */
  bool is_register()const noexcept{return reg;}
  /**
   * @return is cluster pointer
   */
  bool is_cluster()const noexcept{return !reg;}
  /**
   * get as register
   * @throws runtime_error
   */
  reg_t& get_register(){
    if(!reg){
      throw std::runtime_error("bad cast.not a register");
    }
    void* d = p.get();
    if(!d){
      throw std::runtime_error("register is empty");
    }
    reg_t* r = static_cast<reg_t*>(d);
    return *r;
  }
  /**
   * get as cluster
   * @throws runtime_error
   */
  clu_t& get_cluster(){
    if(reg){
      throw std::runtime_error("bad cast.not a cluster");
    }
    void* d = p.get();
    if(!d){
      throw std::runtime_error("cluster is empty");
    }
    clu_t* r = static_cast<clu_t*>(d);
    return *r;
  }
  /**
   * get as register
   * @throws runtime_error
   */
  const reg_t& get_register()const{
    if(!reg){
      throw std::runtime_error("bad cast.not a register");
    }
    void* d = p.get();
    if(!d){
      throw std::runtime_error("register is empty");
    }
    reg_t* r = static_cast<reg_t*>(d);
    return *r;
  }
  /**
   * get as cluster
   * @throws runtime_error
   */
  const clu_t& get_cluster()const{
    if(reg){
      throw std::runtime_error("bad cast.not a cluster");
    }
    void* d = p.get();
    if(!d){
      throw std::runtime_error("cluster is empty");
    }
    clu_t* r = static_cast<clu_t*>(d);
    return *r;
  }

  const void* get(){return p.get();}
 private:
  bool reg;
  std::unique_ptr<void, deleter> p;
};


template<bool IS_REGISTER>struct register_cluster_selector;

template<>struct register_cluster_selector<true>
{
  static constexpr bool is_register = true;
  template<typename R,typename C>
  static constexpr const R& select(const R& r,const C& c){return r;}
  template<typename R,typename C>
  static constexpr R& select(R& r,C& c){return r;}
  template<typename R,typename C>
  static const R* get(const reg_or_cluster<R,C>& rc){
    if(rc.is_register()){return &rc.get_register();}
    return nullptr;
  }
  template<typename R,typename C>
  static R* get(reg_or_cluster<R,C>& rc){
    if(rc.is_register()){return &rc.get_register();}
    return nullptr;
  }

};

template<>struct register_cluster_selector<false>
{
  static constexpr bool is_register = false;
  template<typename R,typename C>
  static constexpr const C& select(const R& r,const C& c){return c;}
  template<typename R,typename C>
  static constexpr C& select(R& r,C& c){return c;}

  template<typename R,typename C>
  static const C* get(const reg_or_cluster<R,C>& rc){
    if(rc.is_cluster()){return &rc.get_cluster();}
    return nullptr;
  }
  template<typename R,typename C>
  static C* get(reg_or_cluster<R,C>& rc){
    if(rc.is_cluster()){return &rc.get_cluster();}
    return nullptr;
  }

};


}} // end namespace nodamushi

#endif // __NODAMUSHI_REGISTERORCLUSTER_HPP__
