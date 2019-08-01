#include <iostream>
#include "nodamushi/svd/boost.hpp"
#include "nodamushi/svd.hpp"
#include "nodamushi/svd/normalized.hpp"


#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "test_utility.hpp"
#include "compare.hpp"
#include <unordered_set>


using namespace nodamushi::svd;
using did_t =typename nodamushi::svd::normalized::dim_info::id_t;

TEST(cluster,xml)
{
  using namespace boost::property_tree;
  auto fname = find_file("data/test","cluster.svd");
  ASSERT_TRUE(!fname.empty());
  ptree bp,bp2;
  read_xml(fname, bp);
  boost_svd_reader svd(bp);
  Device<> device(svd);
  // print xml

  std::stringstream ss;
  ss << device;
  read_xml(ss, bp2);
  boost_svd_reader svd1(bp);
  boost_svd_reader svd2(bp2);
  xml_element e1(svd1);
  xml_element e2(svd2);
  ASSERT_TRUE(e1.equals(e2)) << ss.str();
}

TEST(cluster,normalize)
{
  using namespace nodamushi::svd;
  auto fname = find_file("data/test","cluster.svd");
  ASSERT_TRUE(!fname.empty());
  boost_svd_reader svd(fname);
  Device<> device(svd);
  auto dptr = normalize(device);
  auto& d = *dptr;

  //-device ---------------------------------------
  ASSERT_EQ(d.vendor,"CC0");
  ASSERT_EQ(d.vendorID,"CC0");
  ASSERT_EQ(d.name,"cluster");
  ASSERT_EQ(d.description,"foobar");
  ASSERT_EQ(d.series,"");
  ASSERT_EQ(d.version,"");
  ASSERT_EQ(d.licenseText,"");
  ASSERT_EQ(d.headerSystemFilename,"");
  ASSERT_EQ(d.addressUnitBits,8);
  ASSERT_EQ(d.width,32);
  ASSERT_EQ(d.size,32);
  ASSERT_EQ(d.access,Access::READ_WRITE);
  ASSERT_EQ(d.resetValue,1);
  ASSERT_EQ(d.resetMask,0xFFFFFFFF);
  ASSERT_EQ(d.peripherals.size() ,2);
  
  auto Aptr = d.find_peripheral("A");
  auto Bptr = d.find_peripheral("B");
  ASSERT_TRUE(Aptr);
  ASSERT_TRUE(Bptr);
  ASSERT_EQ(Aptr->name,"A");
  ASSERT_EQ(Aptr->clusters.size(),3);
  ASSERT_EQ(Aptr->baseAddress,0x1000);
  ASSERT_EQ(Aptr->size.get(0),32);
  ASSERT_EQ(Aptr->access,Access::READ_ONLY);
  ASSERT_EQ(Aptr->registers.size(),0);
  ASSERT_EQ(Aptr->addressBlock.size(),1);

  {
    std::unordered_set<did_t> set;
    for(auto& r:Aptr->clusters){
      ASSERT_FALSE(set.count(r.dim.id));
      set.insert(r.dim.id);
    }
  }
  //cluster B
  ASSERT_EQ(Aptr->clusters[0].name , "B");
  ASSERT_EQ(Aptr->clusters[0].derivedFrom , "C");
  ASSERT_EQ(Aptr->clusters[0].addressOffset , 0);
  ASSERT_EQ(Aptr->clusters[0].registers.size() , 3);
  ASSERT_EQ(Aptr->clusters[0].clusters.size() , 0);
  ASSERT_EQ(Aptr->clusters[0].registers[0].name , "X");
  ASSERT_EQ(Aptr->clusters[0].registers[1].name , "Y");
  ASSERT_EQ(Aptr->clusters[0].registers[2].name , "Z");  
  ASSERT_EQ(Aptr->clusters[0].registers[0].get_address() ,
            0x1000 + 0 + 0);  
  ASSERT_EQ(Aptr->clusters[0].registers[1].get_address() ,
            0x1000 + 0 + 4);  
  ASSERT_EQ(Aptr->clusters[0].registers[2].get_address() ,
            0x1000 + 0 + 8);  
  {
    std::unordered_set<did_t> set;
    for(auto& r:Aptr->clusters[0].registers){
      ASSERT_FALSE(set.count(r.dim.id));
      set.insert(r.dim.id);
    }
  }

  //cluster D
  ASSERT_EQ(Aptr->clusters[1].name , "D");
  ASSERT_EQ(Aptr->clusters[1].get_resetMask() ,100);
  ASSERT_EQ(Aptr->clusters[1].derivedFrom , "B.D");
  ASSERT_EQ(Aptr->clusters[1].addressOffset , 100);
  ASSERT_EQ(Aptr->clusters[1].registers.size() , 1);
  ASSERT_EQ(Aptr->clusters[1].get_peripheral() , Aptr);
  ASSERT_EQ(Aptr->clusters[1].clusters.size() , 1);
  ASSERT_EQ(Aptr->clusters[1].registers[0].name , "N");
  ASSERT_EQ(Aptr->clusters[1].registers[0].get_peripheral() , Aptr);
  ASSERT_EQ(Aptr->clusters[1].registers[0].get_resetMask() , 100);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].name , "C");
  ASSERT_EQ(Aptr->clusters[1].clusters[0].get_resetMask() , 100);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].addressOffset , 200);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers.size() , 2);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].clusters.size() , 0);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].get_address() , 
            0x1000 + 100 + 200);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].get_address_offset() , 
            100 + 200);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].get_peripheral() , Aptr);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[0].name , "Z");
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[0].addressOffset , 4);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[0].get_peripheral() , Aptr);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].name , "X");
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].addressOffset , 12);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_peripheral() , Aptr);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[0].get_address() , 
            0x1000 + 100 + 200 + 4);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_address() , 
            0x1000 + 100 + 200 + 12);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[0].get_address_offset() , 
            100 + 200 + 4);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_address_offset() , 
            100 + 200 + 12);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_size() ,32);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_dataType() ,DataType::UINT32);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_resetValue() ,1);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_resetMask() ,100);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_access() ,Access::READ_ONLY);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_protection(),Protection::PRIVILEGED);
  ASSERT_EQ(Aptr->clusters[1].clusters[0].registers[1].get_modifiedWriteValues(),ModifiedWriteValues::SET);
  //cluster C
  ASSERT_EQ(Aptr->clusters[2].name , "C");
  ASSERT_EQ(Aptr->clusters[2].addressOffset , 400);
  ASSERT_EQ(Aptr->clusters[2].registers.size() , 2);
  ASSERT_EQ(Aptr->clusters[2].clusters.size() , 0);
  ASSERT_EQ(Aptr->clusters[2].registers[0].name , "X");
  ASSERT_EQ(Aptr->clusters[2].registers[1].name , "Z");
  ASSERT_EQ(Bptr->name,"B");
  ASSERT_EQ(Bptr->clusters.size(),1);
  ASSERT_EQ(Bptr->registers.size(),0);
  ASSERT_EQ(Bptr->clusters[0].clusters[0].registers[1].get_resetMask() ,300);
}
