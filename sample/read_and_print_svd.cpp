/*
  @brief read and print SVD sample
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#include <iostream>

#include "nodamushi/svd.hpp"
#include "nodamushi/svd/boost.hpp"
#include "nodamushi/svd/expat.hpp"
#include "nodamushi/svd/tinyxml.hpp"



nodamushi::svd::Device<> read_by_boost(const std::string& filename)
{
  namespace svd=nodamushi::svd;
  svd::boost_svd_reader r(filename);
  return svd::Device<>(r);
}


nodamushi::svd::Device<> read_by_Expat(const std::string& filename)
{
  namespace svd=nodamushi::svd;
  svd::expat_svd_reader r(filename);
  return svd::Device<>(r);
}

nodamushi::svd::Device<> read_by_TinyXML(const std::string& filename)
{
  namespace svd=nodamushi::svd;
  svd::tinyxml_svd_reader r(filename);
  return svd::Device<>(r);
}


/**
 * CLI argument: -e: Expat, -b: Boost, -t: TinyXML2
 */
int main(int argc, char *argv[])
{
  std::string filename = "../data/sample.svd";
  int mode = 0;//0:boost,1: Expat,2:TinyXML
  for(int i=1;i<argc;i++){
    std::string s = argv[i];
    if(s == "-e")mode = 1;
    else if(s == "-b")mode = 0;
    else if(s == "-t")mode = 2;
    else filename = s;
  }
  
  if(mode == 0){
    std::cout << " --- Boost Reader-----"<<std::endl;
    std::cout << read_by_boost(filename) << std::endl;
  }else if(mode == 1){
    std::cout << " --- Expat Reader-----"<<std::endl;
    std::cout << read_by_Expat(filename) << std::endl;
  }else if(mode == 2){
    std::cout << " --- TinyXML Reader-----"<<std::endl;
    std::cout << read_by_TinyXML(filename) << std::endl;
  }


  return 0;
}
