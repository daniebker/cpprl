#include "events/die_event.hpp"

#include <iostream>

#include "util.hpp"

namespace cpprl {
void DieEvent::execute() {
  if (entity_.get_name() == "player") {
    engine_.handle_player_death();
  }
  std::cout << util::capitalize(entity_.get_name()) << " has died!\n";
  entity_.die();
}
}  // namespace cpprl
