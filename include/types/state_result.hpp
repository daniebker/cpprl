#ifndef STATE_RESULT_HPP
#define STATE_RESULT_HPP

#include <memory>
#include <string>
#include <variant>

#include "types/state_fwd.hpp"

namespace cpprl {
/**
 * @brief Communicates a change state to the engine.
 */
struct Change {
  /**
   * The new state to change to.
   */
  std::unique_ptr<State> next_state;
};
/**
 * @brief Communicates a reset to the engine.
 */
struct Reset {};

/**
 * @brief Communicates an end turn to the engine.
 */
struct EndTurn {};

/**
 * @brief Communicates a no-op to the engine.
 * Used when the result of an action should not change the state.
 */
struct NoOp {
  /**
   * @brief An optional message that can be used in the UI.
   * or debugging.
   */
  std::string message;
};

/**
 * @brief Used to communicate a quit to the engine.
 */
struct Quit {};

/**
 * @brief Used to communicate a game over to the engine.
 */
struct GameOver {};

/**
 * @brief Used to communicate a load game to the engine.
 */
struct LoadGame {};

/**
 * @brief Alias a state result to a variant of all possible results.
 */
using StateResult =
    std::variant<std::monostate, Change, Reset, EndTurn, Quit, LoadGame, NoOp>;
}  // namespace cpprl
#endif
