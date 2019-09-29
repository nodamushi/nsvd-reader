/*!
  @brief value attriubte
  @file vattr.hpp
*/
#ifndef NODAMUSHI_SVD_VATTR_HPP
#define NODAMUSHI_SVD_VATTR_HPP

# include <tuple>
# include <type_traits>
# include <initializer_list>

namespace nodamushi{
namespace svd{

template<typename t,typename... types>struct contains
{
 private:
  static constexpr bool calc(){
    using swallow = std::initializer_list<int>;
    bool b = false;
    (void)swallow{(void(b |= std::is_same<t,types>::value),0)...};
    return b;
  }
 public:
  static constexpr bool value = calc();
};


template<typename value,typename... attributes>struct vattr{};

template<typename v>
struct vattr_unwrap
{
  using type = v;
  using attr = std::tuple<>;
  template<typename t>
  using contains = ::std::false_type; 
};

template<typename v,typename... attributes>
struct vattr_unwrap<vattr<v,attributes...>>
{
  using type = v;
  using attribute = std::tuple<attributes...>;
  template<typename t>
  using contains = ::nodamushi::svd::contains<t,attributes...>;
};


} // end namespace svd
} // end namespace nodamushi

// include attributes
#include "nodamushi/svd/vattr/hex.hpp"


#endif // NODAMUSHI_SVD_VATTR_HPP
