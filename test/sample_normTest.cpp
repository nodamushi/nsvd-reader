#include <iostream>
#include "nodamushi/svd/boost.hpp"
#include "nodamushi/svd.hpp"
#include "nodamushi/svd/normalized.hpp"


#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <unordered_set>

#include "test_utility.hpp"
#include "compare.hpp"


namespace svd  =  nodamushi::svd;
namespace norm =  nodamushi::svd::normalized;
using did_t =typename norm::dim_info::id_t;
using Access = svd::Access;
using ReadAction = svd::ReadAction;
using AddressBlockUsage = svd::AddressBlockUsage;
using namespace std;


template<typename S>
void check_device(norm::Device<S>& v)
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
  ASSERT_EQ(v.peripherals.size(),3);
  std::unordered_set<did_t> sets;
  for(auto& p:v.peripherals){
    ASSERT_FALSE(sets.count(p.dim.id));
    sets.insert(p.dim.id);
  }
}

template<typename S>
void check_timer0(norm::Peripheral<S>& v)
{
  ASSERT_EQ(v.name, "TIMER0"s);
  ASSERT_FALSE(v.derivedFrom);
  ASSERT_EQ(v.version, "1.0"s);
  ASSERT_EQ(v.description, "timer"s);
  ASSERT_EQ(v.groupName, "TIMER"s);
  ASSERT_EQ(v.baseAddress, 0x40010000);
  ASSERT_FALSE(v.size);
  ASSERT_FALSE(v.resetValue);
  ASSERT_FALSE(v.resetMask);
  ASSERT_EQ(v.get_size(),32);
  ASSERT_EQ(v.get_resetValue(),0);
  ASSERT_EQ(v.get_resetMask(),0xFFFFFFFF);
  ASSERT_EQ(v.access,Access::READ_WRITE_ONCE);
  ASSERT_EQ(v.get_access(),Access::READ_WRITE_ONCE);
  ASSERT_EQ(v.addressBlock.size(),1);
  ASSERT_EQ(v.interrupt.size(),1);
  ASSERT_EQ(v.registers.size(),8 + 3); // 3: RELOAD[%s] 0-4
  did_t dim_id;
  std::unordered_set<did_t> sets;
  for(auto& r:v.registers){
    if(r.name == "RELOAD[0]"s){
      dim_id = r.dim.id;
      ASSERT_FALSE(sets.count(dim_id));
      sets.insert(dim_id);
    }else if(r.name == "RELOAD[1]"s){
      ASSERT_EQ(r.dim.id,dim_id);
    }else if(r.name == "RELOAD[2]"s){
      ASSERT_EQ(r.dim.id,dim_id);
    }else if(r.name == "RELOAD[3]"s){
      ASSERT_EQ(r.dim.id,dim_id);
    }else{
      ASSERT_FALSE(sets.count(r.dim.id)) << r.name << " dim:" << r.dim;
      sets.insert(r.dim.id);
    }
  }
  
}

template<typename S>
void check_timer0(norm::Interrupt<S>& v)
{
  ASSERT_EQ(v.name, "TIMER0"s);
  ASSERT_EQ(v.description, "Timer 0 interrupt"s);
  ASSERT_EQ(v.value, 0);
}

void check_timer0(norm::AddressBlock& v)
{
  ASSERT_EQ(v.offset, 0);
  ASSERT_EQ(v.size,0x100);
  ASSERT_EQ(v.usage, AddressBlockUsage::REGISTERS);
}

template<typename S>
void check_timer0_cr(norm::Register<S>& v)
{
  ASSERT_EQ(v.name, "CR"s);
  ASSERT_FALSE(v.dim);
  ASSERT_EQ(v.description,"Control Register"s);
  ASSERT_EQ(v.displayName,"Control Register"s);
  ASSERT_EQ(v.addressOffset,0);
  ASSERT_EQ(v.get_size(),32);
  ASSERT_EQ(v.get_access(),Access::READ_WRITE_ONCE);
  ASSERT_EQ(v.get_resetValue(),0);
  ASSERT_EQ(v.get_resetMask(),0x1337F7F);
  ASSERT_EQ(v.fields.size(),12);
  std::unordered_set<did_t> sets;
  for(auto& f:v.fields){
    ASSERT_FALSE(sets.count(f.dim.id));
    sets.insert(f.dim.id);
  }
}

