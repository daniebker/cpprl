#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <vector>

#include "game_entity.hpp"
#include "persistent.hpp"
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
class Dungeon : public Persistent {
 private:
  std::vector<Vector2D> l_tunnel_between(Vector2D start, Vector2D end);
  TCODRandom* rng_;
  long seed_;

 public:
  Dungeon() {
    seed_ = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
  };
  virtual ~Dungeon() { delete rng_; };
  Map* generate(DungeonConfig config);
  // void save(TCODZip& zip) override;
  void save(cereal::JSONOutputArchive& archive) override;
  // void load(TCODZip& zip) override;
  void load(cereal::JSONInputArchive& archive) override;
};
}  // namespace cpprl

#endif
