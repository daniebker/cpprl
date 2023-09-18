#include "movement_command.hpp"

namespace cpprl {
MovementCommand::MovementCommand(Vector2D move_vector) { move_vector_ = move_vector; }
MovementCommand::~MovementCommand() {}

void MovementCommand::execute(cpprl::GameEntity& entity) { entity.move(move_vector_); }
}  // namespace cpprl
