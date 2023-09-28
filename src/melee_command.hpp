#pragma once

#include "directional_command.hpp"

namespace cpprl {

class MeleeCommand : DirectionalCommand {
 public:
  MeleeCommand(Engine& engine, GameEntity& entity, Vector2D target_vector)
      : DirectionalCommand(engine, entity, target_vector){};
  void execute();
};
}  // namespace cpprl
