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
  Entity* get_blocking_entity_at(Vector2D position);
  Entity* get_non_blocking_entity_at(Vector2D position);
  Entity* get_closest_monster(Vector2D position, float range) const;
  std::vector<Entity*> get_entities_at(Vector2D position);
  void place_entities(
      RectangularRoom room, int max_monsters_per_room, int max_items_per_room);
  Entity* spawn(Entity* entity);
  Entity* spawn(Entity* entity, Vector2D position);
  void reserve(size_t size) { entities_.reserve(size); }
  void shrink_to_fit() { entities_.shrink_to_fit(); }
  void remove(Entity* entity);
  size_t size() const { return entities_.size(); }

  Entity& at(int index) { return *entities_.at(index); }

  using iterator = std::vector<Entity*>::iterator;
  using const_iterator = std::vector<Entity*>::const_iterator;

  iterator begin() { return entities_.begin(); }

  iterator end() { return entities_.end(); }

  const_iterator begin() const { return entities_.begin(); }

  const_iterator end() const { return entities_.end(); }

 private:
  std::vector<Entity*> entities_;
};
}  // namespace cpprl
