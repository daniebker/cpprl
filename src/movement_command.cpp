#include "movement_command.hpp"

#include "types/map.hpp"

namespace cpprl {

void MovementCommand::execute() {
  Vector2D new_position = entity_.get_position() + move_vector_;
  auto map = engine_.get_map();

  if (map->is_not_in_bounds(new_position)) {
    return;
  }

  if (map->is_walkable(new_position)) {
    entity_.move(new_position);
  }
}
}  // namespace cpprl
