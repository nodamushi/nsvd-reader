#include <iostream>
#include "nodamushi/svd/boost.hpp"
#include "nodamushi/svd.hpp"
#include "nodamushi/svd/normalized.hpp"


#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "test_utility.hpp"
#include "compare.hpp"


using namespace nodamushi::svd;
using namespace std;



void check_device(Device<>& v)
{
  ASSERT_EQ(v.vendor, "nodamushi"s);
  ASSERT_EQ(v.vendorID,"nodamushi"s);
  ASSERT_EQ(v.name,"sample"s);
  ASSERT_EQ(v.version,"1.0"s);
  ASSERT_EQ(v.description,"Sample SVD for test"s);
  ASSERT_EQ(v.addressUnitBits,8);
  ASSERT_EQ(v.width,32);
  ASSERT_EQ(v.size,32);
  ASSERT_EQ(v.access,Access::READ_WRITE);
  ASSERT_EQ(v.resetValue,0);
  ASSERT_EQ(v.resetMask,0xFFFFFFFF); 
}

template<typename S>
void check_timer0(Peripheral<S>& v)
{
  ASSERT_EQ(v.name, "TIMER0"s);
  ASSERT_FALSE(v.derivedFrom);
  ASSERT_EQ(v.version, "1.0"s);
  ASSERT_EQ(v.description, "timer"s);
  ASSERT_EQ(v.groupName, "TIMER"s);
  ASSERT_EQ(v.baseAddress, 0x40010000);
  ASSERT_FALSE(v.size);
  ASSERT_EQ(v.access,Access::READ_WRITE_ONCE);
  ASSERT_TRUE(v.addressBlock);
  ASSERT_EQ(v.addressBlock->size(),1);
  ASSERT_TRUE(v.interrupt);
  ASSERT_EQ(v.interrupt->size(),1);
  ASSERT_TRUE(v.registers);
  ASSERT_EQ(v.registers->size(),8);
}

template<typename S>
void check_timer0(Interrupt<S>& v)
{
  ASSERT_EQ(v.name, "TIMER0"s);
  ASSERT_EQ(v.description, "Timer 0 interrupt"s);
  ASSERT_EQ(v.value, 0);
}

void check_timer0(AddressBlock& v)
{
  ASSERT_EQ(v.offset, 0);
  ASSERT_EQ(v.size,0x100);
  ASSERT_EQ(v.usage, AddressBlockUsage::REGISTERS);
}

template<typename S>
void check_timer0_cr(Register<S>& v)
{
  ASSERT_EQ(v.name, "CR"s);
  ASSERT_EQ(v.description,"Control Register"s);
  ASSERT_EQ(v.displayName,"Control Register"s);
  ASSERT_EQ(v.addressOffset,0);
  ASSERT_EQ(v.size,32);
  ASSERT_EQ(v.access,Access::READ_WRITE_ONCE);
  ASSERT_EQ(v.resetValue,0);
  ASSERT_EQ(v.resetMask,0x1337F7F);
  ASSERT_EQ(v.fields->size(),12);
}

template<typename S>void check_timer0_cr_en(Field<S>& v)
{
  ASSERT_EQ(v.name, "EN"s);
  ASSERT_EQ(v.description,"Enable"s);
  ASSERT_EQ(v.bitRange,bitRange(0,0));
  ASSERT_EQ(v.access,Access::READ_ONLY);
  ASSERT_EQ(v.enumeratedValues->size(),1);
  ASSERT_EQ((*v.enumeratedValues)[0].size(),2);
}

template<typename S>void check_timer0_cr_en_disable(EnumeratedValue<S>& v)
{
  ASSERT_EQ(v.name, "Disable"s);
  ASSERT_EQ(v.description,"Timer is disabled and does not operate"s);
  ASSERT_EQ(v.get_value(),0);
}
template<typename S>void check_timer0_cr_en_enable(EnumeratedValue<S>& v)
{
  ASSERT_EQ(v.name, "Enable"s);
  ASSERT_EQ(v.description,"Timer is enabled and can operate"s);
  ASSERT_EQ(v.get_value(),1);
}

template<typename S>void check_timer0_cr_rst(Field<S>& v)
{
  ASSERT_EQ(v.name, "RST"s);
  ASSERT_EQ(v.description,"Reset Timer"s);
  ASSERT_EQ(v.bitOffset,1);
  ASSERT_EQ(v.bitWidth,1);
  ASSERT_EQ(v.access,Access::WRITE_ONLY);
  ASSERT_EQ(v.enumeratedValues->size(),1);
  ASSERT_EQ((*v.enumeratedValues)[0].size(),2);
}
template<typename S>void check_timer0_cr_cnt(Field<S>& v)
{
  ASSERT_EQ(v.name, "CNT"s);
  ASSERT_EQ(v.description,"Counting direction"s);
  ASSERT_EQ(v.lsb,2);
  ASSERT_EQ(v.msb,3);
  ASSERT_EQ(v.access,Access::WRITE_ONCE);
  ASSERT_EQ(v.enumeratedValues->size(),1);
  ASSERT_EQ((*v.enumeratedValues)[0].size(),4);
  uint64_t i= 0;
  for(EnumeratedValue<S>& e:v.enumeratedValues.get()[0]){
    if(i < 3){
      char n[] = {'C',(char)('0'+i),0};
      std::string nn(n);
      ASSERT_EQ(e.name,nn);
      ASSERT_FALSE(e.is_default());
      ASSERT_EQ(e.get_value(),i);
    }else{
      ASSERT_EQ(e.name,"reserved"s);
      ASSERT_TRUE(e.is_default());
    }
    i++;
  }
}


