#ifndef MOVEMENTCOMMAND_HPP
#define MOVEMENTCOMMAND_HPP

#include "directional_command.hpp"

namespace cpprl {
class MovementCommand : public DirectionalCommand {
 public:
  MovementCommand(Engine& engine, Entity* entity, Vector2D move_vector)
      : DirectionalCommand(engine, entity, move_vector){};
  virtual CommandResult execute();
};
}  // namespace cpprl

#endif
