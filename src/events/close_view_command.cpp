#include "events/close_view_command.hpp"

#include "input_handler.hpp"

namespace cpprl {

void CloseViewCommand::execute() {
  engine_.toggle_pause();
  engine_.toggle_history_view();
  engine_.set_input_handler(
      std::make_unique<GameInputHandler>(engine_, engine_.get_player()));
}
}  // namespace cpprl
