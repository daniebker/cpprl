#include "../../include/types/map.hpp"

namespace cpprl {
Map::Map(int width, int height)
    : width_(width),
      height_(height),
      tiles_(width, height, {false, TileType::wall}),
      tcod_map_(width, height) {
  wall_tile_.light = TCOD_ConsoleTile{'#', WHITE, BLACK};
  wall_tile_.dark = TCOD_ConsoleTile{'#', GREY, BLACK};
  floor_tile_.light = TCOD_ConsoleTile{'.', WHITE, BLACK};
  floor_tile_.dark = TCOD_ConsoleTile{'.', GREY, BLACK};
}

Map::~Map() {}

bool Map::is_in_bounds(Vector2D position) const {
  return position.x >= 0 && position.x < width_ && position.y >= 0 &&
         position.y < height_;
}

void Map::set_tiles_range(std::tuple<Vector2D, Vector2D> bounds, Tile tile) {
  Vector2D bottom_left, top_right;
  std::tie(bottom_left, top_right) = bounds;
  for (int x = bottom_left.x; x < top_right.x; x++) {
    for (int y = bottom_left.y; y < top_right.y; y++) {
      tcod_map_.setProperties(
          x, y, tile.type == TileType::floor, tile.type == TileType::floor);
      tiles_.set({x, y}, tile);
    }
  }
}
void Map::set_tiles_at(Vector2D position, Tile tile) {
  tcod_map_.setProperties(
      position.x,
      position.y,
      tile.type == TileType::floor,
      tile.type == TileType::floor);
  tiles_.set(position, tile);
}

bool Map::is_in_fov(Vector2D position) {
  return tcod_map_.isInFov(position.x, position.y);
}

bool Map::is_walkable(Vector2D position) const {
  return tcod_map_.isWalkable(position.x, position.y);
}

void Map::compute_fov(Vector2D position, int radius) {
  tcod_map_.computeFov(
      position.x, position.y, radius, true, FOV_SYMMETRIC_SHADOWCAST);
}

bool Map::is_explored(Vector2D position) {
  return tiles_.at(position).explored;
}

void Map::render(tcod::Console& console) {
  console.clear();
  // TODO: Should happen in the map render function
  for (int y{0}; y < get_height(); ++y) {
    for (int x{0}; x < get_width(); ++x) {
      if (!console.in_bounds({x, y})) continue;
      bool isFloor = get_tiles().at({x, y}).type == TileType::floor;
      if (is_in_fov({x, y})) {
        set_is_explored({x, y});
        console.at({x, y}) =
            isFloor ? get_floor_tile().light : get_wall_tile().light;
      } else if (is_explored({x, y})) {
        console.at({x, y}) =
            isFloor ? get_floor_tile().dark : get_wall_tile().dark;
      } else {
        console.at({x, y}) = TCOD_ConsoleTile{' ', BLACK_DARK, BLACK_DARK};
      }
    }
  }
  if (is_in_fov(target_tile_)) {
  }
}
}  // namespace cpprl
