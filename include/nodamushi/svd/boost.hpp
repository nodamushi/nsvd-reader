/*!
  @brief SVD reader with Boost property tree
  when you use this header,you must add Boost library to include directories.
  @file nodamushi/svd/boost.hpp
  @see https://www.boost.org/doc/libs/1_65_1/doc/html/property_tree.html
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_BOOST_HPP__
#define __NODAMUSHI_SVD_BOOST_HPP__

# include <string>
# include <istream>
# include <boost/property_tree/ptree.hpp>
# include <boost/property_tree/xml_parser.hpp>
# include <boost/algorithm/string/trim.hpp>
# include "nodamushi/svd/svd_reader.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief SVD reader with Boost property tree.
 * @par
 * When you use this reader,you must add Boost library to include directories.
 * note: Boost property tree is a header only library.
 *
 @code
 nodamushi::svd::boost_svd_reader r(filename);
 if(!r.ok())return; // xml read failed
 nodamushi::svd::Device<> device(r);
 @endcode
 *
 * @see https://www.boost.org/doc/libs/1_65_1/doc/html/property_tree.html
 */
struct boost_svd_reader:public svd_reader
{
  using ptree = boost::property_tree::ptree;
  using itr   = ptree::iterator;
  
  boost_svd_reader()=default;

  boost_svd_reader(ptree& t):
      tree(t.get_child("")),name("root"),value(""),
      it(tree.begin()),end(tree.end()),attr(false),_ok(true)
  {
    skip_comment();
  }

  boost_svd_reader(ptree& t,string_ref n,string_ref v,bool a):
      tree(t.get_child("")),name(n),value(v),
      it(tree.begin()),end(tree.end()),attr(a),_ok(true)
  {
    boost::algorithm::trim(value);
    skip_comment();
  }
  /**
   * @brief constructor with file name
   * @param file_name filename
   * @param throw_error  If this option is true,  throw exception when file open failed / xml error found.
   * @throw nodamushi::svd::file_read_exception 
   * @throw nodamushi::svd::xml_parser_error
   */
  boost_svd_reader(const std::string& file_name,bool throw_error=false):
      tree(),name(""),value(""),
      it(),end(it),attr(false),_ok(false)
  {
    if(file_read_exception::check(file_name,throw_error)){
      try{
        boost::property_tree::read_xml(file_name,tree);
        _ok = true;
      }catch(boost::property_tree::xml_parser::xml_parser_error& e){
        if(throw_error){
          std::string m = e.message();
          throw xml_parse_exception(file_name,m,e.line());
        }
      }
      if(_ok){
        it=tree.begin();
        end=tree.end();
        skip_comment();
      }
    }
  }


  /**
   * @brief constructor with std::istream
   * @param file file contents istream
   * @param throw_error  If this option is true,  throw exception when file open failed / xml error found.
   * @throw nodamushi::svd::file_read_exception 
   * @throw nodamushi::svd::xml_parser_error
   */
  boost_svd_reader(std::istream& file,bool throw_error=false):
      tree(),name(""),value(""),
      it(),end(it),attr(false),_ok(false)
  {
    try{
      boost::property_tree::xml_parser::read_xml(file,tree);
      _ok = true;
    }catch(boost::property_tree::xml_parser::xml_parser_error& e){
      if(throw_error){
        std::string m = e.message();
        throw xml_parse_exception("stream",m,e.line());
      }
    }
    if(_ok){
      it=tree.begin();
      end=tree.end();
      skip_comment();
    }
  }

  /**
   * @brief file open / xml parse status
   * @return file open / xml parse status
   */
  bool ok()const noexcept{return _ok;}

  //-------------------------------------------------------
  // svd_reader interface
  //-------------------------------------------------------
  //! svd_reader interface function
  bool is_attribute()const noexcept{return attr;}
  //! svd_reader interface function
  string_ref get_name()const noexcept{return name;}
  //! svd_reader interface function
  string_ref get_value()const noexcept{return value;}
  //! svd_reader interface function
  boost_svd_reader next_child() noexcept
  {
    auto v = * it;
    ++it;
    skip_comment();
    auto& name = v.first;
    auto& subtree = v.second;
    auto& value = v.second.data();
    bool a= attr || name == "<xmlattr>";
    return boost_svd_reader(subtree,name,value,a);
  }
  //! svd_reader interface function
  operator bool()const{return it != end;}

 private:
  void skip_comment()
  {
    while(it != end && it->first == "<xmlcomment>"){
      ++it;
    }
  }
  ptree tree;
  std::string name;
  std::string value;
  itr it,end;
  bool attr;
  bool _ok;
};

}}// end namespace nodamushi::svd

#endif // __NODAMUSHI_SVD_BOOST_HPP__
