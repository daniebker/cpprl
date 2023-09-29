#include "events/die_event.hpp"

#include <iostream>

#include "util.hpp"

namespace cpprl {
void DieEvent::execute() {
  if (entity_.get_name() == "player") {
    std::cout << "You have died!\n";
    engine_.handle_player_death();
    return;
  }
  std::cout << util::capitalize(entity_.get_name()) << " has died!\n";
  entity_.die();
}
}  // namespace cpprl
