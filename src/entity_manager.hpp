#pragma once

#include <vector>

#include "game_entity.hpp"
#include "rectangular_room.hpp"
#include "types/map.hpp"

namespace cpprl {
class EntityManager {
 public:
  EntityManager() : entities_(){};
  void clear();
  GameEntity* get_blocking_entity_at(Vector2D position);
  void place_entities(RectangularRoom room, int max_monsters_per_room);
  GameEntity& spawn(const GameEntity& entity);
  GameEntity& spawn(const GameEntity& entity, Vector2D position);
  GameEntity& spawn_player(Vector2D position);

  GameEntity& at(int index) { return entities_.at(index); }

  using iterator = std::vector<GameEntity>::iterator;
  using const_iterator = std::vector<GameEntity>::const_iterator;

  iterator begin() { return entities_.begin(); }

  iterator end() { return entities_.end(); }

  const_iterator begin() const { return entities_.begin(); }

  const_iterator end() const { return entities_.end(); }

 private:
  std::vector<GameEntity> entities_;
};
}  // namespace cpprl
