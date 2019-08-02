/*!
  @brief svd_reader_stream_helper.hpp
  @file nodamushi/svd/svd_reader_stream_helper.hpp
*/
#ifndef __NODAMUSHI_SVD_STREAM_HELPER_HPP__
#define __NODAMUSHI_SVD_STREAM_HELPER_HPP__

namespace nodamushi{
namespace svd{


/** 
 * @brief XML element class for stream type parser
 * @param CallBack callback object class
 * - read(stream_reade_element&) : this method will be called when get_value() is called
 * - handle_error(svd_error e,svd_element elem,stream_reader_element&):
 *   this method is called from  unknown_element()and llegal_value()
 *   
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
        call_back->read(*this);
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
        call_back->read(*this);
    }
    return children.size()!=0;
  }
  void unknown_element(svd_element e)
  {
    if(call_back!=nullptr)
      call_back->handle_error(svd_error::UNKNOWN_ELEMENT,e,*this);
  }

  void illegal_value(svd_element e)
  {
    if(call_back!=nullptr)
      call_back->handle_error(svd_error::UNKNOWN_ELEMENT,e,*this);
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
  using error_handler = std::function<void(svd_error,svd_element,el&)>;
  
  stream_reader_base(): children{},remove(false),_active_cache(nullptr),ehandler()
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
  void unknown_element(svd_element e)const noexcept{} // root skip
  void illegal_value(svd_element e)const noexcept{} // root skip
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


  void read(el& e)
  {
    SubClass* p = reinterpret_cast<SubClass*>(this);
    p->read();
  }
  void handle_error(svd_error e,svd_element elem,el& r)const
  {
    if(ehandler) ehandler(e,elem,r);
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
  error_handler ehandler;
};
}}// end namespace nodamushi

#endif // __NODAMUSHI_SVD_STREAM_HELPER_HPP__
