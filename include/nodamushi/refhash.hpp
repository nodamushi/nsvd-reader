/*!
  @brief reference hash (pointer hasn)
  @file nodamushi/refhash.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef __NODAMUSHI_REFHASH_HPP__
#define __NODAMUSHI_REFHASH_HPP__
# include <functional>

namespace nodamushi{


/**
  @brief raw pointer container.(not managed)
 */
template<typename T>struct ref
{
  const T* t;
  ref():t(nullptr){};
  ref(const T& v):t(&v){};
  ref(const ref<T>& r)=default;
  ref(ref<T>&& r)=default;

  bool operator==(const ref<T>& r)const
  {
    if(t == r.t)return true;
    if(!t || !r.t)return false;
    return *t == *r.t;
  }
  bool operator==(const T& r)const
  {
    if(t == &r)return true;
    if(!t)return false;
    return *t == r;
  }
  bool operator==(T&& r)const
  {
    if(t == &r)return true;
    if(!t)return false;
    return *t == r;
  }
  bool operator==(const T* r)const
  {
    if(t == r)return true;
    if(!t || !r)return false;
    return *t == *r;
  }
  explicit operator bool()const{return t;}
  const T& operator*()const{return *t;}
};

template<typename T>
ref<T> ref_of(const T& t){return {t};}


/** 
  @brief   hash of ref<T>
 */
template<typename T>struct refhash
{
  size_t operator()(const ref<T>& key) const{
    return key?std::hash<T>()(*key)+1:0;
  }
  size_t operator()(const T& key) const{
    return std::hash<T>()(key)+1;
  }
};


}// end namespace nodamushi

#endif // __NODAMUSHI_REFHASH_HPP__
