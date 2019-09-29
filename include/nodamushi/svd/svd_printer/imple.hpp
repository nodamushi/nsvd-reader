/*!
  @brief svd printer imple
  @file nodamushi/svd/svd_printer/imple.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_SVD_PRINTER_IMPLE_HPP
#define NODAMUSHI_SVD_SVD_PRINTER_IMPLE_HPP

# include "nodamushi/box.hpp"
# include "nodamushi/boxvec.hpp"
# include "nodamushi/svd/svd_printer.hpp"
# include <sstream>
# include <ios>
# include <iomanip>
# include <type_traits>
namespace nodamushi{
namespace svd{

namespace details{  template<typename T>struct print_with_svd_escape;  }

template<typename T>
std::ostream& svd_printer<T>::print(
    std::ostream& o, const_string name, T const& v,int indent)
{
  o <<get_indent(indent)<<"<" <<name<<">";
  details::print_with_svd_escape<T>::print(o,v);
  o <<"</" << name << ">";
  return o;
}

// for void
template<>struct svd_printer<void>
{
  template<typename T>
  static std::ostream& print(std::ostream& o, const_string name, const T& v,int indent)
  {
    return o;
  }
};



// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
// svd_value_printer
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

namespace details{  

template<typename vtype> struct _hex_printer_elem
{
  static std::ostream& print(
      std::ostream& o, const_string name, vtype v,int indent)
  {
    o <<get_indent(indent)<<"<" <<name<<">";
    if(v){
      std::ios_base::fmtflags f( o.flags() );
      o <<"0x"<< std::hex << std::uppercase << v;
      o.flags( f );
    }else{
      o << v;
    }
    o <<"</" << name << ">";
    return o;
  }
};

template<typename vtype> struct _hex_printer_attr
{
  static std::ostream& print(std::ostream& o, vtype v)
  {
    std::ios_base::fmtflags f( o.flags() );
    o <<"0x"<< std::hex <<std::uppercase << v;
    o.flags( f );
    return o;
  }
};


template<typename T> struct _hex_printer_selector
{
  using u = ::nodamushi::svd::vattr_unwrap<T>;
  using is_hex_t = typename u::template contains<::nodamushi::svd::hex>;
  static constexpr bool is_hex = is_hex_t::value;
  using vtype = typename u::type;
  using type_a = typename std::conditional<
    is_hex,
    _hex_printer_attr<vtype>,
    details::print_with_svd_escape<T>>::type;
  using type_e = typename std::conditional<
    is_hex,
    _hex_printer_elem<vtype>,
    svd_printer<T>>::type;
};

template<typename T>
using select_printer_attr =  typename _hex_printer_selector<T>::type_a;

template<typename T>
using select_printer_elem =  typename _hex_printer_selector<T>::type_e;

}

// attribute
template<>struct svd_value_printer<true> 
{
  template<typename T,bool required,char... name>
  static std::ostream& print(
      std::ostream& o,
      const value<T,true,required,name...>& v,
      int indent)
  {
    if(v){
      o << std::string(indent,' ');
      print_const_string<name...>(o) << "=\"";
      details::select_printer_attr<T>::print(o,*v);
      o<<"\"";
    }
    return o;
  }
};


// ------------------------------------------------------------------------------

template<>struct svd_value_printer<false> // element
{
  template<typename T,bool required,char... name>
  static std::ostream& print(
      std::ostream& o,
      const value<T,false,required,name...>& v,
      int indent)
  {
    NODAMUSHI_LOCAL_CONST_STRING(name,text);
    return details::select_printer_elem<T>::print(o,text,*v,indent);
  }

  template<typename T,bool required,char... name>
  static std::ostream& print(
      std::ostream& o,
      const value<::nodamushi::box<T>,false,required,name...>& v,
      int indent)
  {
    NODAMUSHI_LOCAL_CONST_STRING(name,text);
    return details::select_printer_elem<T>::print(o,text,**v,indent);
  }

  template<typename T,bool required,char... name>
  static std::ostream& print(
      std::ostream& o,
      const value<std::vector<T>,false,required,name...>& v,
      int indent)
  {
    NODAMUSHI_LOCAL_CONST_STRING(name,text);
    bool first = true;
    for(const T& t:*v){
      if(first) first = false;
      else      o << std::endl;
      details::select_printer_elem<T>::print(o,text,t,indent);
    }
    return o;
  }

  template<typename T,bool required,char... name>
  static std::ostream& print(
      std::ostream& o,
      const value<std::vector<::nodamushi::box<T>>,false,required,name...>& v,
      int indent)
  {
    NODAMUSHI_LOCAL_CONST_STRING(name,text);
    bool first = true;
    for(const ::nodamushi::box<T>& t:*v){
      if(first) first = false;
      else      o << std::endl;
      details::select_printer_elem<T>::print(o,text,*t,indent);
    }
    return o;
  }


  template<typename T,typename PTR,bool required,char... name>
  static std::ostream& print(
      std::ostream& o,
      const value<::nodamushi::boxvec<T,PTR>,false,required,name...>& v,
      int indent)
  {
    NODAMUSHI_LOCAL_CONST_STRING(name,text);
    bool first = true;
    for(const T& t:*v){
      if(first) first = false;
      else      o << std::endl;
      details::select_printer_elem<T>::print(o,text,t,indent);
    }
    return o;
  }

};




// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
// print_with_svd_escape
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

namespace details{
template<bool b,typename T> struct print_with_svd_escape_helper;

template<typename T>struct print_with_svd_escape
{
  static constexpr bool simple_type = 
      (std::is_integral<T>::value 
       || std::is_floating_point<T>::value 
       || std::is_enum<T>::value);

  static void print(std::ostream& o,T const& v)
  {
    print_with_svd_escape_helper<simple_type,T>::print(o,v);
  }
};

template<>struct print_with_svd_escape<const char*>
{
  static constexpr bool svd_escape_charactor(char c)
  {
    return c == '"' || c == '\'' || c == '<' || c == '>' || c == '&';
  }
  
  static void print(std::ostream& o,const char* itr,const char* const end)
  {
    const char* p = itr;
    while(true){
      char c;
      bool e = itr == end || (c=*itr) == '\0';
      if(e || svd_escape_charactor(c)){
        if(p != itr) o.write(p,itr-p);
        if(e)  return;
        switch(c){
          case '"': o << "&quot;";
          case '\'':o << "&apos;";
          case '<': o << "&lt;";
          case '>': o << "&gt;";
          case '&': o << "&amp;";
        }
        p = ++itr;
      }else ++itr;
    }
  }

  static void print(std::ostream& o,const char * v)
  {
    print(o,v,nullptr);
  }
};

template<>struct print_with_svd_escape<bool>
{
  static void print(std::ostream& o,const bool& v)
  {
    o << (v? "true": "false");
  }
};

  
template<>struct print_with_svd_escape<std::string>
{
  static void print(std::ostream& o,std::string const& v)
  {
    print_with_svd_escape<const char*>::print(o,v.data(),v.data()+v.length());
  }
};

# if __cplusplus >= 201703
template<>struct print_with_svd_escape<std::string_view>
{
  static void print(std::ostream& o,std::string_view const& v)
  {
    print_with_svd_escape<const char*>::print(o,v.data(),v.data()+v.length());
  }
};
# endif


template<typename T> struct print_with_svd_escape_helper<true,T>
{
  static void print(std::ostream& o,T const& v)
  {
    o << v;
  }
};

template<typename T> struct print_with_svd_escape_helper<false,T>
{
  static void print(std::ostream& o,T const& v)
  {
    std::stringstream s;
    s << v;
    std::string str = s.str();
    print_with_svd_escape<const char*>::print(o,str.data(),str.data()+str.length());
  }
};


}// end namespace details
}// end namespace svd
}// end namespace nodamushi

#endif // NODAMUSHI_SVD_SVD_PRINTER_IMPLE_HPP
