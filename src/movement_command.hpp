#ifndef MOVEMENTCOMMAND_HPP
#define MOVEMENTCOMMAND_HPP

#include "command.hpp"

namespace cpprl {
class MovementCommand : public Command {
 private:
  Vector2D move_vector_;

 public:
  MovementCommand(Vector2D move_vector);
  virtual ~MovementCommand();
  virtual void execute(GameEntity& entity);
};
}  // namespace cpprl

#endif
