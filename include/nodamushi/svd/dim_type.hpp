/*!
  @brief &lt;dim&gt;
  @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_special.html#dimElementGroup_gr
  @file dim_type.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_DIM_TYPE_HPP
#define NODAMUSHI_SVD_DIM_TYPE_HPP

# include <ostream>
# include <cstdint>
# include <string>
# include <vector>
# include "nodamushi/const_string.hpp"
# include "nodamushi/svd/value_setter_helper.hpp"
# include "nodamushi/to_int.hpp"


namespace nodamushi{
namespace svd{

/**
 * &lt;dim&gt; value type
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_special.html#dimElementGroup_gr
 */
using dim_type = uint64_t;
/**
 * &lt;dimIncrement&gt; value type
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_special.html#dimElementGroup_gr
 */
using dimInc_type = uint64_t;

/**
 * &lt;dimName&gt; value type
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_special.html#dimElementGroup_gr
 */
template<typename STR>using dimName = STR;

/**
 *  Each string decomposing dimIndex.
 *  @see dimIndex
 */
struct dimIndex_value
{
  //! value
  dimIndex_value(const std::string& s,size_t f,size_t e):
    str(s.data()+f,s.data()+e),original(),has_original(false){}
  //! for first number
  dimIndex_value(int value,
    const std::string &s, size_t f, size_t e,size_t f2,size_t e2):
    str(std::to_string(value)), original(), has_original(true) 
  {
    original.reserve(e + e2 - f - f2 + 1);
    original.append(s, f, e-f);
    original.push_back('-');
    original.append(s, f2,e2-f2);
  }
  //! for number
  dimIndex_value(int value) : 
    str(std::to_string(value)), original(), has_original(true) {}
# if __cplusplus >= 201703
  //! value
  dimIndex_value(std::string_view s, size_t f, size_t e) :
   str(s.data() + f, s.data() + e), original(), has_original(false) {}
  //! for first number
  dimIndex_value(int value,
                 std::string_view s, size_t f, size_t e, size_t f2, size_t e2) :
    str(std::to_string(value)),original(), has_original(true)
  {
    original.reserve(e + e2 - f - f2 + 1);
    original.append(s, f, e-f);
    original.push_back('-');
    original.append(s, f2,e2-f2);
  }
# endif
  
  bool operator==(string_ref s)const noexcept{return str == s;}
  bool operator!=(string_ref s)const noexcept{return str != s;}
  
  //! dimIndex text
  std::string str;
  /*!
    original text of sequence number. (xxx-yyy)
    this member is enable when has_original is true.
    However, except for the number of the first sequence, this string is empty.
   */
  std::string original;
  bool has_original; //!< flag for original.
};

/**
 * &lt;dimIndex&gt; element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_special.html#dimElementGroup_gr
 */
struct dimIndex
{
  dimIndex():dims{}{}

  
  static bool is_white_space(char c)
  {
    return c == ' ' || c == '\t' 
# ifndef DISABLE_SVD_LINEBREAK_AS_WHITESPACE
    ||c == '\r' || c == '\n' 
# endif
    ;
  }


  dimIndex& operator=(const char* str)
  {
# if __cplusplus >= 201703
    std::string_view s = str;
# else
    std::string s = str;
#endif
    return operator=(s);
  }
# if __cplusplus >= 201703
  dimIndex& operator=(const std::string &str)
  {
    std::string_view s = str;
    return operator=(s);
  }
  
