#include <iostream>
#include <cstdlib>
#include "nodamushi/svd.hpp"
#include "nodamushi/svd/boost.hpp"
#include "nodamushi/svd/normalized.hpp"
using namespace nodamushi::svd::normalized;

struct visitorbar
{
  using this_t = visitorbar;
#undef VISITOR_MEMBER
#define VISITOR_MEMBER static
  NORM_VISITOR_INIT;
  //----------------------------------------------
  
  NORM_Visit_Register(r){
    std::cout << "Register:" <<r.get_path() << std::endl;
    return CONTINUE;
  }
};

struct ivisitorbar
{
  using this_t = ivisitorbar ;
#undef  VISITOR_MEMBER
#define VISITOR_MEMBER
  SVD_VISITOR_INIT;

  SVD_Visit_Device(v){
    std::cout << "instance::----device-----" <<  std::endl;
    return CONTINUE;
  }

  SVD_Visit_Peripheral(v){
    std::cout << "instance::  peripheral:" <<*v.name<< std::endl;
    return SKIP;
  }
};

void bar(const std::string& filename)
{
  namespace svd = nodamushi::svd;
  svd::boost_svd_reader r(filename);
  svd::Device<> device(r);
  ivisitorbar visitor;
  ivisitorbar::accept(device,visitor);
  
  // normalize
  auto dptr = svd::normalize(device);

  if(dptr){
    std::cout << "ok" << std::endl;
    
    // visitor
    visitorbar::accept(dptr);

  }else{
    std::cout << "failed"<<std::endl;
  }

}

