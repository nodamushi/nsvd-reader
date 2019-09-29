/*!
  @brief normalized
  @file nodamushi/svd/normalized.hpp

  @code
  namespace svd  = nodamushi::svd;
  namespace norm = nodamushi::svd::normalized;
  svd::Device<> d(svd_reader);
  // C++17
  std::share_ptr<norm::Device<std::string_view>> nptr = svd::normalize(d);

  // C++14
  std::share_ptr<norm::Device<std::string>> nptr = svd::normalize(d);

  // Feature:
  // when you want to other string format like boost::string_view,
  //
  // auto nptr = svd::nomalizer<boost::string_view>::normalize(d);
  
  @endcode
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_SVD_NORMALIZED_HPP
#define NODAMUSHI_SVD_NORMALIZED_HPP

# include "nodamushi/svd/normalized/normalizer.hpp"

#endif // NODAMUSHI_SVD_NORMALIZED_HPP
