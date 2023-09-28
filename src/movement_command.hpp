#ifndef MOVEMENTCOMMAND_HPP
#define MOVEMENTCOMMAND_HPP

#include "command.hpp"

namespace cpprl {
class MovementCommand : public Command {
 private:
  Vector2D move_vector_;

 public:
  MovementCommand(Engine& engine, GameEntity& entity, Vector2D move_vector)
      : Command(engine, entity), move_vector_(move_vector){};
  virtual void execute();
};
}  // namespace cpprl

#endif
