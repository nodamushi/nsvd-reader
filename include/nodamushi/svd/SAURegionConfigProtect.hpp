/*!
  @brief protect enum of sauRegionsConfig.region.access
  @file nodamushi/svd/SAURegionConfigProtect.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef __NODAMUSHI_SVD_SAUREGIONCONFIGPROTECT_HPP__
#define __NODAMUSHI_SVD_SAUREGIONCONFIGPROTECT_HPP__

#include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

#include "nodamushi/svd/enum_helper.hpp"

namespace nodamushi{
namespace svd{

/**
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_cpu.html#elem_region
 */
enum SAURegionConfigProtect
{
  //! n: non-secure
  NON_SECURE = 0, 
  //! c: secure callable
  SECURE_CALLABLE 
};

//----------------------------------------------

template<>struct enum_helper<SAURegionConfigProtect>
{
  static constexpr bool HAS_HELPER=true;
# if __cplusplus >= 201703
  static constexpr std::optional<SAURegionConfigProtect> get(std::string_view name)
  {
    if(name == "n")return SAURegionConfigProtect::NON_SECURE;
    if(name == "c")return SAURegionConfigProtect::SECURE_CALLABLE;
    return std::nullopt;
  }
  static constexpr bool get(std::string_view name,SAURegionConfigProtect& dst)
  {
    if(auto v = get(name)){ dst = *v;return true;}
    return false;
  }
# endif

# if __cplusplus >= 201703
  static constexpr std::string_view nameof(SAURegionConfigProtect value)
# else
  static constexpr const char* nameof(SAURegionConfigProtect value)
# endif
  {
    switch(value){
      case SAURegionConfigProtect::NON_SECURE:return "n";
      case SAURegionConfigProtect::SECURE_CALLABLE:return "c";
    }
    return "";
  }
  static bool get(const char* name,SAURegionConfigProtect &dst)
  {
    if(std::strcmp(name,"n"))
    {
      dst = SAURegionConfigProtect::NON_SECURE;
      return true;
    }
    if(std::strcmp(name,"c"))
    {
      dst = SAURegionConfigProtect::SECURE_CALLABLE;
      return true;
    }
    return false;
  }
  static bool get(const std::string &name,SAURegionConfigProtect &dst)
  {
    if(name.size() != 1)return false;
    char c = name[0];
    if(c == 'n')
    {
      dst = SAURegionConfigProtect::NON_SECURE;
      return true;
    }
    if(c == 'c')
    {
      dst = SAURegionConfigProtect::SECURE_CALLABLE;
      return true;
    }
    return false;
  }
};

}}


namespace std{
ostream& operator <<(std::ostream& os,const ::nodamushi::svd::SAURegionConfigProtect& value)
{
  os << ::nodamushi::svd::enum_helper<::nodamushi::svd::SAURegionConfigProtect>::nameof(value);
  return os;
}
}

#endif // __NODAMUSHI_SVD_SAUREGIONCONFIGPROTECT_HPP__
