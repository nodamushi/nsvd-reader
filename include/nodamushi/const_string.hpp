/*!
  @brief constexpr small library
  @file nodamushi/const_string.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_CONST_STRING_HPP__
#define __NODAMUSHI_CONST_STRING_HPP__

# include <cstdint>
# include <string>
# include <ostream>

# if __cplusplus >= 201703
#  include <string_view>
#  include <optional>
# endif


# ifdef __GNUC__
#  ifndef  _ENABLE_GNU_TEMPLATE_STRING_
#   define _ENABLE_GNU_TEMPLATE_STRING_
#   ifdef __clang__
#    pragma GCC diagnostic ignored "-Wgnu-string-literal-operator-template"
#   endif
#  endif
# endif


# include "nodamushi/string_type.hpp"

namespace nodamushi{

# if __cplusplus >= 201703
template<typename T>
using optional = std::optional<T>;
constexpr inline bool is_empty(const std::string_view& v){return v.empty();}
# endif

constexpr inline bool is_empty(const char* v){return !v || *v == '\0';}
inline bool is_empty(const std::string& v){return v.empty();}

namespace details{
template<char... chars>struct template_char_length;
template<char... chars>struct template_text;
}

template<char... chars>
constexpr size_t get_const_string_length()
{
  return details::template_char_length<chars...>::value;
}

template<char... chars>
constexpr char at_const_string(size_t index){
  if(sizeof...(chars) <= index){return '\0';}
  char TEXT[] = {chars...};
  return TEXT[index];
}
template<size_t N>
constexpr char at(const char (&array)[N],size_t index)
{
  return index < N?array[index]:'\0';
}
template<size_t MAX_SIZE,size_t N>
constexpr char at0(const char (&array)[N])
{
  static_assert(N <= MAX_SIZE,"N<=MAX_SIZE.");
  return N != 0?array[0]:'\0';
}


template<char... chars>
constexpr const_string get_const_string()
{
  return details::template_text<chars...>::TEXT;
}

# if __cplusplus >= 201703
#  define NODAMUSHI_LOCAL_CONST_STRING(CHARS,varname) std::string_view varname =::nodamushi::details::template_text<CHARS...>::TEXT
# else
#  define NODAMUSHI_LOCAL_CONST_STRING(CHARS,varname) constexpr char varname[] = {CHARS...,'\0'}
# endif



template<char... chars>
std::ostream& print_const_string(std::ostream& o)
{
  NODAMUSHI_LOCAL_CONST_STRING(chars,x);
  o << x;
  return o;

}
}// end namespace nodamushi





namespace nodamushi{
namespace details{

template<char... chars>
struct template_char_length{
 private:
  static constexpr size_t count(){
    char TEXT[] = {chars...,'\0'};
    for(size_t i=0;i<sizeof...(chars);i++){
      if(TEXT[i] == '\0')return i;
    }
    return sizeof...(chars);
  }
  
 public:
  static constexpr size_t value = count();
};

template<char... chars>struct template_text
{
# if __cplusplus >= 201703
 private:
  static inline constexpr char arr[] = {chars...};
 public:
  static inline constexpr std::string_view TEXT = std::string_view(
      arr,template_char_length<chars...>::value);
# else
  static constexpr char TEXT[] = {chars...,'\0'};
# endif
};


} // end namespace details
} // end namespace nodamushi

#endif // __NODAMUSHI_CONST_STRING_HPP__
