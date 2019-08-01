/*!
  @brief visitor imple
  @file nodamushi/visitor/visitor/imple.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_VISITOR_IMPLE_HPP__
#define __NODAMUSHI_VISITOR_IMPLE_HPP__

# include <type_traits>
# include <vector>
# include <memory>
# include "nodamushi/visitor/visitor.hpp"
# include "nodamushi/box.hpp"
# include "nodamushi/boxvec.hpp"
namespace nodamushi{
namespace visitor{


namespace details{
template<typename CONTROLLER,typename VISITOR,typename T>
struct applys
{
  template<typename T2>
  static result apply(T2& t)
  {
    switch(applyVisit<VISITOR>(t))
    {
      case result::BREAK: return result::BREAK;
      case result::SKIP: return result::CONTINUE;
      default: break;
    }
    result r = CONTROLLER::handleVisit(t);
    if(r == result::BREAK) return result::BREAK;
    result r2 = applyPostVisit<VISITOR>(t);
    if(r2 == result::BREAK) return result::BREAK;
    
    return r2 ==result::SKIP?result::SKIP:r;
  }

  template<typename T2>
  static result apply(T2& t,VISITOR& v)
  {
    switch(applyVisit(t,v))
    {
      case result::BREAK: return result::BREAK;
      case result::SKIP: return result::CONTINUE;
      default: break;
    }
    result r = CONTROLLER::handleVisit(t,v);
    if(r == result::BREAK) return result::BREAK;
    result r2 = applyPostVisit(t,v);
    if(r2 == result::BREAK) return result::BREAK;
    
    return r2 ==result::SKIP?result::SKIP:r;
  }

  template<typename T2>
  static result applyController(T2& t)
  {
    return CONTROLLER::visit(t);
  }

  template<typename T2>
  static result applyController(T2& t,VISITOR& v)
  {
    return CONTROLLER::visit(t,v);
  }  
};

template<typename CONTROLLER,typename VISITOR,typename T>
struct applys<CONTROLLER,VISITOR,::nodamushi::box<T>>
{

  template<typename T2>
  static result apply(T2& t)
  {
    return ::nodamushi::visitor::apply<CONTROLLER,VISITOR>(*t);
  }

  template<typename T2>
  static result apply(T2& t,VISITOR& v)
  {
    return ::nodamushi::visitor::apply<CONTROLLER,VISITOR>(*t,v);
  }

  template<typename T2>
  static result applyController(T2& t)
  {
    return ::nodamushi::visitor::applyController<CONTROLLER,VISITOR>(*t);
  }

  template<typename T2>
  static result applyController(T2& t,VISITOR& v)
  {
    return ::nodamushi::visitor::applyController<CONTROLLER,VISITOR>(*t,v);
  }  
};

template<typename CONTROLLER,typename VISITOR,typename T>
struct applys<CONTROLLER,VISITOR,std::vector<T>>
{
  template<typename T2>
  static result apply(T2& t)
  {
    for(auto& e:t){
      auto r = ::nodamushi::visitor::apply<CONTROLLER,VISITOR>(e);
      if(r != result::CONTINUE) return r;
    }
    return result::CONTINUE;
  }

  template<typename T2>
  static result apply(T2& t,VISITOR& v)
  {
    for(auto& e:t){
      auto r = ::nodamushi::visitor::apply<CONTROLLER,VISITOR>(e,v);
      if(r != result::CONTINUE) return r;
    }
    return result::CONTINUE;
  }

  template<typename T2>
  static result applyController(T2& t)
  {
    for(auto& e:t){
      auto r = ::nodamushi::visitor::applyController<CONTROLLER,VISITOR>(e);
      if(r != result::CONTINUE) return r;
    }
    return result::CONTINUE;
  }

  template<typename T2>
  static result applyController(T2& t,VISITOR& v)
  {
    for(auto& e:t){
      auto r = ::nodamushi::visitor::applyController<CONTROLLER,VISITOR>(e,v);
      if(r != result::CONTINUE) return r;
    }
    return result::CONTINUE;
  }
};

template<typename CONTROLLER,typename VISITOR,typename T,typename PTR>
struct applys<CONTROLLER,VISITOR,::nodamushi::boxvec<T,PTR>>
{
  template<typename T2>
  static result apply(T2& t)
  {
    for(auto& e:t){
      auto r = ::nodamushi::visitor::apply<CONTROLLER,VISITOR>(e);
      if(r != result::CONTINUE) return r;
    }
    return result::CONTINUE;
  }

  template<typename T2>
  static result apply(T2& t,VISITOR& v)
  {
    for(auto& e:t){
      auto r = ::nodamushi::visitor::apply<CONTROLLER,VISITOR>(e,v);
      if(r != result::CONTINUE) return r;
    }
    return result::CONTINUE;
  }

  template<typename T2>
  static result applyController(T2& t)
  {
    for(auto& e:t){
      auto r = ::nodamushi::visitor::applyController<CONTROLLER,VISITOR>(e);
      if(r != result::CONTINUE) return r;
    }
    return result::CONTINUE;
  }

  template<typename T2>
  static result applyController(T2& t,VISITOR& v)
  {
    for(auto& e:t){
      auto r = ::nodamushi::visitor::applyController<CONTROLLER,VISITOR>(e,v);
      if(r != result::CONTINUE) return r;
    }
    return result::CONTINUE;
  }
};



}//end namespace details

//-------------------------------------------------------------------------

template<typename CONTROLLER,typename VISITOR,typename T>
result apply(T& t)
{
  using T2 =typename std::remove_const<T>::type;
  return details::applys<CONTROLLER,VISITOR,T2>::template apply<T>(t);
}


template<typename CONTROLLER,typename VISITOR,typename T>
result apply(T& t,VISITOR& v)
{
  using T2 =typename std::remove_const<T>::type;
  return details::applys<CONTROLLER,VISITOR,T2>::template apply<T>(t,v);
}

template<typename CONTROLLER,typename VISITOR,typename T>
result applyController(T& t)
{
  using T2 =typename std::remove_const<T>::type;
  return details::applys<CONTROLLER,VISITOR,T2>::template applyController<T>(t);
}


template<typename CONTROLLER,typename VISITOR,typename T>
result applyController(T& t,VISITOR& v)
{
  using T2 =typename std::remove_const<T>::type;
  return details::applys<CONTROLLER,VISITOR,T2>::template applyController<T>(t,v);
}



namespace details {


template<bool r,typename VISITOR,typename T>struct apply;

template<typename VISITOR,typename T>
struct apply<true,VISITOR,T>
{
  static result applyVisit(T& value)
  {
    return VISITOR::visit(value);
  }

  static result applyPostVisit(T& value)
  {
    return VISITOR::postVisit(value);
  }

  static result applyVisit(T& value,VISITOR& visitor)
  {
    return visitor.visit(value);
  }
  static result applyPostVisit(T& value,VISITOR& visitor)
  {
    return visitor.postVisit(value);
  }
};


template<typename VISITOR,typename T>
struct apply<false,VISITOR,T>
{
  static result applyVisit(T& value)
  {
    VISITOR::visit(value);
    return result::CONTINUE;
  }

  static result applyPostVisit(T& value)
  {
    VISITOR::postVisit(value);
    return result::CONTINUE;
  }

  static result applyVisit(T& value,VISITOR& visitor)
  {
    visitor.visit(value);
    return result::CONTINUE;
  }
  static result applyPostVisit(T& value,VISITOR& visitor)
  {
    visitor.postVisit(value);
    return result::CONTINUE;
  }
};


template<typename t>struct is_result
{
  static constexpr bool value = std::is_same<::nodamushi::visitor::result,t>::value;
};

}  // details




template<typename VISITOR,typename T>
result applyVisit(T& t)
{
  return details::apply<details::is_result<decltype(VISITOR::visit(t))>::value,
                        VISITOR,T>::applyVisit(t);
}

template<typename VISITOR,typename T>
result applyVisit(T& t,VISITOR& v)
{
  return details::apply<details::is_result<decltype(v.visit(t))>::value,
                        VISITOR,T>::applyVisit(t,v);
}


template<typename VISITOR,typename T>
result applyPostVisit(T& t)
{
  return details::apply<details::is_result<decltype(VISITOR::visit(t))>::value,
                        VISITOR,T>::applyPostVisit(t);
}

template<typename VISITOR,typename T>
result applyPostVisit(T& t,VISITOR& v)
{
  return details::apply<details::is_result<decltype(v.visit(t))>::value,
                        VISITOR,T>::applyPostVisit(t,v);
}

} // end namespace visitor
}// end namespace nodamushi

#endif // __NODAMUSHI_VISITOR_IMPLE_HPP__
