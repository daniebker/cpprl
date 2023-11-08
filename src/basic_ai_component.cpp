#include "basic_ai_component.hpp"

#include <algorithm>
#include <iostream>
#include <libtcod.hpp>

#include "entity_manager.hpp"
#include "events/command.hpp"
#include "game_entity.hpp"
#include "types/map.hpp"
#include "world.hpp"

namespace cpprl {

bool can_path_to_target(tcod::BresenhamLine& path, World& world) {
  for (const auto [x, y] : path) {
    if (world.get_entities().get_blocking_entity_at({x, y})) {
      return false;
    }
  }

  return true;
}

AIComponent* AIComponent::create(TCODZip& zip) {
  AiType type = static_cast<AiType>(zip.getInt());
  AIComponent* ai = nullptr;
  switch (type) {
    case HOSTILE:
      ai = new HostileAI();
      break;
    case CONFUSED:
      ai = new ConfusionAI(0, nullptr);
      break;
  }
  ai->load(zip);
  return ai;
}

void HostileAI::update(World& world, Entity* entity) {
  Vector2D position = entity->get_transform_component().get_position();
  if (world.get_map().is_in_fov(position)) {
    Entity* player = world.get_player();
    Vector2D player_position =
        player->get_transform_component().get_position();
    Vector2D delta = player_position - position;

    int distance = std::max(std::abs(delta.x), std::abs(delta.y));
    if (distance <= 1) {
      auto melee_command = MeleeCommand(world, entity, delta);
      melee_command.execute();
    }

    tcod::BresenhamLine path =
        tcod::BresenhamLine({position.x, position.y}, {player_position.x, player_position.y}).without_endpoints();

    if (can_path_to_target(path, world)) {
      auto dest = path[0];
      auto destination = Vector2D{dest[0], dest[1]} - position;

      auto action = MovementCommand(world, entity, destination);
      action.execute();

      return;
    }

    auto action = NoOpEvent(world);
    action.execute();
  }
}
void HostileAI::load(TCODZip& zip) {}
void HostileAI::save(TCODZip& zip) { zip.putInt(HOSTILE); }

void ConfusionAI::update(World& world, Entity* entity) {
  TCODRandom* random = TCODRandom::getInstance();
  int dx = random->getInt(-1, 1);
  int dy = random->getInt(-1, 1);
  Vector2D move_vector_{dx, dy};
  if ((dx != 0 || dy != 0) && num_turns_ > 0) {
    auto action = DirectionalCommand(world, entity, {dx, dy});
    action.execute();
    --num_turns_;
  } else {
    entity->set_ai_component(old_ai_);
    delete this;
  }
}

void ConfusionAI::load(TCODZip& zip) {
  num_turns_ = zip.getInt();
  old_ai_ = AIComponent::create(zip);
}

void ConfusionAI::save(TCODZip& zip) {
  zip.putInt(CONFUSED);
  zip.putInt(num_turns_);
  old_ai_->save(zip);
}
}  // namespace cpprl
