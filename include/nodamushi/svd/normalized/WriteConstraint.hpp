/*!
  @brief normalized &lt;writeConstraint&gt; element
  @file nodamushi/svd/normalized/WriteConstraint.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_NORMALIZED_WRITECONSTRAINT_HPP
#define NODAMUSHI_SVD_NORMALIZED_WRITECONSTRAINT_HPP

# include <cstdint>

namespace nodamushi{
namespace svd{
namespace normalized{

/**
 @brief normalized &lt;writeConstraint&gt;.&lt;range&gt; element
 @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_writeConstraint
 @see nodamushi::svd::WriteConstraint
 */
struct WriteConstraint
{
  /**
   * @brief @brief &lt;writeConstraint&gt;.&lt;range&gt; element
   */
  struct range_t
  {
    /**
       @brief Indicates whether this data is valid.
       @par
       When valid is false, this instance has no value.
    */
    bool valid;
    //! @brief return valid
    operator bool()const noexcept{return valid;}
    
    //! @brief range maxmmum.this value is enable when range is true.
    uint64_t maximum;
    //! @brief range minimum.this value is enable when range is true.
    uint64_t minimum;
    range_t():valid(false),maximum(~(uint64_t)0),minimum(0){}

    void init(uint64_t max,uint64_t min)noexcept
    {
      valid = true;
      maximum = max;
      minimum = min;
    }
    void clear()noexcept
    {
      valid = false;
      maximum = ~(uint64_t)0;
      minimum = 0;
    }
  };


  /**
    @brief Indicates whether this data is valid.
    @par
    When valid is false, this instance has no value.
  */
  bool valid; 
  //! @brief return valid
  operator bool()const noexcept{return valid;}
  //---------------------------------------------------
  /**
   * @brief &lt;writeAsRead&gt;If true, only the last read value can be written. 
   * @see valid
   */
  bool writeAsRead; 
  /**
   * @brief &lt;useEnueratedValues&gt;If true, only the values listed in the enumeratedValues list can be written. 
   * @see valid
   */
  bool useEnumeratedValues;
  
  /**
   * @brief &lt;range&gt;
   * @see valid
   * @see range_t.valid
   */
  range_t range;

  template<typename SVD,typename VALUE,VALUE SVD::* MEMBER>
  WriteConstraint(derived_from_helper<SVD,VALUE,MEMBER>&& d):
      WriteConstraint(d.ptr){}

  template<typename VALUE>// value
  WriteConstraint(const VALUE* ptr):
      valid(ptr!=nullptr),
      writeAsRead(false),
      useEnumeratedValues(false),
      range()
  {
    if(valid){
      const auto& v = **ptr;
      if(v.writeAsRead) writeAsRead= *v.writeAsRead;
      if(v.useEnumeratedValues) useEnumeratedValues= *v.useEnumeratedValues;
      if(v.range){
        const auto& r= *v.range;
        uint64_t max=range.maximum,min=range.minimum;
        if(r.maximum) max = *r.maximum;
        if(r.minimum) min = *r.minimum;
        range.init(max,min);
      }
    }
  }
};

}}} // end namespace svd
#endif // NODAMUSHI_SVD_NORMALIZED_WRITECONSTRAINT_HPP
