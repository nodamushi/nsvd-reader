/*!
  @brief SVD reader with Expat
  @par
  When you want to use this header,add Expat to include directories and link libraries.
  @file nodamushi/svd/expat.hpp
  @see https://github.com/libexpat/libexpat
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
/*
  Expat license is the same as the MIT/X Consortium license.
 */

#ifndef NODAMUSHI_SVD_EXPAT_HPP
#define NODAMUSHI_SVD_EXPAT_HPP

# include "nodamushi/svd/svd_reader.hpp"
# include "nodamushi/svd/svd_reader_stream_helper.hpp"
# include <expat.h>
# include <cstdio> 
namespace nodamushi{
namespace svd{


//-----------------------------------------------------------
// Expat parser handlers
//-----------------------------------------------------------
namespace expat_handler{

template<typename P>
void add_child(
    P* ptr, // expat_svd_reader* or stream_reader_element*
    const XML_Char *name,const XML_Char **attributes)
{
  auto& e = ptr->add_child(std::string(name));
  while(attributes!=nullptr){
    const XML_Char* n = *attributes;
    if(n == nullptr) break;
    attributes++;
    const XML_Char* v = *attributes;
    if(v == nullptr) break;
    attributes++;
    e.add_attribute(std::string(n),std::string(v));
  }
}

template<typename UD>
void XMLCALL element_start(
    void *user_data, // UD*
    const XML_Char *name,const XML_Char **attributes)
{
  UD* ptr= reinterpret_cast<UD*>(user_data);
  auto* c = ptr->get_active_open_element();
  if(c == nullptr)add_child(ptr,name,attributes);
  else            add_child(c,name,attributes);
}

template<typename UD>
void XMLCALL element_end(
    void *user_data,// UD*
    const XML_Char *name)
{
  UD* ptr= reinterpret_cast<UD*>(user_data);
  auto* c = ptr->get_active_open_element();
  if(c!=nullptr){
    //TODO name check
    c->close();
  }
}


template<typename UD>
void XMLCALL element_char(
    void *user_data,// UD*
    const XML_Char *data,int len)
{
  UD* ptr= reinterpret_cast<UD*>(user_data);
  auto* c = ptr->get_active_open_element();
  if(c!=nullptr)
    c->add_value(data,(size_t)len);
}


}//end  expat_handler ------------------------------------


/**
 * @brief SVD reader with Expat(https://github.com/libexpat/libexpat)
 * @par
  When you want to use this header,add Expat to include directories and link libraries.
 *
 @code
 nodamushi::svd::expat_svd_reader r(filename);
 if(!r.ok())return; // file open failed
 nodamushi::svd::Device<> device(r);
 if(!r.ok())return; // xml parser error
 @endcode
 *
 * @see https://github.com/libexpat/libexpat
 */
struct expat_svd_reader:public stream_reader_base<expat_svd_reader>
{
  
  static constexpr size_t BUFFER_SIZE=1024;

  expat_svd_reader(const std::string& file_name,bool throw_error=false):
      parser(),
      file(nullptr),
      enable(false),
      eof(true),
      _ok(false),
      ethrow(throw_error),
      fname(throw_error?file_name:"")
  {
    if(file_read_exception::check(file_name,throw_error)){
      file = std::fopen(file_name.data(),"r");
    }
    if(file){
      parser = XML_ParserCreate( NULL );
      XML_SetElementHandler(parser,
                            expat_handler::element_start<expat_svd_reader>,
                            expat_handler::element_end<expat_svd_reader>);
      XML_SetCharacterDataHandler(parser,expat_handler::element_char<expat_svd_reader>);
      XML_SetUserData(parser,this);
      enable = true;
      eof = false;
      _ok = true;
    }
  }

  /**
   * @return file open / xml parse status
   */
  bool ok()const {return _ok;}

  expat_svd_reader(expat_svd_reader&& d):
      stream_reader_base(std::move(d)),
      parser(std::move(d.parser)),
      file(d.file),
      enable(d.enable),
      eof(d.eof)
  {
    d.enable = false;
    XML_SetUserData(parser,this);
  }

  ~expat_svd_reader(){close();}
  
  expat_svd_reader(const expat_svd_reader&)=delete;
  expat_svd_reader& operator=(const expat_svd_reader&)=delete;
  expat_svd_reader& operator=(expat_svd_reader&&)=delete;

  void close()
  {
    if(enable){
      enable = false;
      std::fclose(file);
      file = nullptr;
      XML_ParserFree(parser);
    }
  }  

  //-------------------------------------------------------------
  // stream_reader_base interface
  //-------------------------------------------------------------
  //! svd_reader_base interface function
  void read()
  {
    if(enable && !eof){
      char buffer[BUFFER_SIZE];
      size_t len = std::fread(buffer,sizeof(char),BUFFER_SIZE,file);
      eof = len != BUFFER_SIZE;
      if(XML_Parse(parser,buffer,len,eof) == XML_STATUS_ERROR){
        _ok = false;
        eof = true;
        if(ethrow){
          throw xml_parse_exception::make(fname);
        }
      }
    }
    if(eof)close_children();
  }
  //! svd_reader_base interface function
  bool is_end(){ return eof;}

 private:  
  XML_Parser parser;
  std::FILE* file;
  bool enable;
  bool eof;
  bool _ok;
  bool ethrow;
  std::string fname;
};


}
}// end namespace nodamushi

#endif // NODAMUSHI_SVD_EXPAT_HPP
