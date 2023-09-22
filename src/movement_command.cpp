#include "movement_command.hpp"

#include "types/map.hpp"

namespace cpprl {
MovementCommand::MovementCommand(Vector2D move_vector) { move_vector_ = move_vector; }
MovementCommand::~MovementCommand() {}

void MovementCommand::execute(Map& map, GameEntity& entity) {
  Vector2D new_position = entity.get_position() + move_vector_;
  if (map.get_tiles().at(new_position.x, new_position.y) == Tiles::floor) {
    entity.move(new_position);
  }
}
}  // namespace cpprl
