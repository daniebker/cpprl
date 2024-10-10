#include "basic_ai_component.hpp"

#include <algorithm>
#include <libtcod.hpp>

#include "events/command.hpp"
#include "game_entity.hpp"
#include "entity_manager.hpp"
#include "world.hpp"
#include <core/math.hpp>

namespace cpprl {

bool can_path_to_target(tcod::BresenhamLine& path, World& world) {
  for (const auto [x, y] : path) {
    if (world.get_entities().get_blocking_entity_at({x, y})) {
      return false;
    }
  }

  return true;
}

void HostileAI::update(World& world, Entity* entity) {
  SupaRL::Vector2D position = entity->get_transform_component().position_;
  if (world.get_map().is_in_fov(position)) {
    Entity* player = world.get_player();
    SupaRL::Vector2D player_position = player->get_transform_component().position_;
    SupaRL::Vector2D delta = player_position - position;

    int distance = std::max(std::abs(delta.x), std::abs(delta.y));
    if (distance <= 1) {
      auto melee_command = MeleeCommand(world, entity, delta);
      melee_command.execute();
    }

    tcod::BresenhamLine path =
        tcod::BresenhamLine({position.x, position.y}, {player_position.x, player_position.y}).without_endpoints();

    if (can_path_to_target(path, world)) {
      auto dest = path[0];
      auto destination = SupaRL::Vector2D{dest[0], dest[1]} - position;

      auto action = MovementCommand(world, entity, destination);
      action.execute();

      return;
    }

    auto action = NoOpEvent(world);
    action.execute();
  }
}

ConfusionAI::ConfusionAI(int num_turns, std::unique_ptr<AIComponent> old_ai)
    : num_turns_(num_turns), old_ai_(std::move(old_ai)) {}
void ConfusionAI::update(World& world, Entity* entity) {
  TCODRandom* random = TCODRandom::getInstance();
  int dx = random->getInt(-1, 1);
  int dy = random->getInt(-1, 1);
  if ((dx != 0 || dy != 0) && num_turns_ > 0) {
    auto action = DirectionalCommand(world, entity, {dx, dy});
    action.execute();
    --num_turns_;
  } else {
    entity->set_ai_component(std::move(old_ai_));
  }
}
}  // namespace cpprl
