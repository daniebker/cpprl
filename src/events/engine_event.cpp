#include "events/engine_event.hpp"

namespace cpprl {

void MouseInputEvent::execute() {
  engine_.get_controller().cursor = position_;
  engine_.get_map()->set_highlight_tile(position_);
}
void MouseClickEvent::execute() {
  engine_.get_controller().cursor = position_;
  // engine_.get_map()->set_highlight_tile(position_);
  bool success =
      engine_.get_map()->set_target_tile(position_, *engine_.get_player());
  if (success) {
    engine_.toggle_targeting_mode();
  }
}

void ExitTargetingModeCommand::execute() { engine_.toggle_targeting_mode(); }
}  // namespace cpprl
