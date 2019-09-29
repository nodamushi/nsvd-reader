/*!
  @brief svd_reader 
  @file nodamushi/svd/svd_reader.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_SVD_READER_HPP
#define NODAMUSHI_SVD_SVD_READER_HPP

# include "nodamushi/const_string.hpp"
# include "nodamushi/boxvec.hpp"
# include <cassert>
# include <type_traits>
# include <utility>
# include <stdexcept>

# include <fstream>


namespace nodamushi{
namespace svd{


enum class svd_error
{
  //! unknown element
  UNKNOWN_ELEMENT, 
  //! illegal value
  ILLEGAL_VALUE
};

/**
 * @brief Indicates which element caused an error in processing.
 */
enum class svd_element
{
  //! @brief nodamushi::svd::Device
  Device,
  //! @brief nodamushi::svd::Peripheral
  Peripheral,
  //! @brief nodamushi::svd::Register
  Register,
  //! @brief nodamushi::svd::Registers
  Registers,
  //! @brief nodamushi::svd::Cluster
  Cluster,
  //! @brief nodamushi::svd::Field
  Field,
  //! @brief nodamushi::svd::Fields
  Fields,
  //! @brief nodamushi::svd::EnumeratedValue
  EnumeratedValue,
  //! @brief nodamushi::svd::DimArrayIndex
  DimArrayIndex,
  //! @brief nodamushi::svd::WriteConstraintRange
  WriteConstraintRange,
  //! @brief nodamushi::svd::WriteConstraint
  WriteConstraint,
  //! @brief nodamushi::svd::Enumeration
  Enumeration,
  //! @brief nodamushi::svd::Interrupt
  Interrupt,
  //! @brief nodamushi::svd::AddressBlock
  AddressBlock,
  //! @brief nodamushi::svd::SAURegionsConfig
  SAURegionsConfig,
  //! @brief nodamushi::svd::SAURegionsConfigRegion
  SAURegionsConfigRegion,
  //! @brief nodamushi::svd::Cpu
  Cpu,
};

/*!
  svd read marker interface.

  implements interface
@code
 struct svd_reader
 {
   //!  if this element is svd attribute group,return true.
   bool is_attribute()const;
   //!  get this element name
   string_ref get_name()const;
   //!  get trimed value
   string_ref get_value();
   //!  get next child element
   svd_reader next_child();
   //!  @return has next child
   operator bool();
   
   // error handler
   void unknown_element(svd_element e);
   void illegal_value(svd_element e);
 };
@endcode
 */
struct svd_reader{};


/**
 * @brief file open ,read error
 */
struct file_read_exception:public std::invalid_argument
{
  static bool check(const std::string& file_name,bool throw_exception)
  {
    using namespace std;
    ifstream ifs(file_name);
    if(!ifs.good()){
      if(throw_exception){
        string m = "File read error : "s + file_name;
        throw file_read_exception(file_name,m);
      }
      return false;
    }
    return true;
  }

  file_read_exception(const std::string& file,const std::string& message):
      invalid_argument(message),
      file_name(file){}
  std::string file_name;
};

/**
 * @brief xml parse error
 */
struct xml_parse_exception:public std::runtime_error
{
  static constexpr size_t UNKNOWN_FILE_LINE= ~(size_t)0;
  static xml_parse_exception make(
      const std::string& file,size_t file_line = UNKNOWN_FILE_LINE)
  {
    using namespace std;
    string m = "XML parse error occured: "s + file;
    return {file,m,file_line};
  }
  xml_parse_exception(const std::string& file,
                      const std::string& message,
                      size_t file_line):
      runtime_error(message),
      file_name(file),
      line_number(file_line){}
  std::string file_name;
  size_t line_number;
};




struct svd_reader_util
{
  template<typename T>
  static std::pair<size_t,size_t> _trim(T& str)
  {
    size_t b = 0;
    const size_t len = str.length();
    size_t e = len;
    while(b!=e){
      char c = str[b];
      if(c == ' ' || c == '\t' || c =='\r' || c == '\n')
        b++;
      else 
        break;
    }
    while(b!=e){
      char c = str[e-1];
      if(c == ' ' || c == '\t' || c =='\r' || c == '\n')
        e--;
      else 
        break;
    }
    return {b,e};
  }
  //!  @brief remove white spaces
  static void trim(std::string& str)
  {
    auto v = _trim(str);
    size_t b = v.first,e=v.second;
    if(e != str.length())
      str.erase(e);
    if(b)
      str.erase(0,b);
  }

# if __cplusplus >= 201703
  //!  @brief remove white spaces
  static void trim(std::string_view& str)
  {
    auto v = _trim(str);
    size_t b = v.first,e=v.second,len = e-b;
    std::string_view n(str.data()+b,len);
    str = n;
  }

  //!  @brief remove white spaces
  static std::string_view trimc(const std::string_view& str)
  {
    auto v = _trim(str);
    size_t b = v.first,e=v.second,len = e-b;
    return {str.data()+b,len};
  }
  //!  @brief remove white spaces
  static std::string_view trimc(const std::string& str)
  {
    auto v = _trim(str);
    size_t b = v.first,e=v.second,len = e-b;
    return {str.data()+b,len};
  }
# else
  //!  @brief remove white spaces
  static std::string trimc(const std::string& str)
  {
    auto v = _trim(str);
    size_t b = v.first,e=v.second,len = e-b;
    return std::string(str,b,len);
  }
# endif

};


}}// end namespace nodamushi::svd

#endif // NODAMUSHI_SVD_SVD_READER_HPP
