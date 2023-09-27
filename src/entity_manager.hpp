#pragma once

#include <vector>

#include "game_entity.hpp"

namespace cpprl {
class EntityManager {
 public:
  clear();
  Entity* get_blocking_entity_at(Vector2D position);
  void place_entities(RectangularRoom room, int max_monsters_per_room);
  Entity& spawn(const Entity& entity);
  Entity& spawn(const Entity& entity, Vector2D position);

  using iterator = std::vector<Entity>::iterator;
  using const_iterator = std::vector<Entity>::const_iterator;

  iterator begin() { return entities_.begin(); }

  iterator end() { return entities_.end(); }

  const_iterator begin() const { return entities_.begin(); }

  const_iterator end() const { return entities_.end(); }

 private:
  std::vector<GameEntity> entities_;
}
}  // namespace cpprl
