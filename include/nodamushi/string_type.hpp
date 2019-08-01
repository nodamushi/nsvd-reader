/*!
  @brief C++17/14 string type
  @file nodamushi/string_type.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_STRING_TYPE_HPP__
#define __NODAMUSHI_STRING_TYPE_HPP__

# include <cstdint>
# include <string>
# include <ostream>
# include <unordered_map>
# include <vector>
# if __cplusplus >= 201703
#  include <iterator>
#  include <string_view>
#  include <optional>
# endif

namespace nodamushi{

# if __cplusplus >= 201703
// C++17

using const_string = std::string_view;
using string_ref = std::string_view;
using substring  = std::string_view;
#define NODAMUSHI_CONSTEXPR_STRING constexpr std::string_view
constexpr auto find_npos = std::string_view::npos;
using str_mapkey = std::string_view;

template<typename ITR,typename ITR_END>
void emplace_back_string(std::vector<std::string_view>& v,ITR itr,ITR_END end)
{
  v.emplace_back(itr,std::distance(itr,end));
}
# else
// C++14

using const_string = const char*;
using string_ref = const std::string&;
using substring  = std::string;
#define NODAMUSHI_CONSTEXPR_STRING std::string
constexpr auto find_npos = std::string::npos;

using str_mapkey = std::string;
# endif
template<typename V>
using str_map = std::unordered_map<str_mapkey, V>;


template<typename ITR,typename ITR_END>
void emplace_back_string(std::vector<std::string>& v,ITR itr,ITR_END end)
{
  v.emplace_back(itr,end);
}


} // end namespace nodamushi

#endif // __NODAMUSHI_STRING_TYPE_HPP__


