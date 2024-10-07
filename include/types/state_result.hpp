#ifndef STATE_RESULT_HPP
#define STATE_RESULT_HPP

#include <memory>
#include <string>
#include <variant>

#include "types/state_fwd.hpp"

namespace cpprl {
  struct Change {
    std::unique_ptr<State> next_state;
  };
  struct Reset {};
  struct EndTurn {};
  struct NoOp {
    std::string message;
  };
  struct Quit {};
  struct GameOver {};
  struct LoadGame {};
  using StateResult =
    std::variant<std::monostate, Change, Reset, EndTurn, Quit, LoadGame, NoOp>;
}
#endif
