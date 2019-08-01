#include <iostream>
#include "nodamushi/svd/dim_type.hpp"
#include <sstream>
#include <gtest/gtest.h>

using namespace nodamushi::svd;

std::string to_string(dimIndex i)
{
  std::stringstream s;
  s << i;
  return s.str();
}

TEST(dimIndex,test1)
{
  dimIndex i;

  ASSERT_FALSE(i);
  ASSERT_EQ(i.size(),0);

  i="   aa  ";
  ASSERT_TRUE(i);
  ASSERT_EQ(i.size(),1);
  ASSERT_EQ(i[0],"aa");
  ASSERT_EQ(to_string(i), "aa");

  i="  1    ";
  ASSERT_TRUE(i);
  ASSERT_EQ(i.size(),1);
  ASSERT_EQ(i[0],"1");


  i = "aaa  \n,  bbb  ,  ccc   ";

  ASSERT_TRUE(i);
  ASSERT_EQ(i.size(),3);
  ASSERT_EQ(i[0],"aaa");
  ASSERT_EQ(i[1],"bbb");
  ASSERT_EQ(i[2],"ccc");
  ASSERT_EQ(to_string(i), "aaa,bbb,ccc");

  i = "1  ,  001  ,  10  -  20  ";
  
  ASSERT_TRUE(i);
  ASSERT_EQ(i.size(),13);
  ASSERT_EQ(i[0],"1");
  ASSERT_EQ(i[1],"001");
  ASSERT_EQ(i[2],"10");
  ASSERT_EQ(i[3],"11");
  ASSERT_EQ(i[4],"12");
  ASSERT_EQ(i[5],"13");
  ASSERT_EQ(i[6],"14");
  ASSERT_EQ(i[7],"15");
  ASSERT_EQ(i[8],"16");
  ASSERT_EQ(i[9],"17");
  ASSERT_EQ(i[10],"18");
  ASSERT_EQ(i[11],"19");
  ASSERT_EQ(i[12],"20");
  ASSERT_EQ(to_string(i), "1,001,10-20");

  i = "1  ,  001  ,10-20";
  
  ASSERT_TRUE(i);
  ASSERT_EQ(i.size(),13);
  ASSERT_EQ(i[0],"1");
  ASSERT_EQ(i[1],"001");
  ASSERT_EQ(i[2],"10");
  ASSERT_EQ(i[3],"11");
  ASSERT_EQ(i[4],"12");
  ASSERT_EQ(i[5],"13");
  ASSERT_EQ(i[6],"14");
  ASSERT_EQ(i[7],"15");
  ASSERT_EQ(i[8],"16");
  ASSERT_EQ(i[9],"17");
  ASSERT_EQ(i[10],"18");
  ASSERT_EQ(i[11],"19");
  ASSERT_EQ(i[12],"20");
  ASSERT_EQ(to_string(i), "1,001,10-20");

}
