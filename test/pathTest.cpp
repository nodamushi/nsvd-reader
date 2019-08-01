#include <iostream>
#include "nodamushi/svd/path.hpp"
#include <gtest/gtest.h>


TEST(path,constructors){
  using namespace nodamushi::svd;
  path<> p0;
  path<> p1("foo");
  path<> p2("foo.bar");
  path<> p3("a.b.c..e");
  ASSERT_EQ(p0.size() ,0);
  ASSERT_EQ(p1.size() ,1);
  ASSERT_EQ(p2.size() ,2);
  ASSERT_EQ(p3.size() ,5);
  
  ASSERT_EQ(p1[0],"foo");

  ASSERT_EQ(p2[0],"foo");
  ASSERT_EQ(p2[1],"bar");

  ASSERT_EQ(p3[0],"a");
  ASSERT_EQ(p3[1],"b");
  ASSERT_EQ(p3[2],"c");
  ASSERT_EQ(p3[3],"");
  ASSERT_EQ(p3[4],"e");

  ASSERT_FALSE(p0);
  ASSERT_TRUE(p1);
  ASSERT_TRUE(p2);
  ASSERT_TRUE(p3);
  ASSERT_FALSE(p0.single());
  ASSERT_TRUE(p0.empty());
  ASSERT_TRUE(p1.single());
  ASSERT_FALSE(p1.empty());
  ASSERT_FALSE(p2.single());
  ASSERT_FALSE(p2.empty());
  ASSERT_FALSE(p3.single());
  ASSERT_FALSE(p3.empty());
}


TEST(path,add){
  using namespace nodamushi::svd;
  path<> p0;
  path<> p1("foo");
  path<> p2("foo.bar");
  path<> p4("piyo");
  
  p0.add(p1);
  ASSERT_EQ(p0.size() ,1);
  ASSERT_EQ(p1.size() ,1);
  ASSERT_EQ(p0[0], "foo");
  ASSERT_EQ(p1[0], "foo");
  p0.add(p1).add(p2);

  ASSERT_EQ(p0.size() ,4);
  ASSERT_EQ(p1.size() ,1);
  ASSERT_EQ(p2.size() ,2);
  ASSERT_EQ(p0[0], "foo");
  ASSERT_EQ(p0[1], "foo");
  ASSERT_EQ(p0[2], "foo");
  ASSERT_EQ(p0[3], "bar");
  ASSERT_EQ(p1[0], "foo");
  ASSERT_EQ(p2[0], "foo");
  ASSERT_EQ(p2[1], "bar");
  
  path<> p5 = p4.append(p0);
  ASSERT_EQ(p0.size() ,4);
  ASSERT_EQ(p4.size() ,1);
  ASSERT_EQ(p5.size() ,5);
  ASSERT_EQ(p5[0], "piyo");
  ASSERT_EQ(p5[1], "foo");
  ASSERT_EQ(p5[2], "foo");
  ASSERT_EQ(p5[3], "foo");
  ASSERT_EQ(p5[4], "bar");

  path<> p2_1=p1.append(path<>("bar"));
  ASSERT_EQ(p2_1,p2);
  ASSERT_NE(p2_1,p1);
}

TEST(path,compare){
  using namespace nodamushi::svd;
  path<> pa("a");
  path<> pA("A");
  path<> pa_2("a");
  path<> paa("aa");
  path<> pb("b");
  path<> pB("B");
  path<> pc("c");
  path<> pa0("a0");
  path<> pa00("a00");
  path<> pa0a("a0a");
  path<> pa00a("a00a");
  path<> pa1("a1");
  path<> pa2("a2");
  path<> pa10("a10");
  path<> pa10a("a10a");
  path<> pa10b("a10b");
  
  path<> abc("abc");
  path<> ABC("ABC");
  path<> aaa("aaa");
  path<> Acc("Acc");
  
  path<> p_long("abc.def.gft.foo.bar");
  path<> p_long2("abc.def.gft.foo.bar");
  path<> p_long3("abc.def.gft.foo");
  path<> p_long4("abc.def.gft.foo.bar.piyo");

  ASSERT_EQ(pa,pa);
  ASSERT_EQ(pa,pa_2);
  ASSERT_NE(pa,pb);
  ASSERT_NE(pa,pA);
  ASSERT_NE(pa,pa0);
  ASSERT_EQ(p_long,p_long);
  ASSERT_EQ(p_long,p_long2);
  ASSERT_NE(p_long,p_long3);
  ASSERT_NE(p_long3,p_long);
  ASSERT_NE(p_long,p_long4);
  ASSERT_NE(p_long4,p_long);
  
  ASSERT_LT(pA,pa);
  ASSERT_LT(pA,pB);
  ASSERT_LT(pA,pb);
  // aaa < ABC <abc < Acc
  ASSERT_LT(ABC,abc); 
  ASSERT_LT(aaa,Acc);
  ASSERT_LT(abc,Acc);

  ASSERT_LT(pa,paa);
  ASSERT_LE(pa,paa);
  
  ASSERT_LE(pa,pa_2);
  ASSERT_LE(pa,pb);
  ASSERT_LE(pa,pc);
  ASSERT_LT(pa,pb);
  ASSERT_LT(pa,pc);
  ASSERT_LE(pa,pa0);
  ASSERT_LT(pa,pa0);
  ASSERT_LE(pa0,pa1);
  ASSERT_LT(pa0,pa1);
  ASSERT_LT(pa0,pa00);
  ASSERT_LT(pa00,pa0a);
  ASSERT_LT(pa0a,pa00a);
  ASSERT_LT(pa00,pa1);
  ASSERT_LT(pa0a,pa1);
  ASSERT_LE(pa0,pa2);
  ASSERT_LT(pa0,pa2);
  ASSERT_LE(pa1,pa1);
  ASSERT_LT(pa1,pa2);
  ASSERT_LE(pa1,pa10);
  ASSERT_LE(pa2,pa10);
  ASSERT_LT(pa2,pa10);
  ASSERT_LT(pa2,pa10a);
  ASSERT_LT(pa2,pa10b);
  ASSERT_LT(pa10a,pa10b);
  ASSERT_LT(pa1,paa);
  ASSERT_LT(pa10,paa);

  ASSERT_LT(p_long3,p_long4);

}
