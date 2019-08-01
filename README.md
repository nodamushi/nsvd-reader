# NSVD Reader

C++ Header only [CMSIS5 SVD](http://www.keil.com/pack/doc/CMSIS/SVD/html/index.html) read library.

This is an unofficial library.


# License

These codes are licensed under CC0.

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png "CC0")](http://creativecommons.org/publicdomain/zero/1.0/deed.ja)

# Language

C++14/17


# Install


Add 'include' directory to your project include search path.


# Dependency

This library uses the external libraries shown below to read XML files.

1. [Boost.PropertyTree](https://www.boost.org/doc/libs/1_65_1/doc/html/property_tree.html) (#include "nodamushi/svd/boost.hpp")
1. [Expat](https://libexpat.github.io/) (#include "nodamushi/svd/expat.hpp")
1. [TinyXML-2](https://github.com/leethomason/tinyxml2) (#include "nodamushi/svd/tinyxml.hpp")


You can choose which library to use. You can also create your own XML reader.

Follow the license of the external library to use.

# Feature

1. read svd file and edit data
1. resolve 'derivedFrom'
1. visitor pattern

# Sample Code

Here is a simple example code.

For other examples, see the files in the sample directory.

```c++
#include <string>
#include <iostream>

#include "nodamushi/svd.hpp"
#include "nodamushi/svd/normalize.hpp"

// select xml reader
#include "nodamushi/svd/boost.hpp"
//#include "nodamushi/svd/expat.hpp"
//#include "nodamushi/svd/tinyxml.hpp"

// visitor pattern
struct visitor
{
  using this_t =  visitor;
#undef  VISITOR_MEMBER
#define VISITOR_MEMBER
  SVD_VISITOR_INIT;
  //-------------------------------
  SVD_Visit_Interrupt(i)
  {
    if(i.name && i.value)
      std::cout << "interrupt:" <<*i.name << "," << *i.value<<std::endl;
    return result::CONTINUE;
  }

  SVD_Visit_Register(r)
  {
    count_register++;
    return result::CONTINUE;
  }
  
  int count_register=0;
};


int main(int argc,char** argv)
{
  std::string filename = argv[1];
  
  // read svd
  namespace svd=nodamushi::svd;
  svd::boost_svd_reader svd(filename);
  svd::Device<> device(svd);
  
  // edit data
  device.vendor = "Foo Bar.inc";
  device.description = "Foo Bar";
  
  // print SVD(xml)
  std::cout << device << std::endl;

  // get cpu name
  if(device.cpu && device.cpu->name){
    std::cout <<"Device CPU:"<< device.cpu->name.get() << std::endl;
  }else{
    std::cout << "Device CPU is not defined" << std::endl;
  }
  
  // use visitor pattern
  visitor v;
  visitor::accept(device,v);
  
  // normalize (resovle 'derivedFrom')
  namespace norm =nodamushi::svd::normalized;
  auto dptr = svd::normalize(device);
  // dptr is std::shared_ptr<norm::Device<std::string/std::string_view>>

  if(auto p = dptr->find_peripheral("UART2")){
    for(auto& r:p->registers){
      std::cout << "UART2 Register:" 
                << r.name << ","
                << r.get_address() << std::endl;
    }
  }
}
```

# Document

TODO
