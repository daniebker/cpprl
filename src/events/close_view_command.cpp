#include "events/close_view_command.hpp"

#include "input_handler.hpp"

namespace cpprl {

void CloseViewCommand::execute() {
  engine_.toggle_pause();
  engine_.toggle_view();
  engine_.set_input_handler(
      new GameInputHandler(engine_, engine_.get_player()));
}
}  // namespace cpprl
