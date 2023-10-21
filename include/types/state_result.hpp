#ifndef STATE_RESULT_HPP
#define STATE_RESULT_HPP

#include "state_fwd.hpp"

namespace cpprl {
struct Change {
  std::unique_ptr<State> next_state;
};
struct Reset {};
struct EndTurn {};
struct Quit {};
using CommandResult =
    std::variant<std::monostate, Change, Reset, EndTurn, Quit>;
}  // namespace cpprl
#endif
