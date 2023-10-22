#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <vector>

#include "game_entity.hpp"
#include "types/map.hpp"

struct DungeonConfig {
  int max_rooms;
  int room_min_size;
  int room_max_size;
  int map_width;
  int map_height;
  int max_monsters_per_room;
};

namespace cpprl {
class Dungeon {
 private:
  std::vector<Vector2D> l_tunnel_between(Vector2D start, Vector2D end);

 public:
  Dungeon(){};
  ~Dungeon() = default;
  std::unique_ptr<Map> generate(DungeonConfig config);
};
}  // namespace cpprl

#endif
