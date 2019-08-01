/**
 * @brief value class, empty_value_access_error class
 * @file nodamushi/svd/value.hpp
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_VALUE_HPP__
#define __NODAMUSHI_SVD_VALUE_HPP__

# include <utility>
# include <type_traits>
# include <functional>
# include <ostream>
# include <sstream>
# include <stdexcept>

# include "nodamushi/const_string.hpp"
# include "nodamushi/svd/value/value_setter.hpp"
# include "nodamushi/svd/vattr.hpp"
# include "nodamushi/svd/svd_reader.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief access to the empty value object.
 */
struct empty_value_access_error:public std::logic_error
{
  empty_value_access_error(std::string& message):logic_error(message){}
};


/**
 * @brief This class reperesents SVD(xml) element / attribute.
 *
 * @par
 * This class has two states that this class object has value or empty, 
 * like an optional type.
 * @par
 * When the state of this class object is empty, 
 * reading the value will result in an error.
 * Check the status before accessing the value.
 * 
 * @param T type of value
 * @param attribute true: attribute, false: element
 * @param required 
 * @param name  element/attribute name
 * @see   empty_value_access_error
 */
template<typename T,bool attribute,bool required,char... name>struct value
{
  using this_t = value<T,attribute,required,name...>;
  using value_type_t = ::nodamushi::svd::vattr_unwrap<T>;
  using type = typename value_type_t::type;

  static constexpr bool REQUIRED=required;
  static constexpr bool ATTRIBUTE=attribute;
  
  // constructors ------------------------------------------------
  //! @brief default constructor
  value():_value(),_has_value(false){};
  //! @brief constructor with initial value
  value(const type& t):_value(t),_has_value(true){}
  //! @brief move constrocutor
  value(value&&) = default;
    
  // constant properties -----------------------------------------
  //! @return this property is required
  constexpr bool is_required()const noexcept{return required;}
  //! @return this property is an attribute
  constexpr bool is_attribute()const noexcept{return attribute;}
  //! @return this element/attribute name.
  NODAMUSHI_CONSTEXPR_STRING get_name()const noexcept;

  // contents ----------------------------------------------------
  //  setter
  /**
   * @brief set value from SVD reader
   * @return success(true) or fail(false)
   */
  template<typename SVD> auto operator =(SVD& src)
      ->typename std::enable_if<std::is_base_of<svd_reader,SVD>::value
                                ,bool>::type
  {
    if(::nodamushi::svd::value_try_set(_value,src)){
      _has_value = true;
      return true;
    }
    return false;
  }
  /**
   * @brief set value from SVD reader
   * @return success(true) or fail(false)
   */
  template<typename SVD> auto operator =(SVD&& src)
      ->typename std::enable_if<std::is_base_of<svd_reader,SVD>::value
                                ,bool>::type
  {
    if(::nodamushi::svd::value_try_set(_value,src)){
      _has_value = true;
      return true;
    }
    return false;
  }
  /**
   * @brief set value.
   * @param value new value
   * @return this object reference
   */
  template<typename V> auto operator =(const V& value)
      ->typename std::enable_if<!std::is_base_of<svd_reader,V>::value
                                ,this_t&>::type
  {
    _value = value;  _has_value =true;
    return *this;
  }
  /**
   * @brief set value.
   * @param value new value
   * @return this object reference
   */
  template<typename V> this_t& operator =(V&& value)
  {
    _value = std::move(value);  _has_value =true;
    return *this;
  }
  /**
   * @brief clear this object.
   * After calling this method,this object status will be empty.
   */
  void clear()noexcept;
  /** 
   *  @brief move
   */
  value& operator=(value&&)=default;
  /** 
   *  @brief copy
   */
  value& operator=(const value&)=default;
  

  
  // getter (empty_value_access_error will be occurer)
  /**
   * @brief get value
   * @return value reference.
   * @throw empty_value_access_error : when this object is empty.
   */
  const type& get()const;
  /**
   * @brief get value
   * @return value reference.
   * @throw empty_value_access_error : when this object is empty.
   */
  type& get();
  /**
   * @brief get value
   * @return value reference.
   * @throw empty_value_access_error : when this object is empty.
   */
  type& operator*(){return get();}
  /**
   * @brief get value
   * @return value reference.
   * @throw empty_value_access_error : when this object is empty.
   */
  const type& operator*()const {return get();}
  /**
   * @brief ->
   * @return value pointer
   * @throw empty_value_access_error : when this object is empty.
   */
  type* operator->(){return &get();}
  /**
   * @brief ->
   * @return value pointer
   * @throw empty_value_access_error : when this object is empty.
   */
  const type* operator->()const{return &get();}
  /**
   * @brief value == t
   * @param t 
   * @return when this object is empty, this method will return false.
   *  when this object has value, return value == t.
   */
  bool operator==(const type& t)const noexcept{return _has_value? _value ==t:false;}
  /**
   * @brief value != t
   * @param t 
   * @return when this object is empty, this method will return true.
   *  Otherwise, return value != t.
   */
  bool operator!=(const type& t)const noexcept{return _has_value? _value !=t:true;}
  /**
   * @brief value == t
   * @param t 
   * @return when this object is empty, this method will return false.
   *  Otherwise, return value == t.
   */
  bool operator==(type&& t)const noexcept{return _has_value? _value ==t:false;}
  /**
   * @brief value != t
   * @param t 
   * @return when this object is empty, this method will return true.
   *  Otherwise, return value != t.
   */
  bool operator!=(type&& t)const noexcept{return _has_value? _value !=t:true;}
  /**
   * @brief value == t
   * @param t 
   * @return 
   * If this object and t are empty, return true.
   * If either this object or t is empty, this method will return false.
   * Otherwise, return value == t.
   */
  bool operator==(const value& t)const noexcept
  {return _has_value&&t._has_value? _value ==t._value:_has_value==t._has_value;}
  /**
   * @brief value != t
   * @param t 
   * @return 
   * If this object and t are empty, return false.
   * If either this object or t is empty, this method will return true.
   * Otherwise, return value != t.
   */
  bool operator!=(const value&& t)const noexcept
  {return _has_value&&t._has_value? _value !=t._value:_has_value!=t._has_value;}

  /**
   * @brief Get value to initialize.
   * After this method is called, empty() method returns false.
   * @return value reference
   */
  type& get_for_init()noexcept{_has_value=true;return _value;}
  /**
   * @brief get or else 
   * @param defaultValue return value when this object is empty.
   * @return value or defaultValue
   */
  type& get(type& defaultValue)noexcept{return _has_value?_value:defaultValue;}
  /**
   * @brief get or else 
   * @param defaultValue return value when this object is empty.
   * @return value or defaultValue
   */
  type& get(type&& defaultValue)noexcept{return _has_value?_value:defaultValue;}
  /**
   * @brief get or else 
   * @param defaultValue return value when this object is empty.
   * @return value or defaultValue
   */
  const type& get(const type& defaultValue)const noexcept{return _has_value?_value:defaultValue;}



  /**
   * @brief when this object is empty,return true.
   * @return when this object is empty,return true.
   */
  bool empty()const noexcept{return !_has_value;}
  /**
   * @brief ! empty()
   * @see empty()
   */
  operator bool()const noexcept{return _has_value;}
  

  /**
   * @brief Check not empty if this element/attribute is required.
   * @return is_required() ? !empty() : true ;
   * @see is_required()
   */
  bool check_require()const noexcept{return !required || _has_value;}
  
 private:
  type _value;
  bool _has_value;
};

}}// end namespace nodamushi::svd

# ifndef __DOXYGEN__DOCUMENT__PREPROCESSOR__

#  define SVD_VALUE(type,name,...) \
  _SVD_VALUE_(type,name,false,__VA_ARGS__,_SVD_VALUE0_,_SVD_VALUE1_,0)

#  define SVD_ATTR(type,name,...) \
  _SVD_VALUE_(type,name,true,__VA_ARGS__,_SVD_VALUE0_,_SVD_VALUE1_,0)

#  define SVD_TYPE(type,attr,require,name) \
  value_t<type,attr,require,__SVD_VALUE_EXPAND_NAME__(name)>

# include "nodamushi/svd/value/imple.hpp"
# include "nodamushi/svd/value/void_value.hpp"
# include "nodamushi/svd/value/define.hpp"

# else
// for doxygen

#  define SVD_VALUE(type,name,...) \
  value<type,ELEMENT> name

#  define SVD_ATTR(type,name,...) \
  value<type,ATTR> name

#  define SVD_TYPE(type,attr,require,name) \
  value<type,ATTR>

# endif


#endif // __NODAMUSHI_SVD_VALUE_HPP__
