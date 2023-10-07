#include "events/die_event.hpp"

#include <iostream>

#include "util.hpp"

namespace cpprl {
void DieEvent::execute() {
  if (entity_.get_name() == "player") {
    engine_.handle_player_death();
  }
  engine_.get_message_log().add_message(
      fmt::format("{} has died!", util::capitalize(entity_.get_name())));
  entity_.get_defense_component()->die(entity_);
}
}  // namespace cpprl
