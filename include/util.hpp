#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>

#include "types/math.hpp"

namespace cpprl::util {
inline std::string capitalize(const std::string& string) {
  auto ret = string;
  auto ch = ret[0];
  ret[0] = std::toupper(ch);
  return ret;
}
}  // namespace cpprl::util
#endif  // UTIL_HPP
