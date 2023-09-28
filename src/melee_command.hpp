#pragma once

#include "movement_command.hpp"

namespace cpprl {

class MeleeCommand : MovementCommand {
 public:
  MeleeCommand(Engine& engine, GameEntity& entity, Vector2D target_vector)
      : MovementCommand(engine, entity, target_vector){};
  void execute();
};
}  // namespace cpprl
