/*!
 * @brief Define &lt;modifiedWriteValues&gt; enum
 * @file nodamushi/svd/ModifiedWriteValues.hpp
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_MODIFIEDWRITEVALUES_HPP__
#define __NODAMUSHI_SVD_MODIFIEDWRITEVALUES_HPP__

# include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

#include "nodamushi/svd/enum_helper.hpp"

namespace nodamushi{
namespace svd{


/**
 * &lt;register.modifiedWriteValues&gt; enum
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_register
 */
enum class ModifiedWriteValues
{
  //! @brief set
  SET=0,
  //! @brief oneToClear
  ONE_TO_CLEAR,
  //! @brief oneToSet
  ONE_TO_SET,
  //! @brief oneToToggle
  ONE_TO_TOGGLE,
  //! @brief zeroToClear
  ZERO_TO_CLEAR,
  //! @brief zeroToSet
  ZERO_TO_SET,
  //! @brief zeroToToggle
  ZERO_TO_TOGGLE,
  //! @brief clear
  CLEAR,
  //! @brief modify
  MODIFY,
};

//-------------------------------------------------------------

template<>struct enum_helper<ModifiedWriteValues>
{
  static constexpr bool HAS_HELPER=true;
# if __cplusplus >= 201703
  static constexpr std::optional<ModifiedWriteValues> get(std::string_view name)
  {
    if(name == "oneToClear")return ModifiedWriteValues::ONE_TO_CLEAR;
    if(name == "oneToSet")return ModifiedWriteValues::ONE_TO_SET;
    if(name == "oneToToggle")return ModifiedWriteValues::ONE_TO_TOGGLE;
    if(name == "zeroToClear")return ModifiedWriteValues::ZERO_TO_CLEAR;
    if(name == "zeroToSet")return ModifiedWriteValues::ZERO_TO_SET;
    if(name == "zeroToToggle")return ModifiedWriteValues::ZERO_TO_TOGGLE;
    if(name == "clear")return ModifiedWriteValues::CLEAR;
    if(name == "set")return ModifiedWriteValues::SET;
    if(name == "modify")return ModifiedWriteValues::MODIFY;
    return std::nullopt;
  }
  static constexpr bool get(std::string_view name,ModifiedWriteValues& dst)
  {
    if(auto v = get(name)){ dst = *v;return true;}
    return false;
  }
# endif

# if __cplusplus >= 201703
  static constexpr std::string_view nameof(ModifiedWriteValues value)
# else
  static constexpr const char* nameof(ModifiedWriteValues value)
# endif
  {
    switch(value){
      case ModifiedWriteValues::ONE_TO_CLEAR:return "oneToClear";
      case ModifiedWriteValues::ONE_TO_SET:return "oneToSet";
      case ModifiedWriteValues::ONE_TO_TOGGLE:return "oneToToggle";
      case ModifiedWriteValues::ZERO_TO_CLEAR:return "zeroToClear";
      case ModifiedWriteValues::ZERO_TO_SET:return "zeroToSet";
      case ModifiedWriteValues::ZERO_TO_TOGGLE:return "zeroToToggle";
      case ModifiedWriteValues::CLEAR:return "clear";
      case ModifiedWriteValues::SET:return "set";
      case ModifiedWriteValues::MODIFY:return "modify";
    }
    return "";
  }
  static bool get(const char* name,ModifiedWriteValues &dst)
  {
    if(std::strcmp(name,"oneToClear"))
    {
      dst = ModifiedWriteValues::ONE_TO_CLEAR;
      return true;
    }
    if(std::strcmp(name,"oneToSet"))
    {
      dst = ModifiedWriteValues::ONE_TO_SET;
      return true;
    }
    if(std::strcmp(name,"oneToToggle"))
    {
      dst = ModifiedWriteValues::ONE_TO_TOGGLE;
      return true;
    }
    if(std::strcmp(name,"zeroToClear"))
    {
      dst = ModifiedWriteValues::ZERO_TO_CLEAR;
      return true;
    }
    if(std::strcmp(name,"zeroToSet"))
    {
      dst = ModifiedWriteValues::ZERO_TO_SET;
      return true;
    }
    if(std::strcmp(name,"zeroToToggle"))
    {
      dst = ModifiedWriteValues::ZERO_TO_TOGGLE;
      return true;
    }
    if(std::strcmp(name,"clear"))
    {
      dst = ModifiedWriteValues::CLEAR;
      return true;
    }
    if(std::strcmp(name,"set"))
    {
      dst = ModifiedWriteValues::SET;
      return true;
    }
    if(std::strcmp(name,"modify"))
    {
      dst = ModifiedWriteValues::MODIFY;
      return true;
    }
    return false;
  }
  static bool get(const std::string &name,ModifiedWriteValues &dst)
  {
    if(name == "oneToClear")
    {
      dst = ModifiedWriteValues::ONE_TO_CLEAR;
      return true;
    }
    if(name == "oneToSet")
    {
      dst = ModifiedWriteValues::ONE_TO_SET;
      return true;
    }
    if(name == "oneToToggle")
    {
      dst = ModifiedWriteValues::ONE_TO_TOGGLE;
      return true;
    }
    if(name == "zeroToClear")
    {
      dst = ModifiedWriteValues::ZERO_TO_CLEAR;
      return true;
    }
    if(name == "zeroToSet")
    {
      dst = ModifiedWriteValues::ZERO_TO_SET;
      return true;
    }
    if(name == "zeroToToggle")
    {
      dst = ModifiedWriteValues::ZERO_TO_TOGGLE;
      return true;
    }
    if(name == "clear")
    {
      dst = ModifiedWriteValues::CLEAR;
      return true;
    }
    if(name == "set")
    {
      dst = ModifiedWriteValues::SET;
      return true;
    }
    if(name == "modify")
    {
      dst = ModifiedWriteValues::MODIFY;
      return true;
    }
    return false;
  }
};

}
}// end namespace nodamushi
namespace std{
inline ostream& operator <<(ostream& os,const ::nodamushi::svd::ModifiedWriteValues& value)
{
  os << ::nodamushi::svd::enum_helper<::nodamushi::svd::ModifiedWriteValues>::nameof(value);
  return os;
}
}
#endif //__NODAMUSHI_MODIFIEDWRITEVALUES_HPP__
