/*!
 * @brief  Define &lt;enumeratedValues&gt;.&lt;usage&gt; enum
 * @file nodamushi/svd/EnumUsage.hpp
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_ENUMUSAGE_HPP
#define NODAMUSHI_SVD_ENUMUSAGE_HPP

# include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

#include "nodamushi/svd/enum_helper.hpp"

namespace nodamushi{
namespace svd{

/**
 *  @brief enumeratedValues.usage element
 *  @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_enumeratedValues
 */
enum class EnumUsage
{
  //! @brief read-write
  READ_WRITE=0, 
  //! @brief read
  READ,  
  //! @brief write
  WRITE,  
};
template<>struct enum_helper<EnumUsage>
{
  static constexpr bool HAS_HELPER=true;
# if __cplusplus >= 201703
  static constexpr std::optional<EnumUsage> get(std::string_view name)
  {
    if(name == "read")return EnumUsage::READ;
    if(name == "write")return EnumUsage::WRITE;
    if(name == "read-write")return EnumUsage::READ_WRITE;
    return std::nullopt;
  }
  static constexpr bool get(std::string_view name,EnumUsage& dst)
  {
    if(auto v = get(name)){ dst = *v;return true;}
    return false;
  }
# endif

# if __cplusplus >= 201703
  static constexpr std::string_view nameof(EnumUsage value)
# else
  static constexpr const char* nameof(EnumUsage value)
# endif
  {
    switch(value){
      case EnumUsage::READ:return "read";
      case EnumUsage::WRITE:return "write";
      case EnumUsage::READ_WRITE:return "read-write";
    }
    return "";
  }
  static bool get(const char* name,EnumUsage &dst)
  {
    if(std::strcmp(name,"read"))
    {
      dst = EnumUsage::READ;
      return true;
    }
    if(std::strcmp(name,"write"))
    {
      dst = EnumUsage::WRITE;
      return true;
    }
    if(std::strcmp(name,"read-write"))
    {
      dst = EnumUsage::READ_WRITE;
      return true;
    }
    return false;
  }
  static bool get(const std::string &name,EnumUsage &dst)
  {
    if(name == "read")
    {
      dst = EnumUsage::READ;
      return true;
    }
    if(name == "write")
    {
      dst = EnumUsage::WRITE;
      return true;
    }
    if(name == "read-write")
    {
      dst = EnumUsage::READ_WRITE;
      return true;
    }
    return false;
  }
};

}
}// end namespace nodamushi
namespace std{
inline ostream& operator <<(ostream& os,const ::nodamushi::svd::EnumUsage& value)
{
  os << ::nodamushi::svd::enum_helper<::nodamushi::svd::EnumUsage>::nameof(value);
  return os;
}
}

#endif //NODAMUSHI_SVD_ENUMUSAGE_HPP
