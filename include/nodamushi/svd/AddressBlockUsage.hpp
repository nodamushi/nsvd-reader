/*!
  @brief Define &lt;addressBlock&gt;.&lt;usage&gt; element
  @file nodamushi/svd/AddressBlockUsage.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_ADDRESSBLOCKUSAGE_HPP__
#define __NODAMUSHI_SVD_ADDRESSBLOCKUSAGE_HPP__

# include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

#include "nodamushi/svd/enum_helper.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief &lt;usage&gt; of &lt;addressBlock&gt; enum.
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_peripherals.html#elem_addressBlock
 */
enum class AddressBlockUsage
{
  //! undefined(default value)
  UNDEFINED=0,
  //! reserved
  RESERVED,  
  //! registers
  REGISTERS, 
  //! buffer
  BUFFER,    
};

//-------------------------------------------------------------

template<>struct enum_helper<AddressBlockUsage>
{
  static constexpr bool HAS_HELPER=true;
# if __cplusplus >= 201703
  static constexpr std::optional<AddressBlockUsage> get(std::string_view name)
  {
    if(name == "reserved")return AddressBlockUsage::RESERVED;
    if(name == "registers")return AddressBlockUsage::REGISTERS;
    if(name == "buffer")return AddressBlockUsage::BUFFER;
    return std::nullopt;
  }
  static constexpr bool get(std::string_view name,AddressBlockUsage& dst)
  {
    if(auto v = get(name)){ dst = *v;return true;}
    return false;
  }
# endif

# if __cplusplus >= 201703
  static constexpr std::string_view nameof(AddressBlockUsage value)
# else
  static constexpr const char* nameof(AddressBlockUsage value)
# endif
  {
    switch(value){
      case AddressBlockUsage::RESERVED:return "reserved";
      case AddressBlockUsage::REGISTERS:return "registers";
      case AddressBlockUsage::BUFFER:return "buffer";
      default:break;
    }
    return "";
  }
  static bool get(const char* name,AddressBlockUsage &dst)
  {
    if(std::strcmp(name,"reserved"))
    {
      dst = AddressBlockUsage::RESERVED;
      return true;
    }
    if(std::strcmp(name,"registers"))
    {
      dst = AddressBlockUsage::REGISTERS;
      return true;
    }
    if(std::strcmp(name,"buffer"))
    {
      dst = AddressBlockUsage::BUFFER;
      return true;
    }
    return false;
  }
  static bool get(const std::string &name,AddressBlockUsage &dst)
  {
    if(name == "reserved")
    {
      dst = AddressBlockUsage::RESERVED;
      return true;
    }
    if(name == "registers")
    {
      dst = AddressBlockUsage::REGISTERS;
      return true;
    }
    if(name == "buffer")
    {
      dst = AddressBlockUsage::BUFFER;
      return true;
    }
    return false;
  }
};

}
}// end namespace nodamushi

namespace std{
inline ostream& operator <<(ostream& os,const ::nodamushi::svd::AddressBlockUsage& value)
{
  os << ::nodamushi::svd::enum_helper<::nodamushi::svd::AddressBlockUsage>::nameof(value);
  return os;
}
} // end namespace std
#endif //__NODAMUSHI_SVD_ADDRESSBLOCKUSAGE_HPP__
