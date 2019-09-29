/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_ENUM_HELPER_HPP
#define NODAMUSHI_SVD_ENUM_HELPER_HPP

# include <string>
# include <cstring>
# if __cplusplus >= 201703
#  include <string_view>
#  include <optional>
# endif

namespace nodamushi{
namespace svd{
//! provide getter,setter
template<typename T>struct enum_helper
{
  //! enum_helper is defined
  static constexpr bool HAS_HELPER=false;
  
  // set enum to dst
  // static bool get(const std::string &name,T &dst)
  // static bool get(const char* &name,T &dst)
  // static bool get(std::string_view name,T &dst)
  // static const char* nameof(Access value)
};

}
} // end namespace nodamushi

#endif //NODAMUSHI_ENUM_HELPER_HPP
