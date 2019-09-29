/*!
  @brief value_setter imple
  @file nodamushi/svd/value/value_setter.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_VALUE_VALUE_SETTER_HPP
#define NODAMUSHI_SVD_VALUE_VALUE_SETTER_HPP

# include <type_traits>
# include <limits>
# include <vector>

# include "nodamushi/svd/enum_helper.hpp"
# include "nodamushi/svd/value_setter_helper.hpp"
# include "nodamushi/box.hpp"
# include "nodamushi/boxvec.hpp"
# include "nodamushi/svd/node.hpp"
# include "nodamushi/svd/create.hpp"

namespace nodamushi{
namespace svd{

template<typename DST,typename SRC>struct value_setter;
template<typename DST,typename SRC>
bool apply_value_setter(DST& dst,const SRC& src)
{
  return value_setter<DST,SRC>::set(dst,src);
}

//-------------------------------------------------------------------------
// value try set
//-------------------------------------------------------------------------
template<typename DST,typename SVD>
auto _value_try_set(DST& dst,SVD& src)
    ->typename std::enable_if<std::is_base_of<svd_node,DST>::value, bool>::type
{
  return create(src, dst);
}

template<typename DST,typename SVD>
auto _value_try_set(DST& dst,SVD& src)
    ->typename std::enable_if<!std::is_base_of<svd_node,DST>::value, bool>::type
{
  return apply_value_setter(dst,src.get_value());
}

template<typename DST,typename SVD>
bool value_try_set(DST& dst,SVD& src)
{
  return _value_try_set(dst,src);
}

template<typename DST,typename SVD>
bool value_try_set(std::vector<DST>& dst,SVD& src)
{
  size_t s = dst.size();
  dst.emplace_back();
  if(!value_try_set(dst[s],src)){
    dst.pop_back();
    return false;
  }
  return true;
}

template<typename DST,typename SVD>
bool value_try_set(std::vector<nodamushi::box<DST>>& dst,SVD& src)
{
  box<DST> tmp{new DST{}};
  DST& d = *tmp;
  if(value_try_set(d,src)){
    dst.emplace_back(std::move(tmp));
    return true;
  }
  return false;
}


template<typename DST,typename PTR,typename SVD>
bool value_try_set(::nodamushi::boxvec<DST,PTR>& dst,SVD& src)
{
  box<DST> tmp{new DST{}};
  DST& d = *tmp;
  if(value_try_set(d,src)){
    dst.push_back(std::move(tmp));
    return true;
  }
  return false;
}

template<typename DST,typename SVD>
bool value_try_set(nodamushi::box<DST>& dst,SVD& src)
{
  box<DST> tmp{new DST{}};
  DST& d = *tmp;
  if(value_try_set(d,src)){
    dst = std::move(tmp);
    return true;
  }
  return false;
}
//-------------------------------------------------------------------------

namespace details{

template<typename I,bool UNSIG>struct irange;
template<typename I>struct irange<I,true>
{
  using limits = std::numeric_limits<I>;
  static constexpr I MAX = limits::max();
  static bool in_range(unsigned long long v){return v <= MAX;}
};
template<typename I>struct irange<I,false>
{
  using limits = std::numeric_limits<I>;
  static constexpr I MAX = limits::max();
  static constexpr I MIN = limits::min();
  static bool in_range(long long v){return MIN <= v && v <= MAX;}
};

template<typename I>
bool cast_integer(const char* const ptr,size_t length,I& dst)
{
  constexpr  bool unsig = std::is_unsigned<I>::value;
  using ir = irange<I,unsig>;
  using LL = std::conditional_t<unsig, unsigned long long, long long>;
  char *str_end=nullptr;
  LL v = static_cast<LL>(std::strtoll(
      ptr,&str_end, 
      (length > 2 && *ptr == '0' && 
       (ptr[1] == 'x' || ptr[1] == 'X'))? 16:10));
  size_t use = str_end - ptr;
  if(!ir::in_range(v) || use != length){
    return false;
  }
  dst = static_cast<I>(v);
  return true;
}

template<typename I> bool cast_integer(const std::string& src,I& dst)
{
  return cast_integer(src.c_str(),src.length(),dst);
}

# if __cplusplus >= 201703
template<typename I> bool cast_integer(const std::string_view& s,I& dst)
{
  std::string src(s);
  return cast_integer(src.c_str() ,src.length(),dst);
}
# endif


template<typename DST>
auto value_setter_set(DST& dst,const std::string& src)
    -> typename std::enable_if<
      std::is_integral<DST>::value&&!enum_helper<DST>::HAS_HELPER
      &&!std::is_same<DST,bool>::value
      ,bool>::type
{
  bool b = details::cast_integer(src,dst);
  // TODO throw number format error ?
  return b;
}

template<typename DST>
auto value_setter_set(DST& dst,const std::string& src)
    -> typename std::enable_if<enum_helper<DST>::HAS_HELPER ,bool>::type
{
  bool b =enum_helper<DST>::get(src,dst);
  // TODO throw number format error ?
  return b;
}


template<typename DST>
auto value_setter_set(DST& dst,const std::string& src)
    -> typename std::enable_if<value_setter_helper<DST>::value ,bool>::type
{
  bool b =value_setter_helper<DST>::set(src,dst);
  return b;
}


template<typename DST>
bool value_setter_set(bool& dst,const std::string& src)
{
  if(src == "true" || src == "1"){
    dst = true;
    return true;
  }else if(src == "false" || src == "0"){
    dst = false;
    return true;
  }
  return false;
}

# if __cplusplus >= 201703


template<typename DST>
auto value_setter_set(DST& dst,std::string_view s)
    -> typename std::enable_if<
      std::is_integral<DST>::value&&!enum_helper<DST>::HAS_HELPER
      &&!std::is_same<DST,bool>::value
      ,bool>::type
{
  std::string src(s);
  bool b = details::cast_integer(src,dst);
  // TODO throw number format error ?
  return b;
}

template<typename DST>
auto value_setter_set(DST& dst,std::string_view s)
    -> typename std::enable_if<enum_helper<DST>::HAS_HELPER ,bool>::type
{
  std::string src(s);
  bool b =enum_helper<DST>::get(src,dst);
  // TODO throw number format error ?
  return b;
}

template<typename DST>
auto value_setter_set(DST& dst,std::string_view src)
    -> typename std::enable_if<value_setter_helper<DST>::value ,bool>::type
{
  bool b =value_setter_helper<DST>::set(src,dst);
  return b;
}

template<typename DST>
bool value_setter_set(bool& dst,std::string_view src)
{

  if(src == "true"){
    dst = true;
    return true;
  }else if(src == "false"){
    dst = false;
    return true;
  }
  return false;
}

# endif


} // end namespace details




template<typename T>struct value_setter<T,T>
{
  static bool set(T& dst,const T& src){
    dst = src;
    return true;
  }
  static bool move(T& dst,T&& src){dst =std::move(src);return true;}
};

template<typename DST>struct value_setter<DST,std::string>
{
  static bool set(DST& dst,const std::string& src)
  {
    return details::value_setter_set<DST>(dst,src);
  }
  
  static auto move(DST& dst, std::string&& src)
  {
    return details::value_setter_set<DST>(dst,src);
  }
};

template<>struct value_setter<std::string,std::string>
{
  static bool set(std::string& dst, const std::string& src)
  {
    dst = src;  return true;
  }

  static bool move(std::string& dst,std::string&& src)
  {
    dst = std::move(src);  return true;
  }
};


//-----------------------------------------------------------------------------
# if __cplusplus >= 201703

template<typename DST>struct value_setter<DST,std::string_view>
{
  static bool set(DST& dst,const std::string_view& src)
  {
    return details::value_setter_set<DST>(dst,src);
  }
  
  static auto move(DST& dst, std::string_view&& src)
  {
    return details::value_setter_set<DST>(dst,src);
  }
};

template<>struct value_setter<std::string_view,std::string_view>
{
  static bool set(std::string_view& dst, const std::string_view& src)
  {
    dst = src;  return true;
  }
  static bool move(std::string_view& dst, std::string_view&& src)
  {
    dst = src;  return true;
  }
};
template<>struct value_setter<std::string,std::string_view>
{
  static bool set(std::string& dst, const std::string_view& src)
  {
    dst = std::string(src);  return true;
  }
  static bool move(std::string& dst,std::string_view&& src)
  {
    dst = std::string(src);  return true;
  }
};

# endif


}} // end namespace nodamushi::svd


#endif // NODAMUSHI_SVD_VALUE_VALUE_SETTER_HPP
