/*!
  @brief value&lt;void&gt;
  @file nodamushi/svd/value/void_value.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef NODAMUSHI_SVD_VALUE_VOID_VALUE_HPP
#define NODAMUSHI_SVD_VALUE_VOID_VALUE_HPP

# include "nodamushi/svd/value.hpp"
namespace nodamushi{
namespace svd{

// void
template<bool attribute,bool r,char... name>struct value<void,attribute,r,name...>
{
  using type = void;
  static constexpr bool REQUIRED=false;
  static constexpr bool ATTRIBUTE=attribute;


  constexpr bool empty()const noexcept{return true;}
  constexpr operator bool()const noexcept{return false;}
  constexpr bool check_require()const noexcept{return true;}
  constexpr bool is_required()const noexcept{return false;}
  constexpr bool is_attribute()const noexcept{return attribute;}
  int operator*()const noexcept{return 0;}
  int get()const noexcept{return 0;}
  NODAMUSHI_CONSTEXPR_STRING get_name()const noexcept
  {
# if __cplusplus >= 201703
    return get_const_string<name...>();
# else
    const char arr[] = {name...,'\0'};
    return std::string(arr);
# endif
  }
};
}}// end namespace nodamushi::svd

#endif // NODAMUSHI_SVD_VALUE_VOID_VALUE_HPP
