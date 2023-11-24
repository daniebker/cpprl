#ifndef ACTION_RESULT_HPP
#define ACTION_RESULT_HPP

#include <memory>
#include <string>
#include <variant>

#include "state.hpp"

namespace cpprl {
class State;

/**
 * @brief Used when an action can resolve without error.
 */
struct Success {};

/**
 * @brief Used when an action cannot resolve without error.
 */
struct Failure {
  std::string message;
};

/**
 * @brief Used when the action needs to poll the user for input.
 */
struct Poll {
  /**
   * @brief The new state to change to.
   */
  std::unique_ptr<State> new_state;
};

/**
 * @brief Alias a state result to a variant of all possible results.
 */
using ActionResult = std::variant<Success, Failure, Poll>;
}  // namespace cpprl
#endif
