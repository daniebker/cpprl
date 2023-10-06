#include "events/view_history_command.hpp"

#include "input_handler.hpp"

namespace cpprl {
void ViewHistoryCommand::execute() {
  engine_.toggle_pause();
  engine_.toggle_history_view();
  engine_.set_input_handler(std::make_unique<HistoryViewInputHandler>(engine_));
}
}  // namespace cpprl
