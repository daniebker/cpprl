#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>

#include "types/math.hpp"

namespace cpprl::util {
inline auto get_data_dir() -> std::filesystem::path {
  static auto root_directory =
      std::filesystem::path{"."};  // Begin at the working directory.
  while (!std::filesystem::exists(root_directory / "data")) {
    // If the current working directory is missing the data dir then it will
    // assume it exists in any parent directory.
    root_directory /= "..";
    if (!std::filesystem::exists(root_directory)) {
      throw std::runtime_error("Could not find the data directory.");
    }
  }
  return root_directory / "data";
};
inline std::string capitalize(const std::string& string) {
  auto ret = string;
  auto ch = ret[0];
  ret[0] = std::toupper(ch);
  return ret;
}
}  // namespace cpprl::util
#endif  // UTIL_HPP
