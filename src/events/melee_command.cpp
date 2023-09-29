#include "events/melee_command.hpp"

#include <SDL2/SDL.h>

#include <iostream>

#include "combat_system.hpp"
#include "events/die_event.hpp"
#include "util.hpp"
namespace cpprl {

void MeleeCommand::execute() {
  auto targetPos = entity_.get_position() + move_vector_;
  auto* target = engine_.get_entities().get_blocking_entity_at(targetPos);

  if (target) {
    int damage = combat_system::handle_attack(entity_, *target);
    if (damage > 0) {
      std::cout << util::capitalize(entity_.get_name()) << " attacks " << target->get_name() << " for " << damage
                << " hit points.\n";

      if (target->is_dead()) {
        auto action = DieEvent(engine_, *target);
        action.execute();
      }
    } else {
      std::cout << util::capitalize(entity_.get_name()) << " attacks " << target->get_name()
                << " but does no damage.\n";
    }
  }
};
}  // namespace cpprl
