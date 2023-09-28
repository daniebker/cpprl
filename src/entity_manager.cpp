#include "../include/entity_manager.hpp"

#include <SDL2/SDL.h>

namespace cpprl {

void EntityManager::clear() { entities_.clear(); }

void EntityManager::place_entities(RectangularRoom room, int max_monsters_per_room) {
  auto* random = TCODRandom::getInstance();
  int number_of_monsters = random->getInt(0, max_monsters_per_room);

  for (int i = 0; i < number_of_monsters; i++) {
    Vector2D bottom_left, top_right;
    std::tie(bottom_left, top_right) = room.innerBounds();
    int x = random->getInt(bottom_left.x + 1, top_right.x - 1);
    int y = random->getInt(bottom_left.y + 1, top_right.y - 1);

    // Check theres no other monster on this position in the room
    auto iterator = std::find_if(entities_.begin(), entities_.end(), [x, y](GameEntity& entity) {
      return entity.get_position() == Vector2D{x, y};
    });

    if (iterator != entities_.end()) {
      continue;
    }

    if (random->getFloat(0.0f, 1.0f) < 0.8f) {
      spawn(ORC, {x, y});
    } else {
      spawn(TROLL, {x, y});
    }
  }
}

GameEntity& EntityManager::spawn(const GameEntity& src) { return entities_.emplace_back(src); }

GameEntity& EntityManager::spawn(const GameEntity& src, Vector2D position) {
  auto& entity = spawn(src);

  if (position != entity.get_position()) {
    entity.set_position(position);
  }

  return entity;
}

GameEntity& EntityManager::spawn_player(Vector2D position) {
  auto& player = spawn(PLAYER, position);
  return player;
}

GameEntity* EntityManager::get_blocking_entity_at(Vector2D position) {
  for (auto& entity : entities_) {
    if (entity.is_blocking() && entity.get_position() == position) {
      return &entity;
    }
  }
  return nullptr;
}
}  // namespace cpprl
