#include "dungeon.hpp"

#include <tuple>

#include "rectangular_room.hpp"
#include "types/map.hpp"

namespace cpprl {
Map Dungeon::generate(int width, int height) {
  map_ = Map(width, height);

  RectangularRoom room1 = RectangularRoom({20, 15}, 10, 15);
  RectangularRoom room2 = RectangularRoom({35, 15}, 10, 15);

  map_.set_tiles_at(room1.innerBounds(), Tiles::floor);
  map_.set_tiles_at(room2.innerBounds(), Tiles::floor);

  std::vector<Vector2D> tunnel = l_tunnel_between(room1.get_center(), room2.get_center());

  for (const Vector2D position : tunnel) {
    map_.get_tiles().set(position, Tiles::floor);
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
