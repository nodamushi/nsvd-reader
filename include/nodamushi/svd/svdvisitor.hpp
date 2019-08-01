/*!
  @brief SVD visitor

@code
// DON'T use local class/struct
struct my_visitor
{
  using this_t =  my_visitor; // set this class name
#undef  VISITOR_MEMBER        // reset VISITOR_MEMBER(this macro is used in SVDVISITOR_INIT)
#define VISITOR_MEMBER static // set member type static

// #define VISITOR_MEMBER     // * non-static version

  SVD_VISITOR_INIT; // initialize macro
  //-------------------------------
  
  // visit functions
  //  implement the necessary elements
  //  if you nead const prefix, define functions the following:
  //   SVD_Visit_Device(v,const){

  SVD_Visit_Device(v){ // v = nodamushi::svd::Device
    return CONTINUE;
  }
  SVD_PostVisit_Device(v){ // v = nodamushi::svd::Device
    return CONTINUE;
  }

  SVD_Visit_Peripheral(v){// v = nodamushi::svd::Peripheral
    return CONTINUE;
  }
  SVD_PostVisit_Peripheral(v){// v = nodamushi::svd::Peripheral
    return CONTINUE;
  }

  SVD_Visit_Interrupt(v){// v = nodamushi::svd::Interrupt
    return CONTINUE;
  }

  SVD_Visit_AddressBlock(v){// v = nodamushi::svd::AddressBlock
    return CONTINUE;
  }

  SVD_Visit_Register(v){// v = nodamushi::svd::Register
    return CONTINUE;
  }
  SVD_PostVisit_Register(v){// v = nodamushi::svd::Register
    return CONTINUE;
  }

  SVD_Visit_Field(v){// v = nodamushi::svd::Field
    return CONTINUE;
  }
  SVD_PostVisit_Field(v){// v = nodamushi::svd::Field
    return CONTINUE;
  }

  SVD_Visit_EnumeratedValues(v){ // field.enumeratedValues
    return CONTINUE;
  }
  SVD_PostVisit_EnumeratedValues(v){
    return CONTINUE;
  }

  SVD_Visit_EnumeratedValue(v){ // field.enumeratedValues.enumeratedValue
    return CONTINUE;
  }
};

//
// call visitor
//
// my_visitor::accept(device); //static
// my_visitor::accept(device, my_visitor() );// non-static
//
@endcode

  see sample/visitor_sample.cpp sample code.
  @file svdvisitor.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_SVDVISITOR_HPP__
#define __NODAMUSHI_SVD_SVDVISITOR_HPP__

# include <type_traits>
# include <cassert>
# include "nodamushi/visitor/visitor.hpp"

namespace nodamushi{
namespace svd{

//! initialize svd visitor class.define accept method
# define SVD_VISITOR_INIT                                               \
  VISITOR_INIT                                                          \
  template<typename T>                                                  \
  static void accept(T& t){                                             \
    using namespace ::nodamushi::visitor;                               \
    constexpr ::nodamushi::visitor::_visitor_member_type_ __member_type__= \
        __NODAMUSHI_VISIT_ENUM__(VISITOR_MEMBER,mt); \
    static_assert(is_static<T,__member_type__>::value,"static visitor");     \
    ::nodamushi::svd::svdvisitor<this_t>::visit(t);                   \
  }                                                                     \
  template<typename T>                                                  \
  static void accept(T& t,this_t& o){                                   \
    using namespace ::nodamushi::visitor;                                 \
    constexpr ::nodamushi::visitor::_visitor_member_type_ __member_type__= \
        __NODAMUSHI_VISIT_ENUM__(VISITOR_MEMBER,mt); \
    static_assert(!is_static<T,__member_type__>::value,"instance visitor"); \
    ::nodamushi::svd::svdvisitor<this_t>::visit(t,o);}                  \
  template<typename T>                                                  \
  static void accept(this_t& o,T& t){\
    using namespace ::nodamushi::visitor;                               \
    constexpr ::nodamushi::visitor::_visitor_member_type_ __member_type__= \
        __NODAMUSHI_VISIT_ENUM__(VISITOR_MEMBER,mt); \
    static_assert(!is_static<T,__member_type__>::value,"instance visitor"); \
    ::nodamushi::svd::svdvisitor<this_t>::visit(t,o);}                  \


//! svd visit device function
# define SVD_Visit_Device(v,...) template<typename S,typename V>        \
      VISITOR_MEMBER result visit(::nodamushi::svd::Device<S,V>__VA_ARGS__& v)
//! svd visit device after function
# define SVD_PostVisit_Device(v,...) template<typename S,typename V>    \
      VISITOR_MEMBER result postVisit(::nodamushi::svd::Device<S,V>__VA_ARGS__& v)

//! svd visit peripheral function
# define SVD_Visit_Peripheral(v,...) template<typename S>               \
  VISITOR_MEMBER result visit(::nodamushi::svd::Peripheral<S>__VA_ARGS__& v)
//! svd visit peripheral after function
# define SVD_PostVisit_Peripheral(v,...) template<typename S>           \
  VISITOR_MEMBER result postVisit(::nodamushi::svd::Peripheral<S>__VA_ARGS__& v)

# define SVD_Visit_Interrupt(v,...) template<typename S>               \
  VISITOR_MEMBER result visit(::nodamushi::svd::Interrupt<S>__VA_ARGS__& v)

# define SVD_Visit_AddressBlock(v,...)                                  \
  VISITOR_MEMBER result visit(::nodamushi::svd::AddressBlock __VA_ARGS__& v)


# define SVD_Visit_Cluster(v,...) template<typename S>                  \
  VISITOR_MEMBER result visit(::nodamushi::svd::Cluster<S>__VA_ARGS__& v)
# define SVD_PostVisit_Cluster(v,...) template<typename S>              \
  VISITOR_MEMBER result postVisit(::nodamushi::svd::Cluster<S> __VA_ARGS__& v)

# define SVD_Visit_Register(v,...) template<typename S>                 \
  VISITOR_MEMBER result visit(::nodamushi::svd::Register<S> __VA_ARGS__ & v)
# define SVD_PostVisit_Register(v,...) template<typename S>             \
  VISITOR_MEMBER result postVisit(__VA_ARGS__ ::nodamushi::svd::Register<S>& v)

# define SVD_Visit_Field(v,...) template<typename S>                    \
  VISITOR_MEMBER result visit(__VA_ARGS__ ::nodamushi::svd::Field<S>& v)
# define SVD_PostVisit_Field(v,...) template<typename S>                \
  VISITOR_MEMBER result postVisit(::nodamushi::svd::Field<S> __VA_ARGS__ & v)

# define SVD_Visit_EnumeratedValues(v,...) template<typename S>                    \
  VISITOR_MEMBER result visit(__VA_ARGS__::nodamushi::svd::Enumeration<S>& v)
# define SVD_PostVisit_EnumeratedValues(v,...) template<typename S>                    \
  VISITOR_MEMBER result postVisit(__VA_ARGS__ ::nodamushi::svd::Enumeration<S>& v)
# define SVD_Visit_EnumeratedValue(v,...) template<typename S>          \
  VISITOR_MEMBER result visit(__VA_ARGS__ ::nodamushi::svd::EnumeratedValue<S>& v)

template<typename CONTROLLER,typename VISITOR,typename T>
struct handleVisit{
  template<typename T2,typename V>
  static ::nodamushi::visitor::result visit(T2& t,V& v){
    return ::nodamushi::visitor::result::SKIP;}
};




template<typename VISITOR>
struct svdvisitor
{

  struct __svd_dummy_visitor_{};

  using this_t = svdvisitor<VISITOR>;
  using result = ::nodamushi::visitor::result;

  template<typename T>
  static result visit(T& t)
  {
    return ::nodamushi::visitor::apply<this_t,VISITOR>(t) == result::BREAK?
        result::BREAK : result::CONTINUE;
  }
  template<typename T>
  static result visit(T& t,VISITOR& v)
  {
    return ::nodamushi::visitor::apply<this_t,VISITOR>(t,v) == result::BREAK?
        result::BREAK : result::CONTINUE;
  }

  template<typename T>
  static result handleVisit(T& t)
  {
    using T2 =typename std::remove_const<T>::type;
    __svd_dummy_visitor_ dummy;
    auto ret =::nodamushi::svd::handleVisit<this_t,VISITOR,T2>::template visit<T>(t,dummy);
    return ret;
  }

  template<typename T>
  static result handleVisit(T& t,VISITOR& v)
  {
    using T2 =typename std::remove_const<T>::type;
    return ::nodamushi::svd::handleVisit<this_t,VISITOR,T2>::template visit<T>(t,v);
  }

  template<typename T>
  static result apply(T& t,VISITOR& v)
  {
    return ::nodamushi::visitor::applyController<this_t,VISITOR>(t,v);
  }

  template<typename T>
  static result apply(T& t,__svd_dummy_visitor_& v)
  {
    return ::nodamushi::visitor::applyController<this_t,VISITOR>(t);
  }
};




#define __NX_SVD_HANDLE_VISIT(type)                                     \
  template<typename CONTROLLER,                                         \
           typename VISITOR,                                            \
           class A>                                                     \
  struct handleVisit<CONTROLLER,VISITOR,type<A>>{                       \
  template<typename T,typename V>                                       \
  static ::nodamushi::visitor::result visit(T& t,V&v)                   \
      
#define __NX_SVD_HANDLE_VISIT2(type)                                    \
  template<typename CONTROLLER,                                         \
           typename VISITOR,                                            \
           class A,class B>                                             \
  struct handleVisit<CONTROLLER,VISITOR,type<A,B>>{                     \
  template<typename T,typename V>                                       \
  static ::nodamushi::visitor::result visit(T& t,V& v)



// ignore constant
template<typename T1,typename T2>struct is_same
{
  static constexpr bool value =
      std::is_same<
    typename std::remove_const<T1>::type,
    typename std::remove_const<T2>::type>::value;
};





} // end namespace svd
}// end namespace nodamushi

#endif // __NODAMUSHI_SVD_SVDVISITOR_HPP__
