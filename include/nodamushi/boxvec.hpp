/*!
  @brief boxvec. vector<pointer<X>> 
  @file nodamushi/boxvec.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef __NODAMUSHI_BOXVEC_HPP__
#define __NODAMUSHI_BOXVEC_HPP__

# include <vector>
# include <memory>
# include <cstdint>
# include <utility>
# include <algorithm>
# include <functional>

# include "nodamushi/box.hpp"

namespace nodamushi{

/**
 * @brief iterator for boxvec
 */
template<typename T,typename ITR>
struct boxitr
{
  ITR itr;

  boxitr<T,ITR>& operator ++(){++itr;return *this;}
  boxitr<T,ITR> operator ++(int){
    boxitr<T,ITR> r{itr};
    ++itr;
    return r;
  }

  boxitr<T,ITR>& operator --(){--itr;return *this;}
  boxitr<T,ITR> operator --(int){
    boxitr<T,ITR> r{itr};
    --itr;
    return r;
  }
  boxitr<T,ITR> operator +(int n)const{return {itr+n};}
  boxitr<T,ITR> operator -(int n)const{return {itr-n};}
  boxitr<T,ITR>& operator +=(int n){itr+=n;return *this;}
  boxitr<T,ITR>& operator -=(int n){itr+=n;return *this;}


  T& operator*()noexcept{return **itr;}
  const T& operator*()const noexcept{return **itr;}
  const T& operator->()const noexcept{return (*itr).operator->();}
  T& operator->()noexcept{return (*itr).operator->();}
  bool operator <(const boxitr<T,ITR>& i)const noexcept{return itr < i.itr;}
  bool operator <=(const boxitr<T,ITR>& i)const noexcept{return itr <= i.itr;}
  bool operator ==(const boxitr<T,ITR>& i)const noexcept{return itr == i.itr;}
  bool operator !=(const boxitr<T,ITR>& i)const noexcept{return itr != i.itr;}
  bool operator >(const boxitr<T,ITR>& i)const noexcept{return itr > i.itr;}
  bool operator >=(const boxitr<T,ITR>& i)const noexcept{return itr >= i.itr;}
};

/**
 * @brief vector<pointer<T>>.
 * @par
 * This list contains pointer eleent.
 * but , boxvec[x] return reference.
 * @code
 * boxvec<T> vector;
 * .....
 * T& v = vector[0];
 * @endcode
 */
template<typename T,typename PTR=::nodamushi::box<T>>
class boxvec
{
 public:
  using box =PTR;
  using vec_t=std::vector<PTR>;

  using iterator = boxitr<T,typename vec_t::iterator>;
  using const_iterator = boxitr<const T,typename vec_t::const_iterator>;
  using size_type = typename vec_t::size_type;
  using reference =T&;
  using const_reference = const T&;
  using value_type = T;
  using reverse_iterator = boxitr<T,typename vec_t::reverse_iterator>;
  using const_reverse_iterator = boxitr<const T,typename vec_t::const_reverse_iterator>;

  boxvec():_v{}{}
  boxvec(size_type initial_capacity):_v{}
  {
    _v.reserve(initial_capacity);
  };
  //--------------------------------------------------
  // deap copy
  //--------------------------------------------------
  //! deap copy
  boxvec(const boxvec<T,PTR>& src):_v{}
  {
    _v.reserve(src.size());
    for(const T& t:src)
      _v.emplace_back(new T(t));
  }
  //! deap copy
  boxvec<T,PTR>& operator=(const boxvec<T,PTR>& src)
  {
    _v.clear();
    _v.reserve(src.size());
    for(const T& t:src)
      _v.emplace_back(new T(t));
    return *this;
  }
 //--------------------------------------------------
  template<typename P>
  boxvec(boxvec<T,P>&& src):_v(std::move(src._v)){}
  template<typename P>
  boxvec<T,PTR>& operator=(boxvec<T,P>&& src){_v = std::move(src._v);}
  //--------------------------------------------------

  template<typename P>
  void add_all(const boxvec<T,P>& src)
  {
    _v.reserved(src.size()+_v.size());
    for(const auto& t:src)
      _v.emplace_back(new T(t));
  }
  template<typename P>
  void add_all(boxvec<T,P>&& src)
  {
    _v.reserved(src.size()+_v.size());
    for(const auto& t:src._v)
      _v.emplace_back(std::move(t));
  }
  operator bool()const noexcept{return !_v.empty();}

