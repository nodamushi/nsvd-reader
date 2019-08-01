/*!
  @brief value imple
  @file nodamushi/svd/value/imple.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef __NODAMUSHIMPLE_SVD_VALUE_IMPLE_HPP__
#define __NODAMUSHIMPLE_SVD_VALUE_IMPLE_HPP__

# include <vector>
# include <memory>
# include "nodamushi/box.hpp"
# include "nodamushi/boxvec.hpp"
# include "nodamushi/svd/value.hpp"
namespace nodamushi{
namespace svd{

namespace details{//--------------------------------------

template<char... name>
inline empty_value_access_error no_value_error()
{
  std::stringstream s;
  s << "empty_value_access_error: ";
  print_const_string<name...>(s)<<" has no value.";
  std::string text = s.str();
  return empty_value_access_error(text);
}


template<typename T>
inline void value_clear_T(T& t)
{
  t = T();
}

template<typename T>
inline void value_clear_T(std::vector<T>& t)
{
  t.clear();
}

template<typename T>
inline void value_clear_T(::nodamushi::box<T>& t)
{
  t.release();
}

template<typename T,typename PTR>
inline void value_clear_T(::nodamushi::boxvec<T,PTR>& t)
{
  t.clear();
}


template<typename T>
using value_type_t = typename ::nodamushi::svd::vattr_unwrap<T>::type;

} // end namespace details -----------------------------


template<typename T,bool attribute,bool required,char... name>
NODAMUSHI_CONSTEXPR_STRING value<T,attribute,required,name...>::get_name()const noexcept
{
# if __cplusplus >= 201703
  return get_const_string<name...>();
# else
    const char arr[] = {name...,'\0'};
    return std::string(arr);
# endif
}
template<typename T,bool attribute,bool required,char... name>
const details::value_type_t<T>& value<T,attribute,required,name...>::get()const{
  if(!_has_value)  throw details::no_value_error<name...>();
  return _value;
}
template<typename T,bool attribute,bool required,char... name>
details::value_type_t<T>& value<T,attribute,required,name...>::get(){
  if(!_has_value)  throw details::no_value_error<name...>();
  return _value;
}

template<typename T,bool attribute,bool required,char... name>
void value<T,attribute,required,name...>::clear()noexcept
{
  _has_value = false;
  details::value_clear_T(_value);
}

} // end namespace svd
}// end namespace nodamushi

#endif // __NODAMUSHIMPLE_SVD_VALUE_IMPLE_HPP__
