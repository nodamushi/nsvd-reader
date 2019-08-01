#ifndef __COMPARE_HPP__
#define __COMPARE_HPP__

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
struct xml_element
{
  template<typename SVD>
  xml_element(SVD& s):
      name(s.get_name()),
      value(s.get_value()),
      attributes{},
      children{}
  {
    while(s){
      decltype(s.next_child()) c = s.next_child();
      if(c.is_attribute()){
        if(c.get_name() == "<xmlattr>"){//for boost
          while(c){
            decltype(s.next_child()) c2 = c.next_child();
            std::string name(c2.get_name());
            std::string value (c2.get_value());
            attributes.emplace(name,value);
          }
        }else{
          std::string name(c.get_name());
          std::string value(c.get_value());
          attributes.emplace(name,value);
        }
      }else{
        children.emplace_back(c);
      }
    }
  }
  static int is_number(const std::string& a)
  {
    if(std::all_of(a.begin(),a.end(),isdigit))
      return 10;
    if(a.length() > 2 && a[0] == '0'){
      if(a[1] =='x' || a[1] =='X'){
        if(std::all_of(a.begin()+2,a.end(),
                       [](char c){return 
                             ('0'<=c && c <='9')||
                             ('a'<=c && c <='f')||
                             ('A'<=c && c <='F');}))
          return 16;
      }else if(a[1] =='b' || a[1] =='B'){
        if(std::all_of(a.begin()+2,a.end(),
                       [](char c){return c == '0' || c == '1';}))
          return 2;
      }
    }
    return 0;
  }
  static bool compare_value(const std::string& a,const std::string& b)
  {
    if(a == b){
      return true;
    }
    int rada=is_number(a);
    if(rada==0)return false;
    int radb=is_number(b);
    if(radb==0)return false;
    unsigned long long 
        na=std::stoull(rada!=10?a.substr(2):a,nullptr,rada),
        nb=std::stoull(radb!=10?b.substr(2):b,nullptr,radb);
    return na == nb;
  }
  /**
   * compare with ignoring order
   */
  bool equals(const xml_element& e)const
  {
    if(e.name == name && compare_value(e.value,value)
       && attributes.size() == e.attributes.size()
       // skip check to show which element is not found
       //&& children.size() == e.children.size() 
       ){
      for(auto p : attributes){
        auto& a = p.first;
        auto& v = p.second;
        auto i = e.attributes.find(a);
        if(i==e.attributes.end()){
          return false;
        }
        if(i->second != v){
          return false;
        }
      }
      
      std::vector<const xml_element*> cptr;
      cptr.reserve(children.size());
      std::string n1="";
      for(const xml_element& c:children){
        cptr.push_back(&c);
        if(c.name == "name"){
          n1 = c.value;
        }
      }
      std::string n2="";
      for(const xml_element& c:e.children){
        if(c.name == "name"){
          n2 = c.value;
          break;
        }
      }
      for(const xml_element& c:e.children){
        auto i = cptr.begin(),e =cptr.end();
        bool found=false;
        while(i!=e){
          if(c.equals(**i)){
            found = true;
            break;
          }
          i++;
        }
        if(found){
          cptr.erase(i);
        }else{
          std::cout <<name<< "."<< c.name 
                    <<"("<<n1<<","<<n2<<","<<c.value<< ") not found" << std::endl;
          return false;
        }
      }
      if(cptr.size()!=0){
        for(const xml_element* c:cptr){
          std::cout << name << "." << (c->name) << " not found" << std::endl;
        }
        return false;
      }
      return true;
    }
    return false;
  }
  
  std::string name;
  std::string value;
  std::unordered_map<std::string,std::string> attributes;
  std::vector<xml_element> children;
  
};



#endif


