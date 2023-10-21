#pragma once

#include "command.hpp"

namespace cpprl {

class DirectionalCommand : public Command {
 protected:
  Vector2D move_vector_;

 public:
  DirectionalCommand(Engine& engine, Entity* entity, Vector2D move_vector)
      : Command(engine, entity), move_vector_(move_vector){};
  virtual CommandResult execute();
};
}  // namespace cpprl