  dimIndex& operator=(std::string_view text)
# else
  dimIndex& operator=(const std::string &text)
# endif    
  {
    // split dimIncrement text.
    dims.clear();
    size_t len = text.length();
    size_t i = 0;

    while(i < len)
    {
      char c;
      // skip white space
      while(i < len && is_white_space(text[i])) i++;

      if(i == len) break;

      const size_t p = i;
      size_t e = p; // to trim last white space
      //
      // number sequence
      //    n1 -  n2
      //   ddd - ddd 
      // (* d = 0,1,2,3,4,5,6,7,8,9)
      //
      // state
      // 0: error
      // 1: no-init       (start)
      // 2: n1
      // 3: white space before '-'
      // 4: white space after '-'
      // 5: n2            (last)
      // 6: trimed spaces (last)
      //  (1) -> 1  input: white-space
      //  (1) -> 2  input: d
      //   2  -> 2  input: d
      //   2  -> 3  input: white-space
      //   2  -> 4  input: -
      //   3  -> 4  input: -
      //   4  ->(5) input: d
      //   5  ->(5) input: d
      //   5  ->(6) input: white-space
      //   6  ->(6) input: white-space
      //
      unsigned int numseq_state = 1; 
      size_t n1_begin_pos=0;
      size_t n1_end_pos = 0;
      size_t n2_begin_pos=0;
      
      while(i < len && ((c=text[i]) != ',')){

        if(is_white_space(c)){
          switch(numseq_state){
          case 2:
            numseq_state=3;
            n1_end_pos = i;
            break;
          case 5: numseq_state=6;break;
          }
          i++;
          continue;
        }

        e = i;
        bool is_num = '0' <= c && c <= '9';
        if(is_num){
          switch(numseq_state){
          case 0:
            break;
          case 1: 
            numseq_state =2;
            n1_begin_pos =i;
            break;
          case 2:
            break;
          case 3:
            numseq_state = 0;
            break;
          case 4:
            numseq_state = 5;
            n2_begin_pos = i;
            break;
          case 5:
            break;
          case 6:
            numseq_state = 0;
            break;
          }
        }else if ((numseq_state == 2 || numseq_state==3)&& c == '-'){
          if(numseq_state == 2){
            n1_end_pos = i;
          }
          numseq_state = 4;
        }else{
          numseq_state = 0;
        }
        i++;
      }
      
      e++;
      if(numseq_state == 5 || numseq_state == 6){
        const int from = ::nodamushi::to_int<int>(text,n1_begin_pos,n1_end_pos);
        const int end = ::nodamushi::to_int<int>(text,n2_begin_pos,e);
        for(int k = from;k<=end;k++){
          if(k == from)
            dims.emplace_back(k, text, n1_begin_pos, n1_end_pos,n2_begin_pos,e);
          else
            dims.emplace_back(k);
        }
      }else{
        dims.emplace_back(text ,p, e);
      } 
      i++;
    }
    return * this;
  }
  const std::vector<dimIndex_value>& get()const{return dims;}
  const std::string& at(int index)const{return dims.at(index).str;}
  size_t length()const noexcept{return dims.size();}
  size_t size()const noexcept{return dims.size();}
  const std::string& operator[](size_t i)const noexcept{return dims[i].str;}
  operator bool()const noexcept{return dims.size() != 0;}
  /*!
  * Append index text to dst.
  * If index >= size, append number text.
  * @param dst target
  * @param index  index
  */
  void append(std::string& dst,size_t index)const{
    if(index < size())dst.append(dims[index].str);
    else              dst.append(std::to_string(index));
  }
  /**
   * @param s text
   * @return contains text s
   */
  bool contains(string_ref s)const noexcept{
    for(auto& d:dims)
      if(d == s)
        return true;
    return false;
  }

private:
  std::vector<dimIndex_value> dims;
};



template<>struct value_setter_helper<dimIndex>
{
  static constexpr bool value=true;
  static bool set(const std::string& src,dimIndex& dst)
  {
    dst = src;
    bool b = dst;
    return b;
  }
# if __cplusplus >= 201703
  static bool set(std::string_view src,dimIndex& dst)
  {
    dst = src;
    bool b = dst;
    return b;
  }
# endif
};


}} // end namespace svd



namespace std{
inline ostream& operator <<(ostream& o,const ::nodamushi::svd::dimIndex& d)
{
  bool first = true;
  for(const auto& v:d.get()){
    if(v.has_original && v.original.empty())
      continue;
    if(first) first = false;
    else      o << ",";
    if(v.has_original)
      o << v.original;
    else
      o << v.str;
  }
  return o;
}
} // end namespace std

#endif // NODAMUSHI_SVD_DIM_TYPE_HPP
