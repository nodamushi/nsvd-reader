/*!
  @brief optional like container
  @file nodamushi/svd/normalized/nullable.hpp
*/
#ifndef __NODAMUSHI_SVD_NORMALIZE_NULLABLE_HPP__
#define __NODAMUSHI_SVD_NORMALIZE_NULLABLE_HPP__

# include <utility>
# include <type_traits>
# include "nodamushi/svd/value.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{

/**
 * @brief a data container like optional type.
 * @param T data type.T muslt be a fundamental type or a enum type.
 */
template<typename T>
struct nullable{
  
  static_assert(std::is_fundamental<T>::value
                || std::is_enum<T>::value
                ,"T must be fundamental or enum");

  nullable():t(),enable(false){};
  nullable(T v):t(v),enable(true){};
  nullable(T v,bool e):t(v),enable(e){};

  template<bool attribute,bool required,char... name>
  nullable(const ::nodamushi::svd::value<T,attribute,required,name...> & v):
      t(v.get(static_cast<T>(0))),enable(v){}

  
  nullable(const nullable&) = default;
  nullable(nullable&&) = default;
  nullable& operator=(const nullable&) = default;
  nullable& operator=(nullable&&) = default;
  
  operator bool()const noexcept{return enable;}
  bool empty()const noexcept{return !enable;}
  
  //! @brief set value
  nullable& operator =(T v)
  {
    t = v;
    enable =true;
    return *this;
  }

  //! @brief clear value
  nullable& clear()
  {
    t = T{};
    enable = false;
    return *this;
  }
  
  bool operator==(T v)const noexcept
  {
    return enable && t ==v;
  }

  bool operator!=(T v)const noexcept
  {
    return !enable || t != v;
  }

  T get(T defaultValue)const noexcept{return enable?t:defaultValue;}
  
  template<T FUNC()> 
  T get()const{return enable? t: FUNC();}

  template<typename SRC,T FUNC(const SRC&)> 
  T get(const SRC* src)const{return enable? t: FUNC(*src);}

 private:
  T t;
  bool enable;
};


}}}// end namespace nodamushi

#endif // __NODAMUSHI_SVD_NORMALIZE_NULLABLE_HPP__
