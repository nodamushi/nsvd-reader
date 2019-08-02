/*!
 * @brief Define &lt;readAction&gt; enum
 * @file nodamushi/svd/ReadAction.hpp
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_READACTION_HPP__
#define __NODAMUSHI_SVD_READACTION_HPP__

# include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

#include "nodamushi/svd/enum_helper.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief &lt;register&gt;.&lt;readAction&gt; enum
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_register
 */
enum class ReadAction
{
  //! @brief default value.( &lt;readAction&gt; is not defined.)
  NO_ACTION =0,
  //! @brief clear
  CLEAR,
  //! @brief set
  SET,
  //! @brief modify
  MODIFY,
  //! @brief modifyExternal
  MODIFY_EXTERNAL,
};

//-----------------------------------------------------------------------

template<>struct enum_helper<ReadAction>
{
  static constexpr bool HAS_HELPER=true;
# if __cplusplus >= 201703
  static constexpr std::optional<ReadAction> get(std::string_view name)
  {
    if(name == "clear")return ReadAction::CLEAR;
    if(name == "set")return ReadAction::SET;
    if(name == "modify")return ReadAction::MODIFY;
    if(name == "modifyExternal")return ReadAction::MODIFY_EXTERNAL;
    return std::nullopt;
  }
  static constexpr bool get(std::string_view name,ReadAction& dst)
  {
    if(auto v = get(name)){ dst = *v;return true;}
    return false;
  }
# endif

# if __cplusplus >= 201703
  static constexpr std::string_view nameof(ReadAction value)
# else
  static constexpr const char* nameof(ReadAction value)
# endif
  {
    switch(value){
      case ReadAction::NO_ACTION:return "";
      case ReadAction::CLEAR:return "clear";
      case ReadAction::SET:return "set";
      case ReadAction::MODIFY:return "modify";
      case ReadAction::MODIFY_EXTERNAL:return "modifyExternal";
    }
    return "";
  }
  static bool get(const char* name,ReadAction &dst)
  {
    if(std::strcmp(name,"clear"))
    {
      dst = ReadAction::CLEAR;
      return true;
    }
    if(std::strcmp(name,"set"))
    {
      dst = ReadAction::SET;
      return true;
    }
    if(std::strcmp(name,"modify"))
    {
      dst = ReadAction::MODIFY;
      return true;
    }
    if(std::strcmp(name,"modifyExternal"))
    {
      dst = ReadAction::MODIFY_EXTERNAL;
      return true;
    }
    return false;
  }
  static bool get(const std::string &name,ReadAction &dst)
  {
    if(name == "clear")
    {
      dst = ReadAction::CLEAR;
      return true;
    }
    if(name == "set")
    {
      dst = ReadAction::SET;
      return true;
    }
    if(name == "modify")
    {
      dst = ReadAction::MODIFY;
      return true;
    }
    if(name == "modifyExternal")
    {
      dst = ReadAction::MODIFY_EXTERNAL;
      return true;
    }
    return false;
  }
};
}}// end namespace nodamushi::svd

namespace std{
inline ostream& operator <<(ostream& os,const ::nodamushi::svd::ReadAction& value)
{
  if(value == ::nodamushi::svd::ReadAction::NO_ACTION){
    os << "no action";
  }else{
    os << ::nodamushi::svd::enum_helper<::nodamushi::svd::ReadAction>::nameof(value);
  }
  return os;
}
}// end namespace std
#endif //__NODAMUSHI_SVD_READACTION_HPP__
