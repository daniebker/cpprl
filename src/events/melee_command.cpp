#include "events/melee_command.hpp"

#include <SDL2/SDL.h>
#include <fmt/format.h>

#include <iostream>

#include "colours.hpp"
#include "combat_system.hpp"
#include "events/die_event.hpp"
#include "message_log.hpp"
#include "util.hpp"
namespace cpprl {

void MeleeCommand::execute() {
  auto targetPos =
      entity_->get_transform_component()->get_position() + move_vector_;
  Entity* target = engine_.get_entities().get_blocking_entity_at(targetPos);

  tcod::ColorRGB attack_colour = WHITE;
  if (entity_->get_name() != "player") {
    attack_colour = RED;
  }

  if (target) {
    int damage = combat_system::handle_attack(*entity_, *target);
    if (damage > 0) {
      std::string message = fmt::format(
          "{} attacks {} for {} hit points.",
          util::capitalize(entity_->get_name()),
          util::capitalize(target->get_name()),
          damage);

      engine_.get_message_log().add_message(message, attack_colour, true);

      if (target->get_defense_component()->is_dead()) {
        auto action = DieEvent(engine_, *target);
        action.execute();
      }
    } else {
      std::string message = fmt::format(
          "{} attacks {} but does no damage.",
          util::capitalize(entity_->get_name()),
          util::capitalize(target->get_name()));

      engine_.get_message_log().add_message(message, attack_colour, true);
    }
  }
};
}  // namespace cpprl
