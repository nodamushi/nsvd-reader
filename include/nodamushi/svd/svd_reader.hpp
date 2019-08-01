/*!
  @brief svd_reader 
  @file nodamushi/svd/svd_reader.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_SVD_READER_HPP__
#define __NODAMUSHI_SVD_SVD_READER_HPP__

# include "nodamushi/const_string.hpp"
# include "nodamushi/boxvec.hpp"
# include <cassert>
# include <type_traits>
# include <utility>
# include <stdexcept>

# include <fstream>


namespace nodamushi{
namespace svd{


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


/** 
 * @brief XML element class for stream type parser
 * @param CallBack callback function to create values.
 *   when get_value() is called,CallBack will be called.
 */
template<typename CallBack>
struct stream_reader_element:public svd_reader
{ 
  stream_reader_element(const std::string& n,CallBack* f):
      name(n),attribute(false),value(),
      children{},closed(false),call_back(f),remove(false){}

  stream_reader_element(std::string&& n,CallBack* f):
      name(std::move(n)),attribute(false),value(),
      children{},closed(false),call_back(f),remove(false){}

  // ------------ svd_reader interface  -----------------
  bool is_attribute()const{return attribute;}
  string_ref get_name()const{return name;}
  string_ref get_value(){
    if(!closed && call_back!=nullptr){
      while(!closed)
        (*call_back)(*this);
    }
    return value;
  }
  stream_reader_element& next_child(){
    remove = true;
    return children[0];
  }
  operator bool(){
    if(remove){
      if(children.size()!=0)
        children.erase(children.begin());
      remove = false;
    }
    if(call_back!=nullptr){
      while(!closed && children.empty())
        (*call_back)(*this);
    }
    return children.size()!=0;
  }
  //--------------------------------------------------
  
  /**
   * reset callback object
   */
  void set_callback(CallBack* cb)
  {
    call_back=cb;
    for(auto& c:children)
      c.set_callback(cb);
  }

  /**
   * Close this tag and trim this contents text.
   */
  void close(){
    if(!closed){
      svd_reader_util::trim(value);
      closed = true;
      for(auto& c:children)
        c.close();
    }
  }
  
  /**
   * set constents text.
   */
  void set_value(const std::string& s){
    value = s;
  }
  /**
   * add constents text.
   */
  void add_value(const std::string& s){
    value.append(s);
  }
  /**
   * add constents text.
   */  
  void add_value(const char* s,size_t size){
    value.append(s,size);
  }
  /**
   * add attribute data
   */
  void add_attribute(const std::string& name,const std::string& value)
  {
    children.emplace_back(name,value,true,call_back);
  }
  /**
   * add attribute data
   */
  void add_attribute(std::string&& name,std::string&& value)
  {
    children.emplace_back(std::move(name),std::move(value),true,call_back);
  }

  /**
   * add child
   */
  stream_reader_element& add_child(std::string&& name)
  {
    size_t s = children.size();
    children.emplace_back(std::move(name),call_back);
    return children[s];
  }
  /**
   * add child
   */
  stream_reader_element& add_child(const std::string& name)
  {
    size_t s = children.size();
    children.emplace_back(name,call_back);
    return children[s];
  }

  /**
   * get current target element.
   */
  stream_reader_element* get_active_open_element()
  {
    if(closed)return nullptr;
    if(children.empty())return this;
    stream_reader_element& e = children[children.size()-1];
    if(e.closed)return this;
    stream_reader_element * p=e.get_active_open_element();
    return p == nullptr? &e:p;
  }
  
  constexpr bool is_closed()const{return closed;}



  
  stream_reader_element(const std::string& n,
                        const std::string& val,
                        bool attr,
                        CallBack* f):
      name(n),attribute(attr),value(val),
      children{},closed(attr),call_back(f),remove(false)
  {
    svd_reader_util::trim(value);
  }

