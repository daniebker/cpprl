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
  TCODRandom* rng_;
  long seed_;

 public:
  Dungeon() {
    seed_ = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
  };
  virtual ~Dungeon() { delete rng_; };
  std::unique_ptr<Map> generate(DungeonConfig config);

  template <class Archive>
  void serialize(Archive& archive) {
    archive(seed_);
  }
};
}  // namespace cpprl

#endif
