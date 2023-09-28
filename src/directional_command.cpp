#include "../include/directional_command.hpp"

#include "../include/melee_command.hpp"
#include "../include/movement_command.hpp"

namespace cpprl {

void DirectionalCommand::execute() {
  auto targetPos = entity_.get_position() + move_vector_;

  if (engine_.get_entities().get_blocking_entity_at(targetPos)) {
    auto action = MeleeCommand(engine_, entity_, move_vector_);
    action.execute();
  } else {
    auto action = MovementCommand(engine_, entity_, move_vector_);
    action.execute();
  }
}
}  // namespace cpprl
