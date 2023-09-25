#include "map.hpp"

namespace cpprl {
Map::Map(int width, int height)
    : width_(width), height_(height), tiles_(width, height, {false, TileType::wall}), tcod_map_(width, height) {
  wall_tile_.light = TCOD_ConsoleTile{'#', tcod::ColorRGB{255, 255, 255}, tcod::ColorRGB{0, 0, 0}};
  wall_tile_.dark = TCOD_ConsoleTile{'#', tcod::ColorRGB{128, 128, 128}, tcod::ColorRGB{0, 0, 0}};
  floor_tile_.light = TCOD_ConsoleTile{'.', tcod::ColorRGB{255, 255, 255}, tcod::ColorRGB{0, 0, 0}};
  floor_tile_.dark = TCOD_ConsoleTile{'.', tcod::ColorRGB{128, 128, 128}, tcod::ColorRGB{0, 0, 0}};
}

Map::~Map() {}

bool Map::is_in_bounds(Vector2D position) const {
  return position.x >= 0 && position.x < width_ && position.y >= 0 && position.y < height_;
}

void Map::set_tiles_at(std::tuple<Vector2D, Vector2D> bounds, Tile tile) {
  Vector2D bottom_left, top_right;
  std::tie(bottom_left, top_right) = bounds;
  for (int x = bottom_left.x; x < top_right.x; x++) {
    for (int y = bottom_left.y; y < top_right.y; y++) {
      tcod_map_.setProperties(x, y, tile.tile == TileType::floor, tile.tile == TileType::floor);
      tiles_.set({x, y}, tile);
    }
  }
}
bool Map::is_in_fov(Vector2D position) { return tcod_map_.isInFov(position.x, position.y); }

void Map::compute_fov(Vector2D position, int radius) { tcod_map_.computeFov(position.x, position.y, radius); }
bool Map::is_explored(Vector2D position) { return tiles_.at(position).explored; }
}  // namespace cpprl
