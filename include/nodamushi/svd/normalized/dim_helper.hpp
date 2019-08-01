/*!
  @brief name%s,dispalyName%s->name0,displayName1
  @file nodamushi/svd/normalized/dim_helper.hpp
*/
#ifndef __NODAMUSHI_SVD_NORMALIZED_DIM_HELPER_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_DIM_HELPER_HPP__

# include "nodamushi/svd/dim_type.hpp"
# include "nodamushi/svd/normalized/dim_info.hpp"
# include "nodamushi/const_string.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{




//! @brief get dim size. when dim is empty return 1.
template<typename NODE>
size_t get_dim_count(const NODE& n){
  return n.dim && *n.dim!=0? *n.dim : 1;
}

/**
 * @brief replacing %s process support 
 */
template<typename STR>struct dim_name_helper
{
  const STR* str;
  const dimIndex* dimIndex;
  dim_type type;
  size_t first_end;
  size_t second_begin;

  template<typename NODE>
  dim_name_helper(const NODE& n,const STR* string):
      str(string),
      dimIndex(n.dimIndex? &n.dimIndex.get() : nullptr),
      type(dim_type::NON),
      first_end(0),
      second_begin(find_npos)
  {
    if(!string)return;
    const STR& s = *string;
    first_end = s.length();
    if(s.length() < 2) return;
    auto p = s.find("%s");

    // not array,seq
    if(p == find_npos)return;
      
    auto size = s.size();
    
    // not array
    if(s[size-1] != ']'){
      type = dim_type::SEQ;
      first_end = p;
      second_begin = p+2;
      return;
    }

    size_t i = size -2;
    // skip space
    while(i != 0 && (s[i] == ' ' || s[i] == '\t')) --i;
    
    // %s ?
    if(i < 3 || s[i-1] != '%' || s[i] != 's' ){
      // seq
      type = dim_type::SEQ;
      first_end = p;
      second_begin = p+2;
    }
    i-=2;// %s
    // skip space
    while(i != 0 && (s[i] == ' ' || s[i] == '\t')) --i;
    
    if(s[i] == '['){
      type = dim_type::ARRAY;
      first_end = i+1;
      second_begin = s.length()-1;
    }else{
      type = dim_type::SEQ;
      first_end = p;
      second_begin = p+2;
    }
  }
  operator bool()const noexcept{return type != dim_type::NON;}
  is_array()const noexcept{return type == dim_type::ARRAY;}
  is_seq()const noexcept{return type == dim_type::SEQ;}

  std::string operator()(size_t index)const{
    if(!str || first_end == 0)return "";
    if(type == dim_type::NON){
      return std::string(*str);// TODO
    }
    const std::string& n = *str;
    std::string s =  n.substr(0,first_end);

    if(type != dim_type::NON){
      if(dimIndex){
        const auto& dimi = *dimIndex;
        if(dimi.size() <= index) s.append(std::to_string(index));
        else                     s.append(dimi[index]);
      }else                      s.append(std::to_string(index));
      if(second_begin != find_npos)
        s.append(n,second_begin,n.length()-second_begin);
    }
    return s;
  }
};


template<typename NODE>
auto make_dim_name_helper(const NODE& n)
    ->dim_name_helper<typename decltype(n.name)::type>
{
  return {n,n.name? &n.name.get() : nullptr};
}

template<typename NODE>
auto make_dim_displayName_helper(const NODE& n)
    ->dim_name_helper<typename decltype(n.displayName)::type>
{
  return {n,n.displayName? &n.displayName.get() : nullptr};
}


template<typename NODE>
struct dim_helper
{
  using name_helper = decltype(make_dim_name_helper(*((NODE*)0)));
  const NODE& node;
  name_helper name;

  size_t size;
  size_t data_byte_size;
  
  dim_info get_base_info(){
    return {name.type,0,size,0};
  }
  
  dim_helper(const NODE& n):
      node(n),
      name(n, (n.name? &n.name.get() : nullptr) ),
      size(get_dim_count(n)),
      data_byte_size(
          (n.dimIncrement && n.dimIncrement.get() > 0)?n.dimIncrement.get() : 1)
  {}
  
  operator bool()const noexcept{return name.type != dim_type::NON;}
  is_array()const noexcept{return name.type == dim_type::ARRAY;}
  is_seq()const noexcept{return name.type == dim_type::SEQ;}

  size_t offset(size_t index)
  {
    return data_byte_size * index ;
  }

};

template<typename NODE>
inline dim_helper<NODE> make_dim_helper(const NODE& n){return dim_helper<NODE>(n);}



template<typename NODELIST>
size_t calcChildrenSize(const NODELIST& list)
{
  size_t count=0;
  for(const auto& f:list){
    if(f.dim && f.dim!=0) count += *f.dim;
    else                  count ++;
  }
  return count;
}




}}}// end namespace nodamushi

#endif // __NODAMUSHI_SVD_NORMALIZED_DIM_HELPER_HPP__
