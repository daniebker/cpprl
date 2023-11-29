#include "dungeon.hpp"

#include <memory>
#include <tuple>

#include "rectangular_room.hpp"
#include "types/map.hpp"
#include "types/tile.hpp"

namespace cpprl {
std::unique_ptr<Map> Dungeon::generate(DungeonConfig dungeon_config) {
  // TODO: Is there a chance that these overflow?
  u_int32_t level_seed = seed_ + level_;
  rng_ = TCODRandom(level_seed, TCOD_RNG_CMWC);
  auto map = std::make_unique<Map>(
      dungeon_config.map_width, dungeon_config.map_height);
  auto rooms = std::vector<RectangularRoom>{};

  Vector2D last_room_center = {0, 0};
  for (int i = 0; i < dungeon_config.max_rooms; i++) {
    int room_width =
        rng_.getInt(dungeon_config.room_min_size, dungeon_config.room_max_size);
    int room_height =
        rng_.getInt(dungeon_config.room_min_size, dungeon_config.room_max_size);

    int x = rng_.getInt(0, dungeon_config.map_width - room_width - 1);
    int y = rng_.getInt(0, dungeon_config.map_height - room_height - 1);

    RectangularRoom new_room = RectangularRoom({x, y}, room_width, room_height);

    if (std::any_of(rooms.begin(), rooms.end(), [&](RectangularRoom room) {
          return room.intersects(new_room);
        })) {
      continue;
    }

    map->set_tiles_range(new_room.innerBounds(), FLOOR_TILE);

    last_room_center = new_room.get_center();
    if (!rooms.empty()) {
      Vector2D previous_room_center = rooms.back().get_center();
      std::vector<Vector2D> tunnel =
          l_tunnel_between(previous_room_center, new_room.get_center());

      for (const Vector2D position : tunnel) {
        map->set_tiles_at(position, FLOOR_TILE);
      }
    }
    rooms.push_back(new_room);
  }
  map->set_tiles_at(last_room_center, DOWN_STAIRS_TILE);
  map->set_down_stairs_location(last_room_center);
  map->set_rooms(rooms);
  level_++;
  return map;
}

constexpr float half_chance = 0.5F;
std::vector<Vector2D> Dungeon::l_tunnel_between(Vector2D start, Vector2D end) {
  Vector2D corner{0, 0};

  if (rng_.get(0.0f, 1.0f) < half_chance) {
    corner = {end.x, start.y};
  } else {
    corner = {start.x, end.y};
  }

  std::vector<Vector2D> tunnel{};

  for (const auto&& [x, y] :
       tcod::BresenhamLine({start.x, start.y}, {corner.x, corner.y})) {
    tunnel.push_back({x, y});
  }

  for (const auto&& [x, y] :
       tcod::BresenhamLine({corner.x, corner.y}, {end.x, end.y})) {
    tunnel.push_back({x, y});
  }

  return tunnel;
}
}  // namespace cpprl
