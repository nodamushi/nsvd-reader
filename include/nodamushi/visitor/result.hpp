/*!
  @brief Header file
  @file result.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef __RESULT_HPP__
#define __RESULT_HPP__

namespace nodamushi{
namespace visitor{

enum class result
{
  //! continue visitor processing
  CONTINUE,
  //! stop visitor processing
  BREAK,
  //! finish the current processing hierarchy
  SKIP
};

} // end namespace visitor
}// end namespace nodamushi


#include <ostream>
namespace std{
std::ostream& operator<<(std::ostream& out,const nodamushi::visitor::result& r)
{
  using namespace nodamushi::visitor;
  switch(r){
    case result::CONTINUE: out << "continue";break;
    case result::BREAK:    out << "break";break;
    case result::SKIP:     out << "skip";break;
  }
  return out;
}
} // end namespace std
#endif // __RESULT_HPP__
