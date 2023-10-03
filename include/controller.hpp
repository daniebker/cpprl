#ifndef INCLUDE_CONTROLLER_HPP_
#define INCLUDE_CONTROLLER_HPP_

#include <optional>

#include "types/math.hpp"

namespace cpprl {
struct Controller {
  std::optional<Vector2D> cursor = {};
};

}  // namespace cpprl

#endif  // INCLUDE_CONTROLLER_HPP_
