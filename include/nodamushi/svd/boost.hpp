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
# include <functional>
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
  struct sub_reader;// sub_reader declaration
  using error_handler = std::function<void(svd_error,svd_element,sub_reader&)>;
  
  

  /**
   * @brief constructor with file name
   * @param file_name filename
   * @param throw_error  If this option is true,  throw exception when file open failed / xml error found.
   * @throw nodamushi::svd::file_read_exception 
   * @throw nodamushi::svd::xml_parser_error
   */
  boost_svd_reader(const std::string& file_name,bool throw_error=false):
      tree(),it(),end(it),_ok(false),ehandler()
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
      tree(),it(),end(it),_ok(false),ehandler()
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
   * @brief constructor with boost::property_tree::ptree
   */
  boost_svd_reader(ptree& t):
      tree(t),it(tree.begin()),end(tree.end()),_ok(true),ehandler()
  {
    skip_comment();
  }
  /**
   * @brief set error handler
   * handler called when an unknwon element found or an illegal value found.
   * @param e handler( void(svd_error,svd_element,sub_reader&))
   */
  void set_error_handler(error_handler& e)
  {
    ehandler = e;
  }
  error_handler& get_handler(){return ehandler;}

  /**
   * @brief file open / xml parse status
   * @return file open / xml parse status
   */
  bool ok()const noexcept{return _ok;}

  //-------------------------------------------------------
  // svd_reader interface
  //-------------------------------------------------------

  //! svd_reader interface function
  constexpr bool is_attribute()const noexcept{return false;}
# if __cplusplus >= 201703
  constexpr std::string_view get_name()const noexcept{return std::string_view{};}
  constexpr std::string_view get_value()const noexcept{return std::string_view{};}
# else
  std::string get_name()const noexcept{return std::string{};}
  std::string get_value()const noexcept{return std::string{};}
# endif
  //! svd_reader interface function
  operator bool()const noexcept{return it != end;}

  //! svd_reader interface function
  sub_reader next_child() noexcept
  {
    auto v = * it;
    ++it;
    skip_comment();
    auto& name = v.first;
    auto& subtree = v.second;
    auto& value = v.second.data();
    return sub_reader(subtree,name,value,false,*this);
  }
  
  void unknown_element(svd_element e)const noexcept{} // root skip
  void illegal_value(svd_element e)const noexcept{} // root skip
  void handle_error(svd_error e,svd_element elem,sub_reader& r)const
  {
    if(ehandler) ehandler(e,elem,r);
  }


  /**
   * @brief sub boost reader
   */
  struct sub_reader:public svd_reader
  {
    sub_reader(ptree& t,string_ref n,string_ref v,bool a,boost_svd_reader& r):
        tree(t.get_child("")),name(n),value(v),
        it(tree.begin()),end(tree.end()),
        attr(a||n == "<xmlattr>"),root(r)
    {
      boost::algorithm::trim(value);
      skip_comment();
    }
    bool is_attribute()const noexcept{return attr;}
    string_ref get_name()const noexcept{return name;}
    string_ref get_value()const noexcept{return value;}
    operator bool()const noexcept{return it != end;}

    void unknown_element(svd_element e)
    {
      root.handle_error(svd_error::UNKNOWN_ELEMENT,e,*this);
    }

    void illegal_value(svd_element e)
    {
      root.handle_error(svd_error::UNKNOWN_ELEMENT,e,*this);
    }

    sub_reader next_child() noexcept
    {
      auto v = * it;
      ++it;
      skip_comment();
      auto& name = v.first;
      auto& subtree = v.second;
      auto& value = v.second.data();
      return sub_reader(subtree,name,value,attr,root);
    }
   private:
    void skip_comment()
    {
      while(it != end && it->first == "<xmlcomment>")++it;
    }
    ptree tree;
    std::string name;
    std::string value;
    itr it,end;
    bool attr;
    boost_svd_reader& root;
  };


  


 private:
  void skip_comment() noexcept
  {
    while(it != end && it->first == "<xmlcomment>")++it;
  }
  ptree tree;
  itr it,end;
  bool _ok;
  error_handler ehandler;
};

}}// end namespace nodamushi::svd

#endif // __NODAMUSHI_SVD_BOOST_HPP__
