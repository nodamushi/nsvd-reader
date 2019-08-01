/*!
  @brief helper for copy derivedFrom element
  @file nodamushi/svd/normalized/derived_from_helper.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef __NODAMUSHI_SVD_NORMALIZED_DERIVED_FROM_HELPER_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_DERIVED_FROM_HELPER_HPP__

# include <type_traits>
# include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

# include "nodamushi/svd/normalized/nullable.hpp"


namespace nodamushi{
namespace svd{
namespace normalized{

//-------------------------------------------------------------------------------

// name( svd.name.get() )
//! __NORMALIZED_DERIVED_FROM(name)
#define __NORMALIZED_DERIVED_FROM(name)\
  ___NORMALIZED_DERIVED_FROM(n,name)


// name( svd.name )
#define __NORMALIZED_DERIVED_FROM_MEMBER(name)\
  ___NORMALIZED_DERIVED_FROM_MEMBER(name,n)

#define __NORMALIZED_DERIVED_FROM_HELPER(name)\
  ___NORMALIZED_DERIVED_FROM_HELPER(name,n)


//-------------------------------------------------------------------------------
// Implements
//-------------------------------------------------------------------------------

template<typename TO>
struct derived_from_helper_cast
{
  template<typename VALUE>
  static TO cast_to(const VALUE* ptr)
  {
    if(ptr)return static_cast<TO>(ptr->get());
    return TO{};
  }

  template<typename VALUE,typename D>
  static TO cast_to(const VALUE* ptr,D d)
  {
    if(ptr)return static_cast<TO>(ptr->get());
    return static_cast<TO>(d);
  }
};

template<typename TO>
struct derived_from_helper_cast<nullable<TO>>
{
  template<typename VALUE>
  static nullable<TO> cast_to(const VALUE* ptr)
  {
    if(ptr)return {static_cast<TO>(ptr->get())};
    else   return {};
  }
  template<typename VALUE,typename D>
  static nullable<TO> cast_to(const VALUE* ptr,D d)
  {
    if(ptr)return {static_cast<TO>(ptr->get())};
    else   return {static_cast<TO>(d)};
  }
};


template<typename SVD,typename VALUE,VALUE SVD::* MEMBER>
struct derived_from_helper
{
  using type =typename std::remove_const<typename VALUE::type>::type;

  const VALUE* ptr;

  template<typename O>
  derived_from_helper(const O& o):
      ptr(nullptr)
  {
    const auto& v = o.svd.*MEMBER;
    if(v){
      ptr = &v;
    }else if(o.derivedFrom){
      const O *df = o.derivedFrom;
      while(df){
        const auto& s= df->svd;
        const auto& dv = s.*MEMBER;
        if(dv){
          ptr = &dv;
          return;
        }
        df = df->derivedFrom;
      }
    }
  }
  operator bool()const noexcept{return ptr!=nullptr;}
  auto operator*()const noexcept
      ->decltype(ptr->get())
  {
    return ptr->get();
  }

};

template<typename T,typename SVD,typename VALUE,VALUE SVD::* MEMBER>
auto cast_df_helper(derived_from_helper<SVD,VALUE,MEMBER>&& h){
  return derived_from_helper_cast<T>::cast_to(h.ptr);
}


template<typename T>struct unwrap_decltype
{
  using unref = typename std::remove_reference<T>::type;
  using type =typename std::remove_const<unref>::type;
};

#define ___NORMALIZED_DERIVED_FROM(o,name)                              \
  name(cast_df_helper<___NORMALIZED_DERIVED_FROM_FIELD_TYPE(name)>(     \
           ___NORMALIZED_DERIVED_FROM_TYPE(name,o){o}))                 \

#define ___NORMALIZED_DERIVED_FROM_FIELD_TYPE(name) \
  typename unwrap_decltype<decltype(this->name)>::type


#define ___NORMALIZED_DERIVED_FROM_TYPE(name,o)             \
  derived_from_helper<                                          \
              typename unwrap_decltype<decltype(o.svd)>::type,\
              typename unwrap_decltype<decltype(o.svd.name)>::type,\
              & unwrap_decltype<decltype(o.svd)>::type::name>

#define ___NORMALIZED_DERIVED_FROM_MEMBER(name,o)               \
  name(derived_from_helper<                              \
              typename unwrap_decltype<decltype(o.svd)>::type,\
              typename unwrap_decltype<decltype(o.svd.name)>::type,\
              & unwrap_decltype<decltype(o.svd)>::type::name>\
       {o})
#define ___NORMALIZED_DERIVED_FROM_HELPER(name,o) \
  derived_from_helper<                                   \
              typename unwrap_decltype<decltype(o.svd)>::type,\
              typename unwrap_decltype<decltype(o.svd.name)>::type,\
              & unwrap_decltype<decltype(o.svd)>::type::name>\
       name{o}

}}}// end namespace nodamushi::svd::normalized

#endif // __NODAMUSHI_SVD_NORMALIZED_DERIVED_FROM_HELPER_HPP__
