#pragma once

#include <vector>

#include "game_entity.hpp"
#include "types/map.hpp"

namespace cpprl {
class Dungeon {
 private:
  std::vector<Vector2D> l_tunnel_between(Vector2D start, Vector2D end);

 public:
  Dungeon(){};
  Map* generate(int max_rooms, int room_min_size, int room_max_size, int map_width, int map_height, GameEntity& player);
};
}  // namespace cpprl
