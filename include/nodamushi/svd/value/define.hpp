/*!
  @brief macros for value
  @file nodamushi/svd/value/define.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_VALUE_DEFINE_HPP__
#define __NODAMUSHI_SVD_VALUE_DEFINE_HPP__

#include "nodamushi/svd/value.hpp"
# include "nodamushi/const_string.hpp"

namespace nodamushi{
namespace svd{
namespace details{

# ifdef _ENABLE_GNU_TEMPLATE_STRING_

template<char... name>struct value_type_builder_name
{
  template<typename T,bool attribute,bool required>
  using type = ::nodamushi::svd::value<T,attribute,required,name...>;
};

template<typename T,bool attribute,bool required,typename name>struct value_type_builder
{
  using type = typename name::template type<T,attribute,required>;
};

# else

template<typename T,bool attribute,bool required,char... name>struct value_type_builder
{
  static constexpr size_t size = get_const_string_length<name...>();
  static constexpr char get_char(size_t index){return ::nodamushi::at_const_string<name...>(index);}

  template<typename s>struct seq;
  template<size_t... s>struct seq<std::integer_sequence<size_t, s...>>{
    using type = value<T,attribute,required,get_char(s)...>;};
  
  using type = typename seq<std::make_integer_sequence<size_t, size>>::type;
};
# endif

} // end namespace nodamushi::svd::details


# ifdef _ENABLE_GNU_TEMPLATE_STRING_

template<typename T,bool attribute,bool required,typename name>
using value_t = typename details::value_type_builder<T,attribute,required,name>::type;

template<typename C,C... name>
constexpr ::nodamushi::svd::details::value_type_builder_name<name...> operator ""_value_type_builder(){return {};}

# else

template<typename T,bool attribute,bool required,char... name>
using value_t = typename details::value_type_builder<T,attribute,required,name...>::type;

# endif

}} // end namespace nodamushi::svd



#define _SVD_VALUE_(type,name,attribute,reqired,MACRO,...) MACRO(type,name,attribute,required)
#define _SVD_VALUE0_(type,name,attribute,dummy) __SVD_VALUE__(type,attribute,false,name)
#define _SVD_VALUE1_(type,name,attribute,required) __SVD_VALUE__(type,attribute,required,name)

#  define __SVD_VALUE__(type,attribute,required,name)             \
  ::nodamushi::svd::value_t<type,attribute,required,__SVD_VALUE_EXPAND_NAME__(name)> name


# ifdef _ENABLE_GNU_TEMPLATE_STRING_
#  define __SVD_VALUE_EXPAND_NAME__(name)\
  decltype(#name##_value_type_builder)

# else

#  define __SVD_VALUE_EXPAND_NAME__(name) __SVD_VALUE_EXPAND_NAME2__(#name)
#  define __SVD_VALUE_EXPAND_NAME2__(name) ::nodamushi::at0<32>(name),::nodamushi::at(name,1 ),::nodamushi::at(name,2 ),::nodamushi::at(name,3 ),::nodamushi::at(name,4 ),::nodamushi::at(name,5 ),::nodamushi::at(name,6 ),::nodamushi::at(name,7 ),::nodamushi::at(name,8 ),::nodamushi::at(name,9 ),::nodamushi::at(name,10),::nodamushi::at(name,11),::nodamushi::at(name,12),::nodamushi::at(name,13),::nodamushi::at(name,14),::nodamushi::at(name,15),::nodamushi::at(name,16),::nodamushi::at(name,17),::nodamushi::at(name,18),::nodamushi::at(name,19),::nodamushi::at(name,20),::nodamushi::at(name,21),::nodamushi::at(name,22),::nodamushi::at(name,23),::nodamushi::at(name,24),::nodamushi::at(name,25),::nodamushi::at(name,26),::nodamushi::at(name,27),::nodamushi::at(name,28),::nodamushi::at(name,29),::nodamushi::at(name,30),::nodamushi::at(name,31)

# endif
#endif // __NODAMUSHI_SVD_VALUE_DEFINE_HPP__
