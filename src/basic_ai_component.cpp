#include "basic_ai_component.hpp"

#include <algorithm>
#include <iostream>
#include <libtcod.hpp>

#include "engine.hpp"
#include "events/engine_event.hpp"
#include "events/melee_command.hpp"
#include "events/movement_command.hpp"
#include "game_entity.hpp"

namespace cpprl {

bool can_path_to_target(tcod::BresenhamLine& path, Engine& engine) {
  for (const auto [x, y] : path) {
    if (engine.get_entities().get_blocking_entity_at({x, y})) {
      return false;
    }
  }

  return true;
}

void HostileAI::update(Engine& engine, Entity* entity) {
  Vector2D position = entity->get_transform_component()->get_position();
  if (engine.get_map()->is_in_fov(position)) {
    Entity* player = engine.get_player();
    Vector2D player_position =
        player->get_transform_component()->get_position();
    Vector2D delta = player_position - position;

    int distance = std::max(std::abs(delta.x), std::abs(delta.y));
    if (distance <= 1) {
      auto melee_command = MeleeCommand(engine, entity, delta);
      melee_command.execute();
    }

    tcod::BresenhamLine path =
        tcod::BresenhamLine({position.x, position.y}, {player_position.x, player_position.y}).without_endpoints();

    if (can_path_to_target(path, engine)) {
      auto dest = path[0];
      auto destination = Vector2D{dest[0], dest[1]} - position;

      auto action = MovementCommand(engine, entity, destination);
      action.execute();

      return;
    }

    auto action = NoOpEvent(engine);
    action.execute();
  }
}

}  // namespace cpprl
