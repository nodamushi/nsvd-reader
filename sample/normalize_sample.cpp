/*
  @brief normalize sampel code
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <iostream>
#include <cstdlib>
#include "nodamushi/svd.hpp"
#include "nodamushi/svd/boost.hpp"
#include "nodamushi/svd/normalized.hpp"


/**
 * @brief visitor sample
 *  -# define this_t
 *  -# clear VISITOR_MEMBER macro
 *  -# set VISITOR_MEMBER macro
 *  -# call NORM_VISIT_INIT macro
 *  -# define NORM_Visit_XXXX functions
 */
struct visitor
{
  using this_t = visitor;
#undef VISITOR_MEMBER
#define VISITOR_MEMBER static
  NORM_VISITOR_INIT;
  //----------------------------------------------
  
  NORM_Visit_Register(r){
    std::cout << "Register:" <<r.get_path() << std::endl;
    return CONTINUE;
  }
};


int main(int argc, char *argv[])
{
  std::string filename = "../data/sample.svd";
  if(argc > 1) filename = argv[1];

  namespace svd = nodamushi::svd;
  svd::boost_svd_reader r(filename);
  svd::Device<> device(r);
  
  // normalize
  auto dptr = svd::normalize(device);

  if(dptr){
    std::cout << "ok" << std::endl;
    
    // visitor
    visitor::accept(dptr);

  }else{
    std::cout << "failed"<<std::endl;
  }
  return 0;
}
