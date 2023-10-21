#pragma once

#include "directional_command.hpp"

namespace cpprl {

class MeleeCommand : DirectionalCommand {
 public:
  MeleeCommand(Engine& engine, Entity* entity, Vector2D target_vector)
      : DirectionalCommand(engine, entity, target_vector){};
  CommandResult execute();
};
}  // namespace cpprl
