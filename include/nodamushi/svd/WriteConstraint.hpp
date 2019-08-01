/*!
  @brief &lt;writeConstraint&gt; element
  @file nodamushi/svd/WriteConstraint.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_WRITECONSTRAINT_HPP__
#define __NODAMUSHI_SVD_WRITECONSTRAINT_HPP__

# include <ostream>
# include "nodamushi/svd/node.hpp"

# include "nodamushi/svd/create.hpp"
# include "nodamushi/const_string.hpp"
# include "nodamushi/svd/svd_printer.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief &lt;writeConstraint&gt;.&lt;range&gt; element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_writeConstraint
 */
struct WriteConstraintRange :public svd_node
{
  //! @brief &lt;minimum&gt;the smallest number to be written to the field. 
  SVD_VALUE(uint64_t,minimum,true);
  //! @brief &lt;maximum&gt;the largest number to be written to the field. 
  SVD_VALUE(uint64_t,maximum,true);
};

/**
 * @brief writeConstraint element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_writeConstraint
 */
struct WriteConstraint :public svd_node
{
  //! @brief &lt;writeAsRead&gt;If true, only the last read value can be written. 
  SVD_VALUE(bool,writeAsRead);
  //! @brief &lt;useEnueratedValues&gt;If true, only the values listed in the enumeratedValues list can be written. 
  SVD_VALUE(bool,useEnumeratedValues);
  //! @brief &lt;range&gt;(minimum,maximum)
  SVD_VALUE(WriteConstraintRange,range);
};

//-------------------------------------------------------------

template<typename SVD>
bool create(SVD& svd,WriteConstraintRange& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t = true;
    if(n == "minimum") t = (d.minimum = c);
    else if(n == "maximum") t = (d.maximum = c);
    if(!t){
      //TODO error
    }
  }
  return true;
}

template<>
struct svd_printer<WriteConstraintRange>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const WriteConstraintRange& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name << ">"<<std::endl;

    int i= indent + 1;
    print_svd_value(o,v.minimum,i,true);
    print_svd_value(o,v.maximum,i,true);
    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};


template<typename SVD>
bool create(SVD& svd,WriteConstraint& d)
{
  while(svd)
  {
    decltype(svd.next_child()) c = svd.next_child();
    string_ref n = c.get_name();
    bool t=true;
    if(n == "writeAsRead") t = (d.writeAsRead = c);
    else if(n == "useEnumeratedValues") t = (d.useEnumeratedValues = c);
    else if(n == "range")t = (d.range = c);
    if(!t){
      //TODO error
    }
  }
  return true;
}

template<>
struct svd_printer<WriteConstraint>{
  static std::ostream& print(
      std::ostream& o,const_string name,
      const WriteConstraint& v,
      int indent)
  {
    o << get_indent(indent)<<"<" << name << ">"<<std::endl;

    int i= indent + 1;
    print_svd_value(o,v.writeAsRead,i,true);
    print_svd_value(o,v.useEnumeratedValues,i,true);
    print_svd_value(o,v.range,i,true);

    o << get_indent(indent)<<"</" << name << ">";
    return o;
  }
};

} // end namespace svd
}// end namespace nodamushi
#endif // __NODAMUSHI_SVD_WRITECONSTRAINT_HPP__