template<typename S>
void check_timer2(Peripheral<S>& v)
{
  ASSERT_EQ(v.name, "TIMER2"s);
  ASSERT_EQ(v.derivedFrom.get(),"TIMER1"s);
  ASSERT_FALSE(v.version);
  ASSERT_FALSE(v.description);
  ASSERT_FALSE(v.groupName);
  ASSERT_EQ(v.baseAddress, 0x40010200);
  ASSERT_FALSE(v.size);
  ASSERT_FALSE(v.access);
  ASSERT_FALSE(v.addressBlock);
  ASSERT_TRUE(v.interrupt);
  ASSERT_EQ(v.interrupt->size(),1);
  ASSERT_TRUE(v.registers);
  ASSERT_EQ(v.registers->size(),3);
}

template<typename S>
void check_timer2_foobar(Register<S>& v,int count)
{
  ASSERT_EQ(v.name, "FOO%sBAR"s);
  if(count == 0){
    ASSERT_EQ(v.addressOffset,0x60);
    ASSERT_FALSE(v.derivedFrom);
  }else if(count == 1){
    ASSERT_EQ(v.addressOffset,0x70);
    ASSERT_EQ(v.derivedFrom.get(),"TIMER0.CR");
  }
  ASSERT_EQ(v.size,32);
  ASSERT_EQ(v.access,Access::READ_WRITE);
  if(count == 0){
    ASSERT_EQ(v.resetValue,0);
    ASSERT_EQ(v.resetMask,0xFFFFFFFF);
  }else{
    ASSERT_FALSE(v.resetValue);
    ASSERT_FALSE(v.resetMask);
  }
  ASSERT_FALSE(v.fields);
  ASSERT_EQ(v.dim,4);
  ASSERT_EQ(v.dimIncrement,4);
  if(count == 0){
    ASSERT_FALSE(v.dimIndex);
  }else{
    ASSERT_TRUE(v.dimIndex);
    ASSERT_TRUE(v.dimIndex->contains("aaaaa"));
    ASSERT_FALSE(v.dimIndex->contains("99"));
    ASSERT_TRUE(v.dimIndex->contains("100"));
    ASSERT_TRUE(v.dimIndex->contains("101"));
    ASSERT_TRUE(v.dimIndex->contains("102"));
    ASSERT_TRUE(v.dimIndex->contains("103"));
    ASSERT_FALSE(v.dimIndex->contains("104"));
  }
}



struct svd_visitor
{
  //-------------------------------
  using this_t =  svd_visitor ;
#   undef  VISITOR_MEMBER
#   define VISITOR_MEMBER
    
  SVD_VISITOR_INIT;
  //-------------------------------
  bool device=false;
  bool timer0=false;
  bool r_cr = false;
  bool f_en = false;
  bool f_rst = false;
  bool f_cnt = false;
  bool f_mode=false;
  bool f_psc=false;
  bool f_cntsrc=false;
  bool f_capsrc=false;
  bool f_capedge=false;
  bool f_trgext=false;
  bool f_reload=false;
  bool f_idr = false;
  bool f_s = false;
  bool f_RSTn = true;
  bool timer1=false;
  bool timer2=false;

  std::string pname;

  SVD_Visit_Device(v){
    device = true;
    check_device(v);
    return CONTINUE;
  }
  SVD_Visit_Peripheral(p){
    if(p.name == "TIMER0"s){
      timer0=true;
      pname = "TIMER0";
      check_timer0(p);
    }else if(p.name=="TIMER2"s){
      timer2=true;
      pname = "TIMER2";
      check_timer2(p);
    }else if(p.name=="TIMER1"s){
      timer1=true;
      pname = "TIMER1";
    }
    
    return CONTINUE;
  }

  SVD_PostVisit_Peripheral(p){
    pname ="";
    return CONTINUE;
  }

    
  SVD_Visit_Interrupt(v){
    if(pname == "TIMER0"s){
      check_timer0(v);
    }
    return CONTINUE;
  }
    
