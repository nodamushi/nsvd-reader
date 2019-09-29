/*!
  @brief ARM CPU enum
  @file nodamushi/svd/ARMCPU.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef NODAMUSHI_SVD_ARMCPU_HPP
#define NODAMUSHI_SVD_ARMCPU_HPP

namespace nodamushi{
namespace svd{

/**
 * @brief Arm Core names.
 */
enum class ARMCPU
{
  //! @brief Other CPU
  OTHER=0, 
  //! @brief Arm Cortex-M0
  CM0,
  //! @brief Arm Cortex-M0+
  CM0PLUS,
  //! @brief Arm Cortex-M1
  CM1,
  //! @brief Arm Secure Core SC000
  SC000,
  //! @brief Arm Cortex-M23
  CM23,
  //! @brief Arm Cortex-M3
  CM3,
  //! @brief Arm Cortex-M33
  CM33,
  //! @brief Arm Cortex-M35P
  CM35P,
  //! @brief Arm Secure Core SC300
  SC300,
  //! @brief Arm Cortex-M4
  CM4,
  //! @brief Arm Cortex-M7
  CM7,
  //! @brief Arm Cortex-A5
  CA5,
  //! @brief Arm Cortex-A7
  CA7,
  //! @brief Arm Cortex-A8
  CA8,
  //! @brief Arm Cortex-A9
  CA9,
  //! @brief Arm Cortex-A15
  CA15,
  //! @brief Arm Cortex-A17
  CA17,
  //! @brief Arm Cortex-A53
  CA53,
  //! @brief Arm Cortex-A57
  CA57,
  //! @brief Arm Cortex-A72
  CA72
};

/**
 * @brief get ARMCPU enum from text.
 * @param name CPU name.
 * @return return ARMCPU. 
 */
template<typename STR>ARMCPU get_cpu_type(const STR& name)
{
  if(name.size() < 3)return ARMCPU::OTHER;
  if(name[0] == 'S'){
    if(name == "SC000") return ARMCPU::SC000;
    if(name == "SC300") return ARMCPU::SC300;
  }else if(name[0] == 'C'){
    if(name[1] == 'M'){
      if(name[2] == '0'){
        if(name.size() == 3) return ARMCPU::CM0;
        if(name == "CM0+") return ARMCPU::CM0PLUS;
        if(name == "CM0PLUS") return ARMCPU::CM0PLUS;
      }else if(name[2] == '1'){
        if(name.size() == 3) return ARMCPU::CM1;
      }else if(name[2] == '2'){
        if(name == "CM23") return ARMCPU::CM23;
      }else if(name[2] == '3'){
        if(name.size() == 3) return ARMCPU::CM3;
        if(name == "CM33") return ARMCPU::CM33;
        if(name == "CM35P") return ARMCPU::CM35P;
      }if(name[2] == '4'){
        if(name.size() == 3) return ARMCPU::CM4;
      }if(name[2] == '7'){
        if(name.size() == 3) return ARMCPU::CM7;
      }
    }else if(name[1] == 'A'){
      if(name[2] == '1'){
        if(name == "CA15") return ARMCPU::CA15;
        if(name == "CA17") return ARMCPU::CA17;
      }else if(name[2] == '5'){
        if(name.size() == 3) return ARMCPU::CA5;
        if(name == "CA53") return ARMCPU::CA53;
        if(name == "CA57") return ARMCPU::CA57;
      }else if(name[2] == '7'){
        if(name.size() == 3) return ARMCPU::CA7;
        if(name == "CA72") return ARMCPU::CA72;
      }else if(name[2] == '8'){
        if(name.size() == 3) return ARMCPU::CA8;
      }else if(name[2] == '9'){
        if(name.size() == 3) return ARMCPU::CA9;
      }
    }
  }
  return ARMCPU::OTHER;
}



}}// end namespace nodamushi

#endif // NODAMUSHI_SVD_ARMCPU_HPP
