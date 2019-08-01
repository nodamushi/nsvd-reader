/*
  @brief svd visitor sample code
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <iostream>
#include "nodamushi/svd.hpp"
#include "nodamushi/svd/boost.hpp"


/**
 * @brief visitor sample
 *  -# define this_t
 *  -# clear VISITOR_MEMBER macro
 *  -# set VISITOR_MEMBER macro
 *  -# call SVD_VISIT_INIT macro
 *  -# define SVD_Visit_XXXX functions
 */
struct static_visitor
{
  //-------------------------------
  using this_t =  static_visitor ;// set this class name
#undef  VISITOR_MEMBER
#define VISITOR_MEMBER static     // set member type

  SVD_VISITOR_INIT;
  //-------------------------------

  SVD_Visit_Device(v,const){
    std::cout << "----const device-----" <<  std::endl;
    return CONTINUE;
  }

  SVD_Visit_Device(v){
    std::cout << "----device-----" <<  std::endl;
    return CONTINUE;
  }


  SVD_Visit_Peripheral(v,const){
    std::cout << "  const peripheral:" <<*v.name<< std::endl;
    return CONTINUE;
  }

  SVD_Visit_Peripheral(arg){
    std::cout << "  peripheral:" <<*arg.name<< std::endl;
    return CONTINUE;
  }

  SVD_Visit_Interrupt(i){
    std::cout << "   interrupt:" <<*i.name << ":" << *i.value<<std::endl;
    return CONTINUE;
  }

  SVD_Visit_AddressBlock(i){
    std::cout << "   address block:" <<*i.offset << " -" << *i.size<<std::endl;
    return CONTINUE;
  }


  SVD_Visit_Register(foo,const){
    std::cout << "     const register:" <<*foo.name<< std::endl;
    return CONTINUE;
  }

  SVD_Visit_Register(bar){
    std::cout << "     register:" <<*bar.name<< std::endl;
    return CONTINUE;
  }

  SVD_Visit_Field(abcdefghijklmnopqrstu){
    std::cout << "        field:" <<*abcdefghijklmnopqrstu.name<< std::endl;
    return CONTINUE;
  }
  SVD_Visit_Field(field,const){
    std::cout << "        const field:" <<*field.name<< std::endl;
    return CONTINUE;
  }

  SVD_Visit_EnumeratedValues(v){ // field.enumeratedValue
    std::cout << "            enumerated value:" <<*v.name<< std::endl;
    return SKIP;
  }

};


struct instance_visitor
{
  //-------------------------------
  using this_t =  instance_visitor ;// set this class name
#undef  VISITOR_MEMBER
#define VISITOR_MEMBER              // set member type(empty = instance member)

  SVD_VISITOR_INIT;
  //-------------------------------

  SVD_Visit_Device(v){
    std::cout << "instance::----device-----" <<  std::endl;
    return CONTINUE;
  }

  SVD_Visit_Peripheral(v){
    std::cout << "instance::  peripheral:" <<*v.name<< std::endl;
    return SKIP;
  }
};



int main(int argc, char *argv[])
{
  std::string filename = "../data/sample.svd";
  if(argc > 1)
    filename = argv[1];

  //-----------------------------------
  // read svd(xml) by boost property tree
  nodamushi::svd::boost_svd_reader svd_reader(filename);
  nodamushi::svd::Device<> device(svd_reader);
  // start visit
  static_visitor::accept(device); 
  
  instance_visitor visitor;
  instance_visitor::accept(device,visitor);
  
 
  return 0;
}
