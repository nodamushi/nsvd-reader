/*!
  @brief dimension information
  @file nodamushi/svd/normalized/dim_info.hpp
*/
#ifndef __NODAMUSHI_SVD_NORMALIZED_DIM_INFO_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_DIM_INFO_HPP__

# include <cstdint>


namespace nodamushi{
namespace svd{
namespace normalized{

//! @brief Array or Sequence or not
enum class dim_type
{
  //! @brief not array and seq.
  NON=0, 
  //! @brief NAME%s
  SEQ,       
  //! @brief NAME[%s]
  ARRAY   
};

//! @brief dim,dimIndex,array information object.
struct dim_info
{
  using id_t = unsigned int;
  //! @brief array or seq or non
  dim_type type; 
  //! @brief index
  size_t  index; 
  //! @brief dimension length
  size_t  length;
  //! @brief dimension group id.this value is unique in the parent.
  id_t    id;

  //! @brief is the dimension type array or seq
  constexpr operator bool()const noexcept{return type != dim_type::NON;}

  //! @brief is the dimension type array
  constexpr bool is_array()const noexcept{return type == dim_type::ARRAY;}
  //! @brief is the dimension type sequence
  constexpr bool is_seq()const noexcept{return type == dim_type::SEQ;}
  //! @brief is the index is 0
  constexpr bool is_first()const noexcept{return index == 0;}
  //! @brief is the index is the last index
  constexpr bool is_last()const noexcept{return index == length-1;}
};


}}}// end namespace nodamushi::svd::normalized


# include <ostream>
namespace std{
inline ostream& operator<<(ostream& o,::nodamushi::svd::normalized::dim_type d)
{
  using dt = ::nodamushi::svd::normalized::dim_type;
  switch(d){
    case dt::NON:o << "non" ;break;
    case dt::SEQ:o << "seq" ;break;
    case dt::ARRAY:o << "array";break;
  }
  return o;
}
inline ostream& operator<<(ostream& o,const ::nodamushi::svd::normalized::dim_info& d)
{
  using dt = ::nodamushi::svd::normalized::dim_type;
  
  if(d.type == dt::NON){
    o << "[dim:non:id="<<d.id<<"]";
  }else{
    o << "[dim:" << d.type << ":i="<<d.index<<":length="<<d.length<<":id="<<d.id<<"]";
  }
  return o;
}

}

#endif // __NODAMUSHI_SVD_NORMALIZED_DIM_INFO_HPP__