template<typename S>
void check_foo100bar(norm::Register<S>& v)
{
  ASSERT_EQ(v.name, "FOO100BAR"s);
  ASSERT_TRUE(v.dim);
  ASSERT_TRUE(v.dim.is_seq());
  ASSERT_FALSE(v.dim.is_array());
  ASSERT_EQ(v.dim.index,1);
  ASSERT_EQ(v.description,"Control Register"s);
  ASSERT_EQ(v.displayName,"foo 100 bar register"s);
  ASSERT_EQ(v.addressOffset,0x70 + 4);
  ASSERT_EQ(v.get_size(),32);
  ASSERT_EQ(v.get_access(),Access::READ_WRITE);
  ASSERT_EQ(v.get_resetValue(),0);
  ASSERT_EQ(v.get_resetMask(),0x1337F7F);
  ASSERT_EQ(v.fields.size(),12);
  std::unordered_set<did_t> sets;
  for(auto& f:v.fields){
    ASSERT_FALSE(sets.count(f.dim.id));
    sets.insert(f.dim.id);
  }
}


template<typename S>
void check_timer0_reload(norm::Register<S>& v,int n)
{
  std::string dname  = "reload register[" + std::to_string(n) + "]";
  ASSERT_EQ(v.displayName, dname);
  ASSERT_TRUE(v.dim);
  ASSERT_TRUE(v.dim.is_array()) << v.dim;
  ASSERT_FALSE(v.dim.is_seq()) << v.dim;
  if(n==0)
    ASSERT_TRUE(v.dim.is_first());
  else
    ASSERT_FALSE(v.dim.is_first());
  if(n==3)
    ASSERT_TRUE(v.dim.is_last());
  else
    ASSERT_FALSE(v.dim.is_last());
  ASSERT_EQ(v.dim.index,n);
  ASSERT_EQ(v.addressOffset,0x50 + 4 * n);
  ASSERT_EQ(v.get_size(),32);
  ASSERT_EQ(v.get_access(),Access::READ_WRITE_ONCE);
  ASSERT_EQ(v.get_resetValue(),0x2C688);
  ASSERT_EQ(v.get_resetMask(),0xFFFFFFFF);
  ASSERT_EQ(v.get_address(),0x50 + 4 * n + (v.get_peripheral()->baseAddress));
  ASSERT_EQ(v.fields.size(),5);
  unsigned int i=0;
  did_t d;
  for(auto& f:v.fields){
    std::string fname = "R"+std::to_string(i);
    ASSERT_EQ(&v,f.get_parent().get());
    ASSERT_EQ(f.name,fname);
    ASSERT_TRUE(f.dim);
    ASSERT_FALSE(f.dim.is_array());
    ASSERT_TRUE(f.dim.is_seq());
    ASSERT_EQ(f.dim.index,i);
    if(i == 0){
      d = f.dim.id;
    }else{
      ASSERT_EQ(f.dim.id,d);
    }
    ASSERT_EQ(f.bit,norm::bit_range(i*3,i*3+2));
    ASSERT_EQ(f.get_access(),Access::READ_WRITE_ONCE);
    ASSERT_EQ(f.get_resetValue(),i);
    ASSERT_EQ(f.bit.width(),3);
    ASSERT_EQ(f.get_readAction(),ReadAction::NO_ACTION);
    i++;
  }
}

