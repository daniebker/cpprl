#ifndef ACTION_RESULT_HPP
#define ACTION_RESULT_HPP

#include <memory>
#include <string>
#include <variant>

#include "state.hpp"

namespace cpprl {
class State;
struct Success {};

struct Failure {
  std::string message;
};

struct Poll {
  std::unique_ptr<State> new_state;
};

using ActionResult = std::variant<Success, Failure, Poll>;
}  // namespace cpprl
#endif
