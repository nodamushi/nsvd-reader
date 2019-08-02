/*!
  @brief SVD reader with TinyXML-2
  When you want to use this header,add TinyXML2 to include directories and link libraries.
  @file nodamushi/svd/tinyxml.hpp
  @see http://www.grinninglizard.com/tinyxml2/
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
/*
 * TinyXML-2 license is zlib license.
 */
#ifndef __NODAMUSHI_SVD_TINYXML_HPP__
#define __NODAMUSHI_SVD_TINYXML_HPP__

# include "nodamushi/svd/svd_reader.hpp"
# include <tinyxml2.h>
# include <cstdio> 

namespace nodamushi{
namespace svd{


/**
 * SVD reader with TinyXML-2k.
 * @par
  When you want to use this header,add TineyXML2 to include directories and link libraries.
 *
 @code
 nodamushi::svd::tinyxml_svd_reader r(filename);
 if(!r.ok())return; // file open failed
 nodamushi::svd::Device<> device(r);
 if(!r.ok())return; // xml parser error
 @endcode
 *
 * @see https://github.com/libexpat/libexpat
 */
struct tinyxml_svd_reader:public svd_reader
{
  struct sub_reader;
  using error_handler = std::function<void(svd_error,svd_element,sub_reader&)>;
  using document  = ::tinyxml2::XMLDocument;
  using node   = ::tinyxml2::XMLNode;
  using text   = ::tinyxml2::XMLText;
  using element   = ::tinyxml2::XMLElement;
  using attribute = ::tinyxml2::XMLAttribute;


  
  tinyxml_svd_reader(const std::string& filename,bool throw_error=false):
      doc(),child(nullptr),_ok(false),ehandler()
  {
    if(file_read_exception::check(filename,throw_error)){
      if(doc.LoadFile(filename.data()) == tinyxml2::XML_SUCCESS){
        child = doc.FirstChildElement();
        _ok = true;
      }else if(throw_error){
        throw xml_parse_exception::make(filename);
      }
    }
  }

  /**
   * @return file open / xml parse status
   */
  bool ok()const {return _ok;}


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




  //-------------------------------------------------------
  // svd_reader interface
  //-------------------------------------------------------
  bool is_attribute()const{return false;}
# if __cplusplus >= 201703
  std::string_view get_name()const{return std::string_view{};}
  std::string_view get_value()const{return std::string_view{};}
# else
  std::string get_name()const{return std::string{};}
  std::string get_value()const{return std::string{};}
# endif
  sub_reader next_child()
  {
    auto* c = child;
    child = child->NextSiblingElement();
    return {c,*this};
  }
  operator bool()const{return child != nullptr;}
  void unknown_element(svd_element e)const noexcept{} // root skip
  void illegal_value(svd_element e)const noexcept{} // root skip
  void handle_error(svd_error e,svd_element elem,sub_reader& r)const
  {
    if(ehandler) ehandler(e,elem,r);
  }


  struct sub_reader:public svd_reader
  {
    
    sub_reader(const element* e,tinyxml_svd_reader& r):
        attr(false),name(e->Name()),value(),
        attrs(e->FirstAttribute()),child(e->FirstChildElement()),
        root(r)
    {
      for(const node* n = e->FirstChild();n!=nullptr;n = n->NextSibling()){
        const text* t = n->ToText();
        if(t){
          value = t->Value();
          svd_reader_util::trim(value);
        }
      }
    }
    sub_reader(const attribute* a,tinyxml_svd_reader& r):
        attr(true),name(a->Name()),value(a->Value()),
        attrs(nullptr),child(nullptr),
        root(r)
    {}
        
    //-------------------------------------------------------
    // svd_reader interface
    //-------------------------------------------------------
    bool is_attribute()const{return attr;}
    string_ref get_name()const{return name;}
    string_ref get_value()const{return value;}
    void unknown_element(svd_element e)
    {
      root.handle_error(svd_error::UNKNOWN_ELEMENT,e,*this);
    }

    void illegal_value(svd_element e)
    {
      root.handle_error(svd_error::UNKNOWN_ELEMENT,e,*this);
    }

    sub_reader next_child()
    {
      if(attrs!=nullptr){
        auto a = attrs;
        attrs = a->Next();
        return {a,root};
      }
      auto c= child;
      child = c->NextSiblingElement();
      return {c,root};
    }
    operator bool()const{
      return attrs != nullptr || child != nullptr;
    }

   private:
    bool attr;
# if __cplusplus >= 201703
    std::string_view name;
    std::string_view value;
# else
    std::string name;
    std::string value;
# endif
    const attribute* attrs;
    const element* child;
    tinyxml_svd_reader& root;
  };// end sub_reader

 private:
  document doc;
  element* child;
  bool _ok;
  error_handler ehandler;
};

}}
#endif
