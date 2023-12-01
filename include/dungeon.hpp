#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <vector>

#include "game_entity.hpp"
#include "types/map.hpp"
#include "types/tile.hpp"

namespace cpprl {
struct DungeonConfig {
  int max_rooms;
  int room_min_size;
  int room_max_size;
  int map_width;
  int map_height;
  int max_monsters_per_room;
};

class Dungeon {
 private:
  std::vector<Vector2D> l_tunnel_between(Vector2D start, Vector2D end);
  TCODRandom rng_;
  int seed_;
  int level_ = 0;
  std::unique_ptr<Map> current_map_;

 public:
  Dungeon() {
    seed_ = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
  };
  virtual ~Dungeon() = default;

  // std::unique_ptr<Map> generate(DungeonConfig config);
  void generate(DungeonConfig config);
  int get_level() const { return level_; }
  int increase_level() { return ++level_; }
  Map& get_map() { return *current_map_; }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(seed_, level_);
  }
};

}  // namespace cpprl

#endif
