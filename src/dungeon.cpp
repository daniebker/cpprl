#include "dungeon.hpp"

#include <tuple>

#include "rectangular_room.hpp"
#include "types/map.hpp"

namespace cpprl {
Map Dungeon::generate(
    int max_rooms, int room_min_size, int room_max_size, int map_width, int map_height, GameEntity& player) {
  map_ = Map(map_width, map_height);
  auto rooms = std::vector<RectangularRoom>{};

  auto* random = TCODRandom::getInstance();
  for (int i = 0; i < max_rooms; i++) {
    int room_width = random->getInt(room_min_size, room_max_size);
    int room_height = random->getInt(room_min_size, room_max_size);

    int x = random->getInt(0, map_width - room_width - 1);
    int y = random->getInt(0, map_height - room_height - 1);

    RectangularRoom new_room = RectangularRoom({x, y}, room_width, room_height);

    if (std::any_of(rooms.begin(), rooms.end(), [&](RectangularRoom room) { return room.intersects(new_room); })) {
      continue;
    }

    map_.set_tiles_at(new_room.innerBounds(), Tiles::floor);

    if (rooms.empty()) {
      player.set_position(new_room.get_center());
    } else {
      Vector2D previous_room_center = rooms.back().get_center();
      std::vector<Vector2D> tunnel = l_tunnel_between(previous_room_center, new_room.get_center());

      for (const Vector2D position : tunnel) {
        map_.get_tiles().set(position, Tiles::floor);
      }
    }
  }

  return map_;
}

constexpr float half_chance = 0.5F;

std::vector<Vector2D> Dungeon::l_tunnel_between(Vector2D start, Vector2D end) {
  auto* random = TCODRandom::getInstance();
  Vector2D corner{0, 0};

  if (random->get(0.0f, 1.0f) < half_chance) {
    corner = {end.x, start.y};
  } else {
    corner = {start.x, end.y};
  }

  std::vector<Vector2D> tunnel{};

  for (const auto&& [x, y] : tcod::BresenhamLine({start.x, start.y}, {corner.x, corner.y})) {
    tunnel.push_back({x, y});
  }

  for (const auto&& [x, y] : tcod::BresenhamLine({corner.x, corner.y}, {end.x, end.y})) {
    tunnel.push_back({x, y});
  }

  return tunnel;
}

}  // namespace cpprl