  size_type size()const noexcept{return _v.size();}
  size_type max_size()const noexcept{return _v.max_size();}
  size_type capacity()const noexcept{return _v.capacity();}
  bool empty()const noexcept{return _v.empty();}
  void reserve(size_type n){_v.reserve(n);}
  void shurink_to_fit(){_v.shurink_to_fit();}

  reference operator[](size_type n){return *_v[n];}
  const_reference operator[](size_type n)const{return *_v[n];}

  reference at(size_type n){return *(_v.at(n));}
  const_reference at(size_type n)const{return *(_v.at(n));}
  PTR ptr_at(size_type n){return _v.at(n);}
  const PTR ptr_at(size_type n)const{return _v.at(n);}


  reference front(){return *_v.front();}
  const_reference front()const{return *_v.front();}

  reference back(){return *_v.back();}
  const_reference back()const{return *_v.back();}

  template <class InputIterator>
  void assign(InputIterator first, InputIterator last)
  {
    while(first!=last){
      _v.emplace_back(new T(*first));
      ++first;
    }
  }

  void push_back(const T& t)
  {
    _v.emplace_back(new T(t));
  }
  void push_back(T&& t)
  {
    _v.emplace_back(new T(std::move(t)));
  }
  void push_back(box&& t)
  {
    _v.emplace_back(std::move(t));
  }

  void push_back(const box& t)
  {
    _v.emplace_back(t);
  }

  template<typename... ARGS>
  void emplace_back(ARGS&&... args)
  {
    _v.emplace_back(new T(std::forward<ARGS>(args)...));
  }
  template <class... ARGS>
  iterator emplace(const_iterator position, ARGS&&... args)
  {
    return {_v.emplace(position.itr,new T(std::forward<ARGS>(args)...))};
  }

  void pop_back(){_v.pop_back();}

  iterator insert(iterator position, const T& x)
  {
    _v.insert(position.itr,box(new T(x)));
  }
  iterator insert(const_iterator position, const T& x)
  {
    _v.insert(position.itr,box(new T(x)));
  }

  iterator insert(const_iterator position, T&& x)
  {
    _v.insert(position.itr,box(new T(std::move(x))));
  }
  template <class InputIterator>
  iterator insert(iterator position,
                  InputIterator first, InputIterator last)
  {
    auto i = position.itr;
    while(first != last){
      _v.insert(i,box(new T(*first)));
      ++first;++i;
    }
    return {i};
  }

  iterator erase(iterator position){return {_v.erase(position.itr)};}
  iterator erase(const_iterator position){return {_v.erase(position.itr)};}
  iterator erase(iterator first,iterator last){return {_v.erase(first.itr,last.itr)};}
  iterator erase(const_iterator first,const_iterator last){return {_v.erase(first.itr,last.itr)};}



  void swap(boxvec<T,PTR>& x){_v.swap(x._v);}
  void clear(){_v.clear();}



  iterator begin()noexcept{return {_v.begin()};}
  const_iterator begin()const noexcept{return {_v.begin()};}
  const_iterator cbegin()const noexcept{return {_v.cbegin()};}
  iterator end() noexcept{return {_v.end()};}
  const_iterator end()const noexcept{return {_v.cend()};}
  const_iterator cend()const noexcept{return {_v.cend()};}

  typename vec_t::iterator ptr_begin()noexcept{return _v.begin();}
  typename vec_t::const_iterator ptr_begin()const noexcept{return _v.begin();}
  typename vec_t::iterator ptr_end() noexcept{return _v.end();}
  typename vec_t::const_iterator ptr_end()const noexcept{return _v.cend();}

  reverse_iterator rbegin() noexcept{return {_v.rbegin()};}
  reverse_iterator rend() noexcept{return {_v.rend()};}
  const_reverse_iterator rbegin()const noexcept{return {_v.rbegin()};}
  const_reverse_iterator rend()const noexcept{return {_v.rend()};}
  const_reverse_iterator crbegin()const noexcept{return {_v.crbegin()};}
  const_reverse_iterator crend()const noexcept{return {_v.crend()};}

  //! @brief sort elements
  void sort(std::function<bool(const T&,const T&)> comp)
  {
    std::sort(_v.begin(),_v.end(),
              [&comp](const box& a,const box& b)->bool{
                return comp(*a,*b);
              });
  }

  //! @brief sort elements
  template<bool Compare(const T&,const T&)>
  void sort()
  {
    std::sort(_v.begin(),_v.end(),
              [](const box& a,const box& b)->bool{
                return Compare(*a,*b);
              });
  }

 private:
  vec_t _v;
};


}// end namespace nodamushi

#endif // __NODAMUSHI_BOXVEC_HPP__
