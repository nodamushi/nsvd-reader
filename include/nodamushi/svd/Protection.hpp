/*!
 * @brief Define &lt;protection&gt; enum
 * @file nodamushi/svd/Protection.hpp
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_PROTECTION_HPP
#define NODAMUSHI_SVD_PROTECTION_HPP

# include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

#include "nodamushi/svd/enum_helper.hpp"

namespace nodamushi{
namespace svd{

//! @brief &lt;protection&gt; enum
enum class Protection
{
  //! n: non-secure
  NON_SECURE=0,
  //! s: secure
  SECURE,
  //! p: privileged
  PRIVILEGED,
};
//--------------------------------------------------
template<>struct enum_helper<Protection>
{
  static constexpr bool HAS_HELPER=true;
# if __cplusplus >= 201703
  static constexpr std::optional<Protection> get(std::string_view name)
  {
    if(name == "s")return Protection::SECURE;
    if(name == "n")return Protection::NON_SECURE;
    if(name == "p")return Protection::PRIVILEGED;
    return std::nullopt;
  }
  static constexpr bool get(std::string_view name,Protection& dst)
  {
    if(auto v = get(name)){ dst = *v;return true;}
    return false;
  }
# endif

# if __cplusplus >= 201703
  static constexpr std::string_view nameof(Protection value)
# else
  static constexpr const char* nameof(Protection value)
# endif
  {
    switch(value){
      case Protection::SECURE:return "s";
      case Protection::NON_SECURE:return "n";
      case Protection::PRIVILEGED:return "p";
    }
    return "";
  }
  static bool get(const char* name,Protection &dst)
  {
    if(std::strcmp(name,"s"))
    {
      dst = Protection::SECURE;
      return true;
    }
    if(std::strcmp(name,"n"))
    {
      dst = Protection::NON_SECURE;
      return true;
    }
    if(std::strcmp(name,"p"))
    {
      dst = Protection::PRIVILEGED;
      return true;
    }
    return false;
  }
  static bool get(const std::string &name,Protection &dst)
  {
    if(name == "s")
    {
      dst = Protection::SECURE;
      return true;
    }
    if(name == "n")
    {
      dst = Protection::NON_SECURE;
      return true;
    }
    if(name == "p")
    {
      dst = Protection::PRIVILEGED;
      return true;
    }
    return false;
  }
};
}}// end namespace nodamushi::svd

namespace std{
inline ostream& operator <<(ostream& os,const ::nodamushi::svd::Protection& value)
{
  os << ::nodamushi::svd::enum_helper<::nodamushi::svd::Protection>::nameof(value);
  return os;
}
}// end namespace std
#endif //NODAMUSHI_SVD_PROTECTION_HPP
