/*!
  @brief &lt;access&gt; enum
  @file nodamushi/svd/Access.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_ACCESS_HPP__
#define __NODAMUSHI_SVD_ACCESS_HPP__

#include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

#include "nodamushi/svd/enum_helper.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief Read/Write Access type enum.&lt;access&gt;
 * Read-Write/Read-Only/Write-Only/Write-Once/Read- Write Once
 * @see is_writable()
 * @see is_readable()
 */
enum class Access
{
  //! @brief read-write
  READ_WRITE=0,
  //! @brief read-only
  READ_ONLY, 
  //! @brief write-only
  WRITE_ONLY,
  //! @brief writeOnce
  WRITE_ONCE,
  //! @brief read-writeOnce
  READ_WRITE_ONCE,
};

/**
 * @brief when access is writable ,return true.
 */
inline constexpr bool is_writable(Access access)
{
  return access != Access::READ_ONLY;
}
/**
 * @brief when access is readable ,return true.
 */
inline constexpr bool is_readable(Access access)
{
  return access != Access::WRITE_ONLY && access != Access::WRITE_ONCE;
}

//-------------------------------------------------------------

template<>struct enum_helper<Access>
{
  static constexpr bool HAS_HELPER=true;
# if __cplusplus >= 201703
  static constexpr std::optional<Access> get(std::string_view name)
  {
    if(name == "read-only")return Access::READ_ONLY;
    if(name == "write-only")return Access::WRITE_ONLY;
    if(name == "read-write")return Access::READ_WRITE;
    if(name == "writeOnce")return Access::WRITE_ONCE;
    if(name == "read-writeOnce")return Access::READ_WRITE_ONCE;
    return std::nullopt;
  }
  static constexpr bool get(std::string_view name,Access& dst)
  {
    if(auto v = get(name)){ dst = *v;return true;}
    return false;
  }
# endif

# if __cplusplus >= 201703
  static constexpr std::string_view nameof(Access value)
# else
  static constexpr const char* nameof(Access value)
# endif
  {
    switch(value){
      case Access::READ_ONLY:return "read-only";
      case Access::WRITE_ONLY:return "write-only";
      case Access::READ_WRITE:return "read-write";
      case Access::WRITE_ONCE:return "writeOnce";
      case Access::READ_WRITE_ONCE:return "read-writeOnce";
    }
    return "";
  }
  static bool get(const char* name,Access &dst)
  {
    if(std::strcmp(name,"read-only"))
    {
      dst = Access::READ_ONLY;
      return true;
    }
    if(std::strcmp(name,"write-only"))
    {
      dst = Access::WRITE_ONLY;
      return true;
    }
    if(std::strcmp(name,"read-write"))
    {
      dst = Access::READ_WRITE;
      return true;
    }
    if(std::strcmp(name,"writeOnce"))
    {
      dst = Access::WRITE_ONCE;
      return true;
    }
    if(std::strcmp(name,"read-writeOnce"))
    {
      dst = Access::READ_WRITE_ONCE;
      return true;
    }
    return false;
  }
  static bool get(const std::string &name,Access &dst)
  {
    if(name.size() < 8)return false;
    if(name[0] == 'r'){
      if(name[name.size()-1]=='y' &&name == "read-only")
      {
        dst = Access::READ_ONLY;
        return true;
      }
      if(name[name.size()-2]=='t' && name == "read-write")
      {
        dst = Access::READ_WRITE;
        return true;
      }
      if(name == "read-writeOnce")
      {
        dst = Access::READ_WRITE_ONCE;
        return true;
      }
    }else{
      if(name[name.size()-1]=='y' && name == "write-only")
      {
        dst = Access::WRITE_ONLY;
        return true;
      }
      if(name == "writeOnce")
      {
        dst = Access::WRITE_ONCE;
        return true;
      }
    }
    return false;
  }
};

}
}// end namespace nodamushi
namespace std{
inline ostream& operator <<(std::ostream& os,const ::nodamushi::svd::Access& value)
{
  os << ::nodamushi::svd::enum_helper<::nodamushi::svd::Access>::nameof(value);
  return os;
}
}
#endif //__NODAMUSHI_SVD_ACCESS_HPP__
