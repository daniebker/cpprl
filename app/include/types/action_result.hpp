#pragma once

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
}