  stream_reader_element(
      std::string&& n,
      std::string&& val,
      bool attr,
      CallBack* f):
      name(std::move(n)),attribute(attr),value(std::move(val)),
      children{},closed(attr),call_back(f),remove(false)
  {
    svd_reader_util::trim(value);
  }
 private:
  // Don't copy and move
  stream_reader_element(const stream_reader_element&)=delete;
  stream_reader_element& operator=(const stream_reader_element&)=delete;
  stream_reader_element(stream_reader_element&&d)=delete;
  stream_reader_element& operator=(stream_reader_element&&)=delete;

  std::string name;
  bool attribute;
  std::string value;
  ::nodamushi::boxvec<stream_reader_element> children;
  bool closed;
  CallBack* call_back;
  bool remove; //!< flag of remove first child
};


/**
 * @brief svd_reader base class for stream type parser
 * subclasses must be implements read method and is_end method
 * @code
 * void read();  // create children
 * bool is_end();// the xml data is finished
 * @endcode
 * @param SubClass subclass type
 * @see nodamushi::svd::expat_svd_reader
 */
template<typename SubClass>
struct stream_reader_base:public svd_reader
{
  using el = stream_reader_element<stream_reader_base>;
  
  stream_reader_base(): children{},remove(false),_active_cache(nullptr)
  {
    static_assert(std::is_base_of<stream_reader_base<SubClass>,SubClass>::value,
                  "stream_reader_base is base of SubClass");
  }
  virtual ~stream_reader_base()=default;

  // move
  stream_reader_base(stream_reader_base&& d):
      children(std::move(d.children)),
      remove(d.remove),_active_cache(nullptr)
  {
    for(auto & c:children) c.set_callback(this);
  }
  stream_reader_base& operator=(stream_reader_base&&d)
  {
    children = std::move(d.children);
    remove = d.remove;
    _active_cache=nullptr;
    for(auto & c:children) c.set_callback(this);
  }

  // Don't copy
  stream_reader_base(const stream_reader_base&) = delete;
  stream_reader_base& operator=(const stream_reader_base&)=delete;

  
  // ------------ svd_reader interface  -----------------
  bool is_attribute()const{return false;}
# if __cplusplus >= 201703
  std::string_view get_name()const{return std::string_view{};}
  std::string_view get_value()const{return std::string_view{};}
# else
  std::string get_name()const{return std::string{};}
  std::string get_value()const{return std::string{};}
# endif
  el& next_child(){
    remove = true;
    return children[0];
  }
  operator bool(){
    if(remove){
      if(!children.empty())
        children.erase(children.begin());
      remove = false;
    }
    SubClass* p = reinterpret_cast<SubClass*>(this);
    while(children.empty() && !p->is_end()){
      p->read();
    }
    return children.size() != 0;
  }

  // call back
  void operator()(el& e)
  {
    SubClass* p = reinterpret_cast<SubClass*>(this);
    p->read();
  }


  //-----------------------------------------------------
  el& add_child(std::string&& name)
  {
    size_t s = children.size();
    children.emplace_back(name,this);
    return children[s];
  }

  el& add_child(const std::string& name)
  {
    size_t s = children.size();
    children.emplace_back(name,this);
    return children[s];
  }

  el* get_active_open_element()
  {
    if(_active_cache != nullptr && !_active_cache->is_closed()){
      _active_cache = _active_cache ->get_active_open_element();
      return _active_cache;
    }
    _active_cache = nullptr;
    if(children.empty())return nullptr;
    el& e = children[children.size()-1];
    if(e.is_closed())return nullptr;
    _active_cache = e.get_active_open_element();
    return _active_cache;
  }

  void close_children()
  {
    for(auto& c:children)c.close();
  }
  

 private:
  ::nodamushi::boxvec<el> children;
  bool remove;
  el*  _active_cache;
};


}}// end namespace nodamushi::svd

#endif // __NODAMUSHI_SVD_SVD_READER_HPP__
