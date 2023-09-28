#include "../../include/events/melee_command.hpp"

#include <SDL2/SDL.h>

#include <iostream>
namespace cpprl {

void MeleeCommand::execute() {
  auto targetPos = entity_.get_position() + move_vector_;
  auto* target = engine_.get_entities().get_blocking_entity_at(targetPos);

  if (target) {
    SDL_Log("The %s laughs at your puny efforts to attack him!", target->get_name().data());
  }
};
}  // namespace cpprl