template<typename S>
void check_crr(norm::Register<S>& v)
{
  ASSERT_EQ(v.displayName, "hoge moge"s);
  ASSERT_FALSE(v.dim) << v.dim;
  ASSERT_FALSE(v.dim.is_array()) << v.dim;
  ASSERT_FALSE(v.dim.is_seq()) << v.dim;
  ASSERT_EQ(v.addressOffset,0x80);
  ASSERT_EQ(v.get_size(),32);
  ASSERT_EQ(v.get_access(),Access::READ_WRITE_ONCE);
  ASSERT_EQ(v.get_resetValue(),0x2C688);
  ASSERT_EQ(v.get_resetMask(),0xFFFFFFFF);
  ASSERT_EQ(v.get_address(),0x80 + (v.get_peripheral()->baseAddress));
  ASSERT_EQ(v.fields.size(),5+4);
  unsigned int i=0;
  did_t d,d2;
  for(auto& f:v.fields){
    ASSERT_EQ(&v,f.get_parent().get());
    if(i < 5){
      std::string fname = "R"+std::to_string(i);
      ASSERT_EQ(f.name,fname);
      ASSERT_TRUE(f.dim);
      ASSERT_FALSE(f.dim.is_array());
      ASSERT_TRUE(f.dim.is_seq());
      ASSERT_EQ(f.dim.index,i);
      if(i == 0){
        d = f.dim.id;
      }else{
        ASSERT_EQ(f.dim.id,d);
      }
      ASSERT_EQ(f.bit,norm::bit_range(i*3,i*3+2));
      ASSERT_EQ(f.get_access(),Access::READ_WRITE_ONCE);
      ASSERT_EQ(f.get_resetValue(),i);
      ASSERT_EQ(f.bit.width(),3);
      ASSERT_EQ(f.get_readAction(),ReadAction::NO_ACTION);
    }else if(i == 5){
      ASSERT_EQ(f.name,"foo"s);
      d2 = f.dim.id;
      ASSERT_NE(f.dim.id,d);
      ASSERT_FALSE(f.dim);
      ASSERT_EQ(f.bit,norm::bit_range(15,15));
    }else{
      std::string fname = "bar"s+std::to_string(i-6);
      ASSERT_EQ(f.name,fname);
      ASSERT_NE(f.dim.id,d);
      ASSERT_NE(f.dim.id,d2);
      ASSERT_TRUE(f.dim);
      ASSERT_EQ(f.dim.index ,i-6);
      ASSERT_FALSE(f.dim.is_array());
      ASSERT_TRUE(f.dim.is_seq());
      ASSERT_EQ(f.bit,norm::bit_range(16+(i-6)*2,16+(i-6)*2));
    }
    i++;
  }
}


template<typename S>void check_timer0_cr_en(norm::Field<S>& v)
{
  ASSERT_EQ(v.name, "EN"s);
  ASSERT_EQ(v.description,"Enable"s);
  ASSERT_EQ(v.bit,norm::bit_range(0,0));
  ASSERT_EQ(v.access,Access::READ_ONLY);
  ASSERT_EQ(v.enumeratedValues.size(),1);
  ASSERT_EQ(v.enumeratedValues[0].size(),2);
}

template<typename S>void check_timer0_cr_en_disable(norm::EnumeratedValue<S>& v)
{
  ASSERT_EQ(v.name, "Disable"s);
  ASSERT_EQ(v.description,"Timer is disabled and does not operate"s);
  ASSERT_EQ(v.get_value(),0);
}
template<typename S>void check_timer0_cr_en_enable(norm::EnumeratedValue<S>& v)
{
  ASSERT_EQ(v.name, "Enable"s);
  ASSERT_EQ(v.description,"Timer is enabled and can operate"s);
  ASSERT_EQ(v.get_value(),1);
}

