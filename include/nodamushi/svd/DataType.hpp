/*!
 * @brief  Define &lt;dataType&gt; enum
 * @file nodamushi/svd/DataType.hpp
 */
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_DATATYPE_HPP
#define NODAMUSHI_SVD_DATATYPE_HPP

# include <cstdint>
# include <algorithm>
# include <string>
# if __cplusplus >= 201703
#  include <string_view>
# endif

# include "nodamushi/svd/enum_helper.hpp"

namespace nodamushi{
namespace svd{

/**
 * @brief register data type
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_registers.html#elem_register
 */
enum class DataType
{
  //! @brief undefined
  UNDEFINED=0,
  //! @brief uint8_t
  UINT8,
  //! @brief uint16_t
  UINT16,
  //! @brief uint32_t
  UINT32,
  //! @brief uint64_t
  UINT64,
  //! @brief int8_t
  INT8,
  //! @brief int16_t
  INT16,
  //! @brief int32_t
  INT32,
  //! @brief int64_t
  INT64,
  //! @brief uint8_t*
  UINT8_PTR,
  //! @brief uint16_t*
  UINT16_PTR,
  //! @brief uint32_t*
  UINT32_PTR,
  //! @brief uint64_t*
  UINT64_PTR,
  //! @brief int8_t*
  INT8_PTR,
  //! @brief int16_t*
  INT16_PTR,
  //! @brief int32_t*
  INT32_PTR,
  //! @brief int64_t*
  INT64_PTR,
};

//---------------------------------------------------------------------
/**
 * get C type of DataType
 @code
 using type = typedef data_type_type<DataType::UINT32>::type;
 @endcode
 */
template<DataType t>struct data_type_type;
template<>struct data_type_type<DataType::UINT8>{ using type = uint8_t; };
template<>struct data_type_type<DataType::UINT16>{ using type = uint16_t; };
template<>struct data_type_type<DataType::UINT32>{ using type = uint32_t; };
template<>struct data_type_type<DataType::UINT64>{ using type = uint64_t; };
template<>struct data_type_type<DataType::INT8>{ using type = int8_t; };
template<>struct data_type_type<DataType::INT16>{ using type = int16_t; };
template<>struct data_type_type<DataType::INT32>{ using type = int32_t; };
template<>struct data_type_type<DataType::INT64>{ using type = int64_t; };
template<>struct data_type_type<DataType::UINT8_PTR>{ using type = uint8_t*; };
template<>struct data_type_type<DataType::UINT16_PTR>{ using type = uint16_t*; };
template<>struct data_type_type<DataType::UINT32_PTR>{ using type = uint32_t*; };
template<>struct data_type_type<DataType::UINT64_PTR>{ using type = uint64_t*; };
template<>struct data_type_type<DataType::INT8_PTR>{ using type = int8_t*; };
template<>struct data_type_type<DataType::INT16_PTR>{ using type = int16_t*; };
template<>struct data_type_type<DataType::INT32_PTR>{ using type = int32_t*; };
template<>struct data_type_type<DataType::INT64_PTR>{ using type = int64_t*; };

template<DataType t>
using datatype_t = typename data_type_type<t>::type;

//--------------------------------------------------------------------
template<>struct enum_helper<DataType>
{
  static constexpr bool HAS_HELPER=true;
# if __cplusplus >= 201703
  static std::optional<DataType> get(std::string_view name)
  {
    std::string s(name);
    DataType t=DataType::UNDEFINED;
    if(_get(s,t))return t;
    return std::nullopt;
  }
  static bool get(std::string_view name,DataType& dst)
  {
    if(auto v = get(name)){ dst = *v;return true;}
    return false;
  }
# endif

# if __cplusplus >= 201703
  static constexpr std::string_view nameof(DataType value)
# else
  static constexpr const char* nameof(DataType value)
# endif
  {
    switch(value){
      case DataType::UINT8:  return "uint8_t";
      case DataType::UINT16: return "uint16_t";
      case DataType::UINT32: return "uint32_t";
      case DataType::UINT64: return "uint64_t";
      case DataType::INT8:   return "int8_t";
      case DataType::INT16:  return "int16_t";
      case DataType::INT32:  return "int32_t";
      case DataType::INT64:  return "int64_t";
      case DataType::UINT8_PTR:  return "uint8_t*";
      case DataType::UINT16_PTR: return "uint16_t*";
      case DataType::UINT32_PTR: return "uint32_t*";
      case DataType::UINT64_PTR: return "uint64_t*";
      case DataType::INT8_PTR:   return "int8_t*";
      case DataType::INT16_PTR:  return "int16_t*";
      case DataType::INT32_PTR:  return "int32_t*";
      case DataType::INT64_PTR:  return "int64_t*";
      default: break;
    }
    return "";
  }
  static bool get(const char* name,DataType &dst)
  {
    std::string s(name);
    return _get(s,dst);
  }
  static bool get(const std::string& name,DataType &dst){
    std::string s(name);
    return _get(s,dst);
  }
  static bool _get(std::string& name,DataType &dst)
  {
    // remove ' '
    name.erase(std::remove_if(name.begin(),name.end(),isspace), name.end());
    if(name.size() < 6) return false;
    DataType d;
    bool b;

    if(name[0] == 'u'){
      if(name[name.size()-1] == 't'){
        switch(name[4])
        {
          case '1':d=DataType::UINT16;b = name =="uint16_t";break;
          case '3':d=DataType::UINT32;b = name =="uint32_t";break;
          case '6':d=DataType::UINT64;b = name =="uint64_t";break;
          case '8':d=DataType::UINT8;b = name =="uint8_t";break;
          default :b = false;
        }
      }else{
        switch(name[4])
        {
          case '1':d=DataType::UINT16_PTR;b = name =="uint16_t*";break;
          case '3':d=DataType::UINT32_PTR;b = name =="uint32_t*";break;
          case '6':d=DataType::UINT64_PTR;b = name =="uint64_t*";break;
          case '8':d=DataType::UINT8_PTR;b = name =="uint8_t*";break;
          default :b = false;
        }
      }
    }else{
      if(name[name.size()-1] == 't'){
        switch(name[3])
        {
          case '1':d=DataType::INT16;b = name =="int16_t";break;
          case '3':d=DataType::INT32;b = name =="int32_t";break;
          case '6':d=DataType::INT64;b = name =="int64_t";break;
          case '8':d=DataType::INT8;b = name =="int8_t";break;
          default :b = false;
        }
      }else{
        switch(name[3])
        {
          case '1':d=DataType::INT16_PTR;b = name =="int16_t*";break;
          case '3':d=DataType::INT32_PTR;b = name =="int32_t*";break;
          case '6':d=DataType::INT64_PTR;b = name =="int64_t*";break;
          case '8':d=DataType::INT8_PTR;b = name =="int8_t*";break;
          default :b = false;
        }
      }
    }
    if(!b)return false;
    dst = d;
    return true;
  }


};

}
}// end namespace nodamushi
namespace std{
inline ostream& operator <<(std::ostream& os,const ::nodamushi::svd::DataType& value)
{
  os << ::nodamushi::svd::enum_helper<::nodamushi::svd::DataType>::nameof(value);
  return os;
}
}
#endif //NODAMUSHI_SVD_DATATYPE_HPP
