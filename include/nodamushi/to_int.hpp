/*!
  @brief to_int
  @file nodamushi/to_int.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef __NODAMUSHI_TO_INT_HPP__
#define __NODAMUSHI_TO_INT_HPP__

# include <cstdint>
# include <string>
# if __cplusplus >= 201703
#  include <string_view>
#  include <optional>
#  include <charconv>
# else
#  include <type_traits>
# endif


namespace nodamushi{
# if __cplusplus >= 201703
template<typename INT>
INT to_int(std::string_view v,int base=10)
{
  INT x;
  std::from_chars(v.data(), v.data()+v.length(),x,base);
  return x;
}
template<typename INT>
INT to_int(std::string_view v,size_t begin,size_t end,int base=10)
{
  INT x;
  std::from_chars(v.data()+begin, v.data()+end,x,base);
  return x;
}
template<typename INT>
INT to_int(const char* text,size_t begin,size_t end,int base=10)
{
  INT x;
  std::from_chars(text+begin, text+end,x,base);
  return x;
}
template<typename INT>
INT to_int(const char* text,int base=10)
{
  INT x;
  const char* end=text;
  while(*text)text++;
  std::from_chars(text,end,x,base);
  return x;
}


# else

namespace details{
template<bool> struct _to_int;

template<> struct _to_int<true>
{
  static unsigned long long conv(const std::string& v,int base=10)
  {
    return ::std::stoull(v,nullptr,base);
  }
};
template<> struct _to_int<false>
{
  static long long conv(const std::string& v,int base=10)
  {
    return ::std::stoll(v,nullptr,base);
  }
};
}

template<typename INT>
INT to_int(const std::string& v,int base=10)
{
  constexpr bool unsig = std::is_unsigned<INT>::value;
  return static_cast<INT>(details::_to_int<unsig>::conv(v,base));
}

template<typename INT>
INT to_int(const std::string& v,size_t begin,size_t end,int base=10)
{
  std::string s = v.substr(begin,end-begin);
  return to_int<INT>(s,base);
}
template<typename INT>
INT to_int(const char* text,size_t begin,size_t end,int base=10)
{
  std::string s(text+begin,text+end);
  return to_int<INT>(s,base);
}
template<typename INT>
INT to_int(const char* text,int base=10)
{
  const std::string s = text;
  return to_int<INT>(s,base);
}

# endif


}// end namespace nodamushi

#endif // __TO_INT_HPP__
