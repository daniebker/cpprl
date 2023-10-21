#include "state.hpp"

#include "engine.hpp"
#include "exceptions.hpp"

namespace cpprl {
void InGameState::on_enter() {
  input_handler_ =
      std::make_unique<GameInputHandler>(engine_, *engine_.get_player());
}

CommandResult InGameState::on_update(SDL_Event& event) {
  try {
    EngineEvent& command = input_handler_->handle_sdl_event(event);
    return command.execute();
  } catch (const Impossible& impossible) {
    engine_.get_message_log().add_message(impossible.what(), RED);
  }
}
}  // namespace cpprl
