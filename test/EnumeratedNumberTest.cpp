#include <iostream>
#include "nodamushi/svd/EnumeratedNumber.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <string>

using namespace nodamushi::svd;

std::string to_str(const EnumeratedNumber& n){
  std::stringstream s;
  s << n;
  s << 100 << std::hex <<10;
  return s.str();
}

TEST(entest,dec)
{
  EnumeratedNumber n ;
  
  n = "100";
  ASSERT_EQ(n.get_value(),100);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),10);
  ASSERT_EQ(to_str(n),"100100a");
  n = "12345678910";
  ASSERT_EQ(n.get_value(),12345678910);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),10);
  ASSERT_EQ(to_str(n),"12345678910100a");
  
}

TEST(entest,hex)
{
  EnumeratedNumber n ;
  n = "0x100";
  ASSERT_EQ(n.get_value(),0x100);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),16);
  ASSERT_EQ(to_str(n),"0x100100a");
  n = "0x123456789AbC";
  ASSERT_EQ(n.get_value(),0x123456789ABC);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),16);
  ASSERT_EQ(to_str(n),"0x123456789ABC100a");
}

TEST(entest,bin1)
{
  EnumeratedNumber n ;
  n = "0b001001";
  ASSERT_EQ(n.get_value(),9);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),2);
  ASSERT_EQ(to_str(n),"0b1001100a");
  n = "0b1001";
  ASSERT_EQ(n.get_value(),9);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),2);
  ASSERT_EQ(to_str(n),"0b1001100a");

  n = "0b0101010101010101010101010101010101010101010101010101010101010101";
  ASSERT_EQ(n.get_value(),0x5555555555555555ll);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),2);
  ASSERT_EQ(to_str(n),"0b101010101010101010101010101010101010101010101010101010101010101100a");
}


TEST(entest,bin2)
{
  EnumeratedNumber n ;
  n = "0bxx1xx1";
  ASSERT_EQ(n.get_value(),9);
  ASSERT_EQ(n.get_dont_care(),54);
  ASSERT_EQ(n.get_base(),2);
  ASSERT_EQ(to_str(n),"0bxx1xx1100a");
  n = "0b1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x101";
  ASSERT_EQ(n.get_value(),    0x5555555555555555ll);
  ASSERT_EQ(n.get_dont_care(),0x2AAAAAAAAAAAAAA8ll);
  ASSERT_EQ(n.get_base(),2);
  ASSERT_EQ(to_str(n),"0b1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x101100a");
}
TEST(entest,bin3)
{
  EnumeratedNumber n ;
  n = "#001001";
  ASSERT_EQ(n.get_value(),9);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),2);
  ASSERT_EQ(to_str(n),"#1001100a");
  n = "#0101010101010101010101010101010101010101010101010101010101010101";
  ASSERT_EQ(n.get_value(),0x5555555555555555ll);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),2);
  ASSERT_EQ(to_str(n),"#101010101010101010101010101010101010101010101010101010101010101100a");
}


TEST(entest,bin4)
{
  EnumeratedNumber n ;
  n = "#xx1xx1";
  ASSERT_EQ(n.get_value(),9);
  ASSERT_EQ(n.get_dont_care(),54);
  ASSERT_EQ(to_str(n),"#xx1xx1100a");
  n = "#1001";
  ASSERT_EQ(n.get_value(),9);
  ASSERT_EQ(n.get_dont_care(),0);
  ASSERT_EQ(n.get_base(),2);
  ASSERT_EQ(to_str(n),"#1001100a");

  n = "#x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1";
  ASSERT_EQ(n.get_value(),    0x5555555555555555ll);
  ASSERT_EQ(n.get_dont_care(),0xAAAAAAAAAAAAAAAAll);
  ASSERT_EQ(to_str(n),"#x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1x1100a");
}


TEST(entest,compare)
{
  EnumeratedNumber n1,n2,n3,n4;
  n1 = "#1001";
  n2 = "#1011";
  n3 = "#10x1";
  n4 = "#1x11";
  ASSERT_LT(n1,n2);
  ASSERT_LT(n1,n4);
  ASSERT_GT(n2,n1);
  ASSERT_EQ(n1,n3);
  ASSERT_EQ(n2,n3);
  ASSERT_EQ(n3,n4);
  ASSERT_EQ(n2,n4);
  ASSERT_EQ(n3,9);
  ASSERT_EQ(n3,11);
}

