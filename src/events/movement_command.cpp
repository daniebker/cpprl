#include "../../include/events/movement_command.hpp"

#include "../../include/events/melee_command.hpp"
#include "../../include/types/map.hpp"
#include "exceptions.hpp"

namespace cpprl {

CommandResult MovementCommand::execute() {
  Vector2D new_position =
      entity_->get_transform_component()->get_position() + move_vector_;
  auto map = engine_.get_map();

  if (map->is_not_in_bounds(new_position)) {
    throw Impossible("You can't move out the bounds of space and time.");
  }

  if (engine_.get_entities().get_blocking_entity_at(new_position)) {
    return EndTurn{};
  }

  if (map->is_walkable(new_position)) {
    entity_->get_transform_component()->move(new_position);
  } else {
    throw Impossible("You can't walk on that.");
  }
  return EndTurn{};
}
}  // namespace cpprl
