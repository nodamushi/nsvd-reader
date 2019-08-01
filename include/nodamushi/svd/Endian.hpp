/*!
  @brief Define Endian enum
  @file nodamushi/svd/Endian.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_ENDIAN_HPP__
#define __NODAMUSHI_SVD_ENDIAN_HPP__

# include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

#include "nodamushi/svd/enum_helper.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief endianness of the processor 
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_cpu.html
 */
enum class Endian
{
  //! @brief little endian
  LITTLE=0,  
  //! @brief big endian
  BIG,  
  //! @brief selectable
  SELECTABLE,
  //! @brief other
  OTHER,  
};

//-------------------------------------------------

template<>struct enum_helper<Endian>
{
  static constexpr bool HAS_HELPER=true;
# if __cplusplus >= 201703
  static constexpr std::optional<Endian> get(std::string_view name)
  {
    if(name == "little")return Endian::LITTLE;
    if(name == "big")return Endian::BIG;
    if(name == "selectable")return Endian::SELECTABLE;
    if(name == "other")return Endian::OTHER;
    return std::nullopt;
  }
  static constexpr bool get(std::string_view name,Endian& dst)
  {
    if(auto v = get(name)){ dst = *v;return true;}
    return false;
  }
# endif

# if __cplusplus >= 201703
  static constexpr std::string_view nameof(Endian value)
# else
  static constexpr const char* nameof(Endian value)
# endif
  {
    switch(value){
      case Endian::LITTLE:return "little";
      case Endian::BIG:return "big";
      case Endian::SELECTABLE:return "selectable";
      case Endian::OTHER:return "other";
    }
    return "";
  }
  static bool get(const char* name,Endian &dst)
  {
    if(std::strcmp(name,"little"))
    {
      dst = Endian::LITTLE;
      return true;
    }
    if(std::strcmp(name,"big"))
    {
      dst = Endian::BIG;
      return true;
    }
    if(std::strcmp(name,"selectable"))
    {
      dst = Endian::SELECTABLE;
      return true;
    }
    if(std::strcmp(name,"other"))
    {
      dst = Endian::OTHER;
      return true;
    }
    return false;
  }
  static bool get(const std::string &name,Endian &dst)
  {
    if(name == "little")
    {
      dst = Endian::LITTLE;
      return true;
    }
    if(name == "big")
    {
      dst = Endian::BIG;
      return true;
    }
    if(name == "selectable")
    {
      dst = Endian::SELECTABLE;
      return true;
    }
    if(name == "other")
    {
      dst = Endian::OTHER;
      return true;
    }
    return false;
  }
};

}
}// end namespace nodamushi
namespace std{
ostream& operator <<(ostream& os,const ::nodamushi::svd::Endian& value)
{
  os << ::nodamushi::svd::enum_helper<::nodamushi::svd::Endian>::nameof(value);
  return os;
}
}
#endif //__NODAMUSHI_SVD_ENDIAN_HPP__
