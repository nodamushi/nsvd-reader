/*!
  @brief svd element path
  @file path.hpp
*/
#ifndef __NODAMUSHI_SVD_PATH_HPP__
#define __NODAMUSHI_SVD_PATH_HPP__

# include <vector>
# include <iterator>
# include <algorithm>
# include "nodamushi/string_type.hpp"
# include "nodamushi/svd/value_setter_helper.hpp"
namespace nodamushi{
namespace svd {

template<typename STR>struct path;

namespace path_details {
constexpr int compare(char a,char b)
{
  if(a == b) return 0;
  const int a1 = a <= 'Z'?(int)a-(int)'A':(int)a -(int)'a';
  const int b1 = b <= 'Z'?(int)b-(int)'A':(int)b -(int)'a';
  return (a1 < b1)?-1:
      (a1 > b1)? 1:
      (a  < b )?-2:2;
}

constexpr int num(char c){
  return ('0'<=c && c <='9')? c-'0': -1;
}

template<typename S1,typename S2>
constexpr int compare(const S1& s1,const S2& s2)
{
  const size_t ss1 = s1.size(),ss2 = s2.size();
  int same = 0;
  size_t i1=0,i2=0;
  for(;i1 < ss1 && i2 < ss2;i1++,i2++){
    const char c1 = s1[i1],c2 = s2[i2];
    const int n1 = num(c1);
    const int n2 = num(c2);

    if(n1!=-1 && n2!=-1){
      constexpr size_t OVER_FLOW = ((size_t)~0) / 10;
      size_t num1=n1,count1=0;
      for(;i1+1<ss1;i1++,count1++){
        const int n = num(s1[i1+1]);
        if(n==-1)break;
        if(num1 >= OVER_FLOW)break; //TODO: big integer
        num1 = num1 * 10 + (size_t)n;
      }
        
      size_t num2=n2,count2=0;
      for(;i2+1<ss2;i2++,count2++){
        const int n = num(s2[i2+1]);
        if(n==-1)break;
        if(num2 >= OVER_FLOW)break; //TODO: big integer
        num2 = num2 * 10 + (size_t)n;
      }
      if(num1 < num2) return -1;
      else if(num1 > num2) return 1;

      if(count1!=count2)
        same = count1 < count2? -2: 2;

    }else if(n1!=-1){
      return -1;
    }else if(n2!=-1){
      return 1;
    }else{
      const int v = compare(c1,c2);
      if(v & 1) return v;
      if(v!=0)same = v;
    }
  }

  size_t rest1 = ss1-i1 , rest2 = ss2-i2;
  return same != 0&&rest1==0 && rest2==0? (same>>1):
      rest1 < rest2? -1: 
      rest1 ==rest2?  0: 1;
}
}// namespace path_details------------------

/**
 * @brief dot '.' separeted path.
 *
 @par
   compare rule:

  -  a == a
  -  a < b
  -  A < a
  -  ab < ac
  -  AB < ab
  -  ab < AC
  -  a0 < aa
  -  a0 < a1
  -  a1 < a2
  -  a2 < a10
  -  a00 < a10
  -  a00 < a1
  -  a0 < a00
  -  a00 < a0a
  -  a0a < a00a
  -  a0 < a0a
  -  a0a < a0b
  -  a0a < a1
  -  a < a.a

 */
template<typename STR=substring>struct path
{
  template<typename STR2> friend class path;

  path():names{}{};
  path(string_ref s):names(0)
  {
    size_t count =1;
    for(auto c :s)
      if(c == '.')
        count++;
    names.reserve(count);

    auto from = s.cbegin();
    auto i = from;
    auto end = s.cend();
    while(i!=end){
      auto c = *i;
      if(c == '.'){
        emplace_back_string(names,from,i);
        from = i+1;
      }
      ++i;
    }
    if(i != from)
      emplace_back_string(names,from,i);
  }
  path(const path<STR>& p,size_t from,size_t size):
      names(p.names.begin()+from,p.names.begin()+from+size){}
  path(const path<STR>& a,const path<STR>& b):names{}
  {
    names.reserve(a.names.size()+b.names.size());
    for(const auto& e:a)names.push_back(e);
    for(const auto& e:b)names.push_back(e);
  }
  path(const path<STR>& a,path<STR>&& b):names{}
  {
    names.reserve(a.names.size()+b.names.size());
    for(const auto& e:a)names.push_back(e);
    for(auto&& e:b)names.push_back(std::move(e));
    b.names.clear();
  }
  path(path<STR>&&) = default;
  template<typename STR2>
  path(const path<STR2>&p):names{}
  {
    names.reserve(p.size());
    for(const auto& s:p){
      names.push_back(s);
    }
  }

  path(const path<STR>&) = default;
  path<STR>& operator=(path<STR>&&)=default;
  path<STR>& operator=(const path<STR>&)=default;
  template<typename STR2>
  path<STR>& operator=(const path<STR2>&p)
  {
    names.clear();
    names.reserve(p.size());
    for(const auto& s:p){
      names.push_back(s);
    }
    return *this;
  }
  path<STR>& operator=(const std::string& s){
    operator=(path<STR>(s));
    return *this;
  }

# if __cplusplus >= 201703
  path<STR>& operator=(const std::string_view s){
    operator=(path<STR>(s));
    return *this;
  }
# endif
  path<STR>& add(const path<STR>& p)
  {
    size_t s = names.size();
    size_t s2 = s + p.size();
    names.reserve(s2);
    for(const auto& e:p.names)
      names.push_back(e);
    return *this;
  }
  path<STR>& add(path<STR>&& p)
  {
    size_t s = names.size();
    size_t s2 = s + p.size();
    names.reserve(s2);
    for(auto&& e:p.names)
      names.push_back(std::move(e));
    return *this;
  }
  path<STR>& add_name(string_ref p)
  {
    names.push_back(p);
    return *this;
  }
  
  path<STR> append(const path<STR>& a)const{return {*this,a};}
  path<STR> append(path<STR>&& a)const{return {*this,a};}
  path<STR> add(const path<STR>& a)const{return {*this,a};}
  path<STR> add(path<STR>&& a)const{return {*this,a};}

  string_ref operator[](size_t i)const{return names[i];}
  size_t size()const noexcept{return names.size();}
  bool single()const noexcept{return names.size()==1;}
  bool empty()const noexcept{return names.size()==0;}
  operator bool()const noexcept{return names.size()!=0;}
  path<STR> subpath(size_t from,size_t end=0)const{
    size_t s = (end==0) ? size() - from:end-from;
    return {*this,from,s};
  }

  auto begin()const{return names.begin();}
  auto end()const{return names.end();}
  auto cbegin()const{return names.cbegin();}
  auto cend()const{return names.cend();}
  auto begin(){return names.begin();}
  auto end(){return names.end();}

  template<typename S>
  bool equals(const path<S>& s)const
  {
    if((void*)this == (void*)&s)return true;
    if(s.size() != size())return false;
    for(size_t i = 0,e = size();i < e;i++){
      if(names[i] != s.names[i]){
        return false;
      }
    }
    return true;
  }

  template<typename S>
  bool operator!=(const path<S>& s)const
  {
    return !equals(s);
  }
  template<typename S>
  bool operator==(const path<S>& s)const
  {
    return equals(s);
  }

  bool operator==(string_ref s)const
  {
    path<> p(s);
    return equals(p);
  }
  bool operator!=(string_ref s)const
  {
    path<> p(s);
    return !equals(p);
  }
  
  /*!
    a == a
    a < b
    A < a
    ab < ac
    AB < ab
    ab < AC
    a0 < aa
    a0 < a1
    a1 < a2
    a2 < a10
    a00 < a10
    a00 < a1
    a0 < a00
    a00 < a0a
    a0a < a00a
    a0 < a0a
    a0a < a0b
    a0a < a1
    
    a < a.a
   */
  template<typename S>int compare(const path<S>& s)const noexcept{
    if(this == &s)return 0;
    const size_t st = size(),s1 = s.size();
    const size_t size = st<s1? st:s1;
    for(size_t i = 0;i<size;i++){
      const int n = path_details::compare(names[i],s.names[i]);
      if(n!=0)
        return n;
    }
    return st == s1 ? 0: st < s1? -1:1;
  }
  
  template<typename S>bool operator<(const path<S>& s)const noexcept{return compare(s) < 0;}
  template<typename S>bool operator>(const path<S>& s)const noexcept{return compare(s) > 0;}
  template<typename S>bool operator<=(const path<S>& s)const noexcept{return compare(s) <= 0;}
  template<typename S>bool operator>=(const path<S>& s)const noexcept{return compare(s) >= 0;}

 private:
  std::vector<STR> names;
};

template<typename STR>struct value_setter_helper<path<STR>>
{
  static constexpr bool value=true;
  static constexpr bool set(const std::string& src,path<STR>& dst){dst = src;return true;}
# if __cplusplus >= 201703
  static constexpr bool set(const std::string_view& src,path<STR>& dst){dst = src;return true;}
# endif
};


}  // svd
}// end namespace nodamushi

# include <ostream>

namespace std{
template<typename S>
ostream& operator <<(ostream& o,const ::nodamushi::svd::path<S>& p)
{
  for(size_t i = 0,n=p.size();i<n;i++){
    if(i!=0) o << '.';
    o << p[i];
  }
  return o;
}
}

#endif // __NODAMUSHI_SVD_PATH_HPP__
