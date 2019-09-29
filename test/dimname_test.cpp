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

TEST(dimname,xml)
{
  using namespace boost::property_tree;
  auto fname = find_file("data/test","dimname.svd");
  ASSERT_TRUE(!fname.empty());
  boost_svd_reader svd(fname);
  Device<> device(svd);
  auto dptr = normalize(device);

  ASSERT_TRUE(dptr);

  auto& d = *dptr;
  auto Aptr = d.find_peripheral("A");
  ASSERT_TRUE(Aptr);
  ASSERT_EQ(Aptr->registers.size(),16);
  ASSERT_EQ(Aptr->registers[0].name,"A0");
  ASSERT_EQ(Aptr->registers[1].name,"A1");
  ASSERT_EQ(Aptr->registers[2].name,"A2");
  ASSERT_EQ(Aptr->registers[3].name,"B[0]");
  ASSERT_EQ(Aptr->registers[4].name,"B[1]");
  ASSERT_EQ(Aptr->registers[5].name,"B[2]");
  ASSERT_EQ(Aptr->registers[6].name,"C1");
  ASSERT_EQ(Aptr->registers[7].name,"C2");
  ASSERT_EQ(Aptr->registers[8].name,"C3");
  ASSERT_EQ(Aptr->registers[9].name,"C4");
  ASSERT_EQ(Aptr->registers[10].name,"C5");
  ASSERT_EQ(Aptr->registers[11].name,"C10");
  ASSERT_EQ(Aptr->registers[12].name,"C11");
  ASSERT_EQ(Aptr->registers[13].name,"C12");
  ASSERT_EQ(Aptr->registers[14].name,"C13");
  ASSERT_EQ(Aptr->registers[15].name,"C14");


  ASSERT_EQ(Aptr->registers[0].fields.size(),3);
  ASSERT_EQ(Aptr->registers[1].fields.size(),3);
  ASSERT_EQ(Aptr->registers[2].fields.size(),3);
  ASSERT_EQ(Aptr->registers[2].fields[0].name,"F0");
  ASSERT_EQ(Aptr->registers[2].fields[1].name,"F1");
  ASSERT_EQ(Aptr->registers[2].fields[2].name,"F2");

  ASSERT_EQ(Aptr->registers[3].fields.size(),3);
  ASSERT_EQ(Aptr->registers[4].fields.size(),3);
  ASSERT_EQ(Aptr->registers[5].fields.size(),3);
  ASSERT_EQ(Aptr->registers[5].fields[0].name,"Foo");
  ASSERT_EQ(Aptr->registers[5].fields[1].name,"Bar");
  ASSERT_EQ(Aptr->registers[5].fields[2].name,"Piyo");


  ASSERT_EQ(Aptr->registers[6].fields[0].name,"AFooB");
  ASSERT_EQ(Aptr->registers[6].fields[1].name,"ABarB");
  ASSERT_EQ(Aptr->registers[6].fields[2].name,"APiyoB");
}
