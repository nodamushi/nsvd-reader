/*!
  @brief svd printer
  @file svd_printer.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_SVD_PRINTER_HPP
#define NODAMUSHI_SVD_SVD_PRINTER_HPP

# include <ostream>
# include <type_traits>
# include <vector>
# include <memory>


# include "nodamushi/svd/value.hpp"

namespace nodamushi{
namespace svd{

/**
 * svd(xml) printer interface
 */
template<typename T>struct svd_printer
{
  static std::ostream& print(std::ostream& o, const_string name, const T& v,int indent);
};


template<bool attr>struct svd_value_printer;

template<typename T,bool attribute,bool required,char... name>
std::ostream& print_svd_value(
    std::ostream& o,
    const value<T,attribute,required,name...>& v,
    int indent,bool endline=false)
{
  if(v){
    svd_value_printer<attribute>::print(o,v,indent);
    if(endline)o << std::endl;
  }
  return o;
}

template<size_t INDENT_BASE=2,char INDENT=' '>
std::string get_indent(int s)
{
  return std::string(s * INDENT_BASE,INDENT);
}

}} // end namespace sanken::svd

# include "nodamushi/svd/svd_printer/imple.hpp"

#endif // NODAMUSHI_SVD_SVD_PRINTER_HPP
