/*!
  @brief elements header file
  @file nodamushi/svd/elements.hpp
*/
#ifndef __NODAMUSHI_SVD_ELEMENTS_HPP__
#define __NODAMUSHI_SVD_ELEMENTS_HPP__

#include <ostream>
#include <vector>
#include <memory>
#include <type_traits>

#include "nodamushi/box.hpp"
#include "nodamushi/boxvec.hpp"
#include "nodamushi/const_string.hpp"
#include "nodamushi/svd/bitRange.hpp"
#include "nodamushi/svd/svdvisitor.hpp"
#include "nodamushi/svd/value.hpp"
#include "nodamushi/svd/svd_printer.hpp"
#include "nodamushi/svd/svd_reader.hpp"
#include "nodamushi/svd/create.hpp"
#include "nodamushi/svd/node.hpp"
#include "nodamushi/svd/path.hpp"
#include "nodamushi/svd/dim_type.hpp"
#include "nodamushi/svd/reg_or_cluster.hpp"
// enum
#include "nodamushi/svd/Access.hpp"
#include "nodamushi/svd/ReadAction.hpp"
#include "nodamushi/svd/Protection.hpp"
#include "nodamushi/svd/ModifiedWriteValues.hpp"
#include "nodamushi/svd/EnumUsage.hpp"
#include "nodamushi/svd/Endian.hpp"
#include "nodamushi/svd/AddressBlockUsage.hpp"
#include "nodamushi/svd/svdvisitor.hpp"
#include "nodamushi/svd/SAURegionConfigProtect.hpp"
#include "nodamushi/svd/DataType.hpp"

// elements
#include "nodamushi/svd/EnumeratedValue.hpp"
#include "nodamushi/svd/DimArrayIndex.hpp"
#include "nodamushi/svd/WriteConstraint.hpp"
#include "nodamushi/svd/Enumeration.hpp"
#include "nodamushi/svd/Field.hpp"
#include "nodamushi/svd/Register.hpp"
#include "nodamushi/svd/Cluster.hpp"
#include "nodamushi/svd/Interrupt.hpp"
#include "nodamushi/svd/AddressBlock.hpp"
#include "nodamushi/svd/Peripheral.hpp"
#include "nodamushi/svd/Cpu.hpp"
#include "nodamushi/svd/Device.hpp"


#endif // __NODAMUSHI_SVD_ELEMENTS_HPP__
