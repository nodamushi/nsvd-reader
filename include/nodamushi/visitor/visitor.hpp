/*!
  visitor contoller
  @file visitor.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_VISITOR_VISITOR_HPP__
#define __NODAMUSHI_VISITOR_VISITOR_HPP__

# include "nodamushi/visitor/result.hpp"

namespace nodamushi{
namespace visitor{


template<typename CONTROLLER,typename VISITOR,typename T>
result apply(T& t);

template<typename CONTROLLER,typename VISITOR,typename T>
result apply(T& t,VISITOR& visitor);

template<typename CONTROLLER,typename VISITOR,typename T>
result applyController(T& t);

template<typename CONTROLLER,typename VISITOR,typename T>
result applyController(T& t,VISITOR& visitor);


template<typename VISITOR,typename T>
result applyVisit(T& t);

template<typename VISITOR,typename T>
result applyVisit(T& t,VISITOR& visitor);


template<typename VISITOR,typename T>
result applyPostVisit(T& t);

template<typename VISITOR,typename T>
result applyPostVisit(T& t,VISITOR& visitor);


enum class _visitor_member_type_:int
{
  staticmt=0,
  mt=1
};
template<typename T,_visitor_member_type_ v>struct is_static
{
  static constexpr bool value = v ==_visitor_member_type_::staticmt;
};



#define __NODAMUSHI_VISIT_ENUM__(x,y) ::nodamushi::visitor::_visitor_member_type_::__NODAMUSHI_VISIT_ENUM2__(x,y)
#define __NODAMUSHI_VISIT_ENUM2__(x,y) x##y
#define VISITOR_INIT\
  using result = ::nodamushi::visitor::result;                          \
  static constexpr result CONTINUE = result::CONTINUE;                  \
  static constexpr result BREAK = result::BREAK;                        \
  static constexpr result SKIP = result::SKIP;                          \
  template<typename T>VISITOR_MEMBER result visit(const T& t){return result::CONTINUE;} \
  template<typename T>VISITOR_MEMBER result visit(T& t){return result::CONTINUE;} \
  template<typename T>VISITOR_MEMBER result postVisit(const T& t){return result::CONTINUE;} \
  template<typename T>VISITOR_MEMBER result postVisit(T& t){return result::CONTINUE;} 
  
# ifndef VISITOR_MEMBER
#   define VISITOR_MEMBER
# endif

} // end namespace visitor
}// end namespace nodamushi

# include "nodamushi/visitor/visitor/imple.hpp"

#endif // __NODAMUSHI_VISITOR_VISITOR_HPP__