template<typename S>void check_timer0_cr_rst(norm::Field<S>& v)
{
  ASSERT_EQ(v.name, "RST"s);
  ASSERT_EQ(v.description,"Reset Timer"s);
  ASSERT_EQ(v.bit,norm::bit_range(1,1));
  ASSERT_EQ(v.access,Access::WRITE_ONLY);
  ASSERT_EQ(v.enumeratedValues.size(),1);
  ASSERT_EQ(v.enumeratedValues[0].size(),2);
}
template<typename S>void check_timer0_cr_cnt(norm::Field<S>& v)
{
  ASSERT_EQ(v.name, "CNT"s);
  ASSERT_EQ(v.description,"Counting direction"s);
  ASSERT_EQ(v.bit,norm::bit_range(2,3));
  ASSERT_EQ(v.access,Access::WRITE_ONCE);
  ASSERT_EQ(v.enumeratedValues.size(),1);
  ASSERT_EQ(v.enumeratedValues[0].size(),4);
  uint64_t i= 0;
  for(norm::EnumeratedValue<S>& e:v.enumeratedValues[0]){
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
void check_timer2(norm::Peripheral<S>& v)
{
  ASSERT_EQ(v.name, "TIMER2"s);
  ASSERT_EQ(v.derivedFrom,"TIMER1"s);
  ASSERT_EQ(v.version, "1.0"s);
  ASSERT_EQ(v.description, "timer"s);
  ASSERT_EQ(v.groupName, "TIMER"s);

  ASSERT_EQ(v.baseAddress, 0x40010200);
  ASSERT_EQ(v.get_size(), 32);
  ASSERT_EQ(v.get_access(),Access::READ_WRITE_ONCE);
  ASSERT_EQ(v.addressBlock.size(),1);
  ASSERT_EQ(v.interrupt.size(),1);
  ASSERT_EQ(v.interrupt[0].name,"TIMER2"s);
  ASSERT_EQ(v.interrupt[0].value,6);
  ASSERT_EQ(v.registers.size(),(8+3) + 4 + 4 +1);
  did_t dim_id,dim_id1,dim_id2;
  std::unordered_set<did_t> sets;
  for(auto& r:v.registers){
    if(r.name == "RELOAD[0]"s){
      dim_id = r.dim.id;
      ASSERT_FALSE(sets.count(dim_id));
      sets.insert(dim_id);
    }else if(r.name == "RELOAD[1]"s){
      ASSERT_EQ(r.dim.id,dim_id);
      ASSERT_TRUE(r.dim);
      ASSERT_TRUE(r.dim.is_array());
      ASSERT_FALSE(r.dim.is_seq());
    }else if(r.name == "RELOAD[2]"s){
      ASSERT_EQ(r.dim.id,dim_id);
      ASSERT_TRUE(r.dim);
      ASSERT_TRUE(r.dim.is_array());
      ASSERT_FALSE(r.dim.is_seq());
    }else if(r.name == "RELOAD[3]"s){
      ASSERT_EQ(r.dim.id,dim_id);
      ASSERT_TRUE(r.dim);
      ASSERT_TRUE(r.dim.is_array());
      ASSERT_FALSE(r.dim.is_seq());
    }else if(r.name == "FOO0BAR"s){
      dim_id1 = r.dim.id;
      ASSERT_FALSE(sets.count(dim_id1));
      sets.insert(dim_id1);
      ASSERT_TRUE(r.dim);
      ASSERT_FALSE(r.dim.is_array());
      ASSERT_TRUE(r.dim.is_seq());
      ASSERT_EQ(r.dim.index,0);
    }else if(r.name == "FOO1BAR"s){
      ASSERT_EQ(r.dim.id,dim_id1);
      ASSERT_TRUE(r.dim);
      ASSERT_FALSE(r.dim.is_array());
      ASSERT_TRUE(r.dim.is_seq());
      ASSERT_EQ(r.dim.index,1);
    }else if(r.name == "FOO2BAR"s){
      ASSERT_EQ(r.dim.id,dim_id1);
      ASSERT_TRUE(r.dim);
      ASSERT_FALSE(r.dim.is_array());
      ASSERT_TRUE(r.dim.is_seq());
      ASSERT_EQ(r.dim.index,2);
    }else if(r.name == "FOO3BAR"s){
      ASSERT_EQ(r.dim.id,dim_id1);
      ASSERT_TRUE(r.dim);
      ASSERT_FALSE(r.dim.is_array());
      ASSERT_TRUE(r.dim.is_seq());
      ASSERT_EQ(r.dim.index,3);
    }else if(r.name == "FOOaaaaaBAR"s){
      dim_id2 = r.dim.id;
      ASSERT_FALSE(sets.count(dim_id2));
      sets.insert(dim_id2);
      ASSERT_TRUE(r.dim);
      ASSERT_FALSE(r.dim.is_array());
      ASSERT_TRUE(r.dim.is_seq());
      ASSERT_EQ(r.dim.index,0);
    }else if(r.name == "FOO100BAR"s){
      ASSERT_EQ(r.dim.id,dim_id2);
      ASSERT_TRUE(r.dim);
      ASSERT_FALSE(r.dim.is_array());
      ASSERT_TRUE(r.dim.is_seq());
      ASSERT_EQ(r.dim.index,1);
    }else if(r.name == "FOO101BAR"s){
      ASSERT_EQ(r.dim.id,dim_id2);
      ASSERT_TRUE(r.dim);
      ASSERT_FALSE(r.dim.is_array());
      ASSERT_TRUE(r.dim.is_seq());
      ASSERT_EQ(r.dim.index,2);
    }else if(r.name == "FOO102BAR"s){
      ASSERT_EQ(r.dim.id,dim_id2);
      ASSERT_TRUE(r.dim);
      ASSERT_FALSE(r.dim.is_array());
      ASSERT_TRUE(r.dim.is_seq());
      ASSERT_EQ(r.dim.index,3);
    }else{
      ASSERT_FALSE(sets.count(r.dim.id)) << r.name << " dim:" << r.dim;
      sets.insert(r.dim.id);
    }
  }

}

template<typename S>
void check_timer2_foobar(norm::Register<S>& v,int count)
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
  ASSERT_EQ(v.resetValue,0);
  ASSERT_EQ(v.resetMask,0xFFFFFFFF);
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


template<typename S>
void check_timer1(norm::Peripheral<S>& v)
{
  ASSERT_EQ(v.name, "TIMER1"s);
  ASSERT_EQ(v.derivedFrom,"TIMER0"s);
  ASSERT_EQ(v.version, "1.0"s);
  ASSERT_EQ(v.description, "timer"s);
  ASSERT_EQ(v.groupName, "TIMER"s);
  ASSERT_EQ(v.baseAddress, 0x40010100);
  ASSERT_EQ(v.get_size(), 32);
  ASSERT_EQ(v.get_access(),Access::READ_WRITE_ONCE);
  ASSERT_EQ(v.addressBlock.size(),1);
  ASSERT_EQ(v.interrupt.size(),1);
  ASSERT_EQ(v.registers.size(),11);
  ASSERT_EQ(v.interrupt[0].name,"TIMER1"s);
  ASSERT_EQ(v.interrupt[0].value,4);
  did_t dim_id;
  std::unordered_set<did_t> sets;
  for(auto& r:v.registers){
    if(r.name == "RELOAD[0]"s){
      dim_id = r.dim.id;
      ASSERT_FALSE(sets.count(dim_id));
      sets.insert(dim_id);
    }else if(r.name == "RELOAD[1]"s){
      ASSERT_EQ(r.dim.id,dim_id);
    }else if(r.name == "RELOAD[2]"s){
      ASSERT_EQ(r.dim.id,dim_id);
    }else if(r.name == "RELOAD[3]"s){
      ASSERT_EQ(r.dim.id,dim_id);
    }else{
      ASSERT_FALSE(sets.count(r.dim.id)) << r.name << " dim:" << r.dim;
      sets.insert(r.dim.id);
    }
  }

}




struct norm_visitor
{
  //-------------------------------
  using this_t =  norm_visitor ;
#   undef  VISITOR_MEMBER
#   define VISITOR_MEMBER
    
  NORM_VISITOR_INIT;
  //-------------------------------
  bool device=false;
  bool timer0=false;
  bool timer1=false;
  bool timer2=false;
  int r_cr = 0;
  int r_reload0 = 0;
  int r_reload1 = 0;
  int r_reload2 = 0;
  int r_reload3 = 0;
  int f_en = 0;
  int f_rst = 0;
  int f_cnt = 0;
  int f_mode=0;
  int f_psc=0;
  int f_cntsrc=0;
  int f_capsrc=0;
  int f_capedge=0;
  int f_trgext=0;
  int f_reload=0;
  int f_idr = 0;
  int f_s = 0;
  int f_RSTn = true;

  std::string pname;

  NORM_Visit_Device(v){
    device = true;
    check_device(v);
    return CONTINUE;
  }
  NORM_Visit_Peripheral(p){
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
      check_timer1(p);
    }
    return CONTINUE;
  }

  NORM_PostVisit_Peripheral(p){
    pname ="";
    return CONTINUE;
  }

    
  NORM_Visit_Interrupt(v){
    if(pname == "TIMER0"s){
      check_timer0(v);
    }
    return CONTINUE;
  }
    
  NORM_Visit_AddressBlock(v){
    if(pname == "TIMER0"s){
      check_timer0(v);
    }
    return CONTINUE;
  }
  std::string reg="";
  int foobar=0;
  int r_crr=0;
  int r_foo100bar=0;
  NORM_Visit_Register(v){
    if(v.name == "CR"s){
      reg = "CR";
      r_cr++;
      check_timer0_cr(v);
    }else if(v.name == "RELOAD[0]"){
      r_reload0++;
      check_timer0_reload(v,0);
    }else if(v.name == "RELOAD[1]"){
      r_reload1++;
      check_timer0_reload(v,1);
    }else if(v.name == "RELOAD[2]"){
      r_reload2++;
      check_timer0_reload(v,2);
    }else if(v.name == "RELOAD[3]"){
      r_reload3++;
      check_timer0_reload(v,3);
    }else if(v.name=="CRR"s){
      r_crr++;
      check_crr(v);
    }else if(v.name=="FOO100BAR"s){
      r_foo100bar++;
      check_foo100bar(v);
    }else{
      std::cout << "     TODO: test-register:" <<v.get_path()<< std::endl;
    }
    return CONTINUE;
  }
  NORM_PostVisit_Register(v){
    reg = "";
    return CONTINUE;
  }
  std::string fname="";
  NORM_Visit_Field(v){

    if(reg == "CR"s){
      if(v.name == "EN"s){
        fname = "EN";
        f_en++;
        check_timer0_cr_en(v);
      }else if(v.name == "RST"s){
        f_rst++;
        check_timer0_cr_rst(v);
      }else if(v.name == "CNT"s){
        f_cnt++;
        check_timer0_cr_cnt(v);
      }else if(v.name == "MODE"s){
        f_mode++;
      }else if(v.name == "PSC"s){
        f_psc++;
      }else if(v.name == "CNTSRC"s){
        f_cntsrc++;
      }else if(v.name == "CAPSRC"s){
        f_capsrc++;
      }else if(v.name == "CAPEDGE"s){
        f_capedge++;
      }else if(v.name == "TRGEXT"s){
        f_trgext++;
      }else if(v.name == "RELOAD"s){
        f_reload++;
      }else if(v.name == "IDR"s){
        f_idr++;
      }else if(v.name == "S"s){
        f_s++;
      }
    }
    return CONTINUE;
  }
  NORM_PostVisit_Field(v){
    fname = "";
    return CONTINUE;
  }
  NORM_Visit_EnumeratedValues(v){ // field.enumeratedValues
    return CONTINUE;
  }
  int e_disable=0;
  int e_enable=0;
  NORM_Visit_EnumeratedValue(v){ // field.enumeratedValues
    if(reg == "CR"s && fname=="EN"s){
      if(v.name == "Disable"s){
        e_disable++;
        check_timer0_cr_en_disable(v);
      }else if(v.name=="Enable"s){
        e_enable++;
        check_timer0_cr_en_enable(v);
      }
    }
    return CONTINUE;
  }
    
};

TEST(sample_readTest,norm)
{
  auto fname = find_file("data/test","sample.svd");
  ASSERT_TRUE(!fname.empty());
  svd::boost_svd_reader svd(fname);
  ASSERT_TRUE(svd.ok());
  svd::Device<> device(svd);
  ASSERT_TRUE(svd.ok());
  
  auto ptr = svd::normalize(device);
  
  ASSERT_TRUE(ptr);

  norm_visitor v;

  norm_visitor::accept(ptr,v);

  ASSERT_TRUE(v.device);
  ASSERT_TRUE(v.timer0);
  ASSERT_TRUE(v.timer1);
  ASSERT_TRUE(v.timer2);
  ASSERT_EQ(v.r_cr,3);
  ASSERT_EQ(v.f_en,3);
  ASSERT_EQ(v.f_rst,3);
  ASSERT_EQ(v.f_cnt,3);
  ASSERT_EQ(v.f_mode,3);
  ASSERT_EQ(v.f_psc,3);
  ASSERT_EQ(v.f_cntsrc,3);
  ASSERT_EQ(v.f_capsrc,3);
  ASSERT_EQ(v.f_capedge,3);
  ASSERT_EQ(v.f_trgext,3);
  ASSERT_EQ(v.f_reload,3);
  ASSERT_EQ(v.f_idr,3);
  ASSERT_EQ(v.f_s,3);
  ASSERT_EQ(v.e_disable,3);
  ASSERT_EQ(v.e_enable,3);
  ASSERT_EQ(v.r_reload0,3);
  ASSERT_EQ(v.r_reload1,3);
  ASSERT_EQ(v.r_reload2,3);
  ASSERT_EQ(v.r_reload3,3);
  ASSERT_EQ(v.r_crr,1);
  ASSERT_EQ(v.r_foo100bar,1);
  ASSERT_EQ(v.fname,"");
  ASSERT_EQ(v.reg,"");
  ASSERT_EQ(v.pname,"");

  ASSERT_TRUE(v.f_RSTn);
}