  SVD_Visit_AddressBlock(v){
    if(pname == "TIMER0"s){
      check_timer0(v);
    }
    return CONTINUE;
  }
  std::string reg="";
  int foobar=0;
  SVD_Visit_Register(v){
    if(pname=="TIMER0"s){
      if(v.name == "CR"s){
        reg = "CR";
        r_cr = true;
        check_timer0_cr(v);
      }
    }else if(pname == "TIMER2"s){
      if(v.name == "FOO%sBAR"s){
        check_timer2_foobar(v,foobar++);
      }else if(v.name == "CRR"s){
        return SKIP;
      }
    }else{
      std::cout << "     register:" <<*v.name<< std::endl;
    }
    return CONTINUE;
  }
  SVD_PostVisit_Register(v){
    reg = "";
    return CONTINUE;
  }
  std::string fname="";
  SVD_Visit_Field(v){
    if(pname == "TIMER0"s){
      if(reg == "CR"s){
        if(v.name == "EN"s){
          fname = "EN";
          f_en = true;
          check_timer0_cr_en(v);
        }else if(v.name == "RST"s){
          f_rst = true;
          check_timer0_cr_rst(v);
        }else if(v.name == "CNT"s){
          f_cnt = true;
          check_timer0_cr_cnt(v);
        }else if(v.name == "MODE"s){
          f_mode=true;
        }else if(v.name == "PSC"s){
          f_psc=true;
        }else if(v.name == "CNTSRC"s){
          f_cntsrc=true;
        }else if(v.name == "CAPSRC"s){
          f_capsrc=true;
        }else if(v.name == "CAPEDGE"s){
          f_capedge=true;
        }else if(v.name == "TRGEXT"s){
          f_trgext=true;
        }else if(v.name == "RELOAD"s){
          f_reload=true;
        }else if(v.name == "IDR"s){
          f_idr = true;
        }else if(v.name == "S"s){
          f_s = true;
        }
      }
    }else if(v.name=="RSTn"s){
      f_RSTn = false;
    }
    return CONTINUE;
  }
  SVD_PostVisit_Field(v){
    fname = "";
    return CONTINUE;
  }
  SVD_Visit_EnumeratedValues(v){ // field.enumeratedValues
    return CONTINUE;
  }
  bool e_disable=false;
  bool e_enable=false;
  SVD_Visit_EnumeratedValue(v){ // field.enumeratedValues
    if(fname=="EN"s){
      if(v.name == "Disable"s){
        e_disable=true;
        check_timer0_cr_en_disable(v);
      }else if(v.name=="Enable"s){
        e_enable=true;
        check_timer0_cr_en_enable(v);
      }
    }
    return CONTINUE;
  }
    
};

TEST(sample_readTest,svd)
{
  auto fname = find_file("data/test","sample.svd");
  ASSERT_TRUE(!fname.empty());
  boost_svd_reader svd(fname);
  ASSERT_TRUE(svd.ok());
  Device<> device(svd);
  ASSERT_TRUE(svd.ok());
  std::stringstream ss;
  ss << device;
  
  boost_svd_reader svd1(fname);
  boost_svd_reader svd2(ss);
  xml_element e1(svd1);
  xml_element e2(svd2);
  
  ASSERT_TRUE(e1.equals(e2)) << ss.str();
  svd_visitor v;
  ASSERT_FALSE(v.device);
  ASSERT_FALSE(v.timer0);
  ASSERT_FALSE(v.r_cr);
  ASSERT_FALSE(v.f_en);
  ASSERT_FALSE(v.f_rst);
  ASSERT_FALSE(v.f_cnt);
  ASSERT_FALSE(v.f_mode);
  ASSERT_FALSE(v.f_psc);
  ASSERT_FALSE(v.f_cntsrc);
  ASSERT_FALSE(v.f_capsrc);
  ASSERT_FALSE(v.f_capedge);
  ASSERT_FALSE(v.f_trgext);
  ASSERT_FALSE(v.f_reload);
  ASSERT_FALSE(v.f_idr);
  ASSERT_FALSE(v.f_s);
  ASSERT_FALSE(v.e_disable);
  ASSERT_FALSE(v.e_enable);

  svd_visitor::accept(device,v);

  ASSERT_TRUE(v.device);
  ASSERT_TRUE(v.timer0);
  ASSERT_TRUE(v.timer1);
  ASSERT_TRUE(v.timer2);
  ASSERT_TRUE(v.r_cr);
  ASSERT_TRUE(v.f_en);
  ASSERT_TRUE(v.f_rst);
  ASSERT_TRUE(v.f_cnt);
  ASSERT_TRUE(v.f_mode);
  ASSERT_TRUE(v.f_psc);
  ASSERT_TRUE(v.f_cntsrc);
  ASSERT_TRUE(v.f_capsrc);
  ASSERT_TRUE(v.f_capedge);
  ASSERT_TRUE(v.f_trgext);
  ASSERT_TRUE(v.f_reload);
  ASSERT_TRUE(v.f_idr);
  ASSERT_TRUE(v.f_s);
  ASSERT_TRUE(v.e_disable);
  ASSERT_TRUE(v.e_enable);
  ASSERT_EQ(v.fname,"");
  ASSERT_EQ(v.reg,"");
  ASSERT_EQ(v.pname,"");
  ASSERT_EQ(v.foobar,2);
  ASSERT_TRUE(v.f_RSTn);
}

