/*!
  @brief normalized CPU and SAU
  @file nodamushi/svd/normalized/Cpu.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __NODAMUSHI_SVD_NORMALIZED_CPU_HPP__
#define __NODAMUSHI_SVD_NORMALIZED_CPU_HPP__

# include <cstdint>
# include "nodamushi/svd/Endian.hpp"
# include "nodamushi/svd/SAURegionConfigProtect.hpp"
# include "nodamushi/svd/ARMCPU.hpp"
# include "nodamushi/svd/normalized/node_container.hpp"

namespace nodamushi{
namespace svd{
namespace normalized{


/**
 * @brief Security Attribution Unit(SAU) region.
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_cpu.html#elem_region
 * @see nodamushi::svd::SAURegionsConfigRegion
 */
template<typename STRREF>struct SAURegionsConfigRegion
{
  //! @brief attribute enabled
  bool enabled;
  //! @brief attribute name
  STRREF name;
  //! @brief &lt;base&gt; element
  uint64_t base;
  //! @brief &lt;limit&gt; element
  uint64_t limit;
  //! @brief &lt;access&gt;
  SAURegionConfigProtect access;

  /**
   * @param T nodamushi::svd::SAURegionsConfigRegion
   */
  template<typename T>  SAURegionsConfigRegion(const T& s)
      :enabled(s.enabled.get(true)),
       name(s.name.get("")),
       base(s.base.get(0)),
       limit(s.limit.get(0)),
       access(s.access.get(SAURegionConfigProtect{}))
  {}
};

/**
 * @brief Security Attribution Unit(SAU).
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_cpu.html#elem_sauRegionsConfig
 * @see nodamushi::svd::SAURegionConfig
 */
template<typename STRREF>struct SAURegionsConfig
{
  //! @brief attribute enabled
  bool enabled;
  //! @brief attribute protectionWhenDisabled
  SAURegionConfigProtect protectionWhenDisabled;
  //! @brief &lt;region&gt; elements.
  std::vector<SAURegionsConfigRegion<STRREF>> region;
  
  //! @brief default constructor
  SAURegionsConfig():
      enabled(false),
      protectionWhenDisabled(SAURegionConfigProtect{}),
      region{}
  {}
  
 private:
  template<typename X>friend class Cpu;
  template<typename V> void init(const V& v){
    if(v){
      enabled = v->enabled.get(true);
      protectionWhenDisabled=
          v->protectionWhenDisabled.get(SAURegionConfigProtect{});
      region.reserve(v->region.size());
      for(const auto& ss:v->region)
        region.emplace_back(ss);
    }
  }

};

/*!
 * @brief &lt;cpu&gt; element
 * @see http://www.keil.com/pack/doc/CMSIS/SVD/html/elem_cpu.html
 * @see nodamushi::svd::Cpu
 */
template<typename STRREF>struct Cpu
{
  /**
   *  @brief &lt;name&gt; element
   *  @see nodamushi::svd::ARMCPU
   *  @see get_armcpu()
   */
  STRREF name; 
  //! @brief &lt;revision&gt; element
  STRREF revision;
  //! @brief &lt;endian&gt; element
  Endian endian;
  //! @brief &lt;mpuPresent&gt; element
  bool mpuPresent;
  //! @brief &lt;fpuPresent&gt; element
  bool fpuPresent;
  //! @brief &lt;fpuDP&gt; element
  bool fpuDP;
  //! @brief &lt;dspPresent&gt; element
  bool dspPresent;
  //! @brief &lt;icachePresent&gt; element
  bool icachePresent;
  //! @brief &lt;dcachePresent&gt; element
  bool dcachePresent;
  //! @brief &lt;itcmPresent&gt; element
  bool itcmPresent;
  //! @brief &lt;dtcmPresent&gt; element
  bool dtcmPresent;
  //! @brief &lt;vtorPresent&gt; element
  bool vtorPresent;
  //! @brief &lt;nvicPrioBits&gt; element
  uint32_t nvicPrioBits;
  //! @brief &lt;vendorSysticConfig&gt; element
  bool vendorSystickConfig;
  //! @brief &lt;deviceNumInterrupts&gt; element
  uint32_t deviceNumInterrupts;
  //! @brief &lt;sauNumRegions&gt; element
  uint32_t sauNumRegions;
  //! @brief &lt;sauRegionsConfig&gt; element
  SAURegionsConfig<STRREF> sauRegionsConfig;

  //! @brief convert name to ARMCPU enum
  ARMCPU get_armcpu()const noexcept{return ::nodamushi::svd::get_cpu_type(name);}
  
  /**
   * @brief This constructor is for normalizer.
   * @param V nodamushi::svd::value of &lt;cpu&gt;
   */
  template<typename V>//V = value
  Cpu(const V& v):
      name(""),
      revision(""),
      endian(static_cast<Endian>(0)),
      mpuPresent(false),
      fpuPresent(false),
      fpuDP(false),
      dspPresent(false),
      icachePresent(false),
      dcachePresent(false),
      itcmPresent(false),
      dtcmPresent(false),
      nvicPrioBits(0),
      vendorSystickConfig(false),
      deviceNumInterrupts(0),
      sauNumRegions(0),
      sauRegionsConfig()
  {
    if(v){
      name = v->name.get("");
      revision = v->revision.get("");
      endian = v->endian.get(static_cast<Endian>(0));
      mpuPresent = v->mpuPresent.get(false);
      fpuPresent = v->fpuPresent.get(false);
      fpuDP = v->fpuDP.get(false);
      dspPresent = v->dspPresent.get(false);
      icachePresent = v->icachePresent.get(false);
      dcachePresent = v->dcachePresent.get(false);
      itcmPresent = v->itcmPresent.get(false);
      dtcmPresent = v->dtcmPresent.get(false);
      nvicPrioBits = v->nvicPrioBits.get(0);
      vendorSystickConfig = v->vendorSystickConfig.get(false);
      deviceNumInterrupts = v->deviceNumInterrupts.get(0);
      sauNumRegions = v->sauNumRegions.get(0);
      sauRegionsConfig.init(v->sauRegionsConfig);
    }
  }
};



}}} // end namespace svd

#endif // __NODAMUSHI_SVD_NORMALIZED_CPU_HPP__
