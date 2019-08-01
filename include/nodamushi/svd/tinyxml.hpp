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
  
  
  tinyxml_svd_reader(const std::string& filename,bool throw_error=false):
      doc(),child(nullptr),_ok(false)
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


  using node   = ::tinyxml2::XMLNode;
  using text   = ::tinyxml2::XMLText;
  using element   = ::tinyxml2::XMLElement;
  using attribute = ::tinyxml2::XMLAttribute;
  using document  = ::tinyxml2::XMLDocument;

 
  struct sub_reader:public svd_reader
  {
    sub_reader(const element* e):
        attr(false),name(e->Name()),value(),
        attrs(e->FirstAttribute()),child(e->FirstChildElement())
    {
      for(const node* n = e->FirstChild();n!=nullptr;n = n->NextSibling()){
        const text* t = n->ToText();
        if(t){
          value = t->Value();
          svd_reader_util::trim(value);
        }
      }
    }
    sub_reader(const attribute* a):
        attr(true),name(a->Name()),value(a->Value()),
        attrs(nullptr),child(nullptr)
    {}
        
    //-------------------------------------------------------
    // svd_reader interface
    //-------------------------------------------------------
    bool is_attribute()const{return attr;}
    string_ref get_name()const{return name;}
    string_ref get_value()const{return value;}
    sub_reader next_child()
    {
      if(attrs!=nullptr){
        auto a = attrs;
        attrs = a->Next();
        return {a};
      }
      auto c= child;
      child = c->NextSiblingElement();
      return {c};
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
  };  


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
    return {c};
  }
  operator bool()const{return child != nullptr;}



 private:
  document doc;
  element* child;
  bool _ok;
};

}}
#endif
