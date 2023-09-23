#include "map.hpp"

namespace cpprl {
Map::Map(int width, int height) : width_(width), height_(height), tiles_(width, height, Tiles::wall) {}

Map::~Map() {}

bool Map::is_in_bounds(Vector2D position) const {
  return position.x >= 0 && position.x < width_ && position.y >= 0 && position.y < height_;
}

void Map::set_tiles_at(std::tuple<Vector2D, Vector2D> bounds, Tiles tile) {
  Vector2D bottom_left, top_right;
  std::tie(bottom_left, top_right) = bounds;
  for (int x = bottom_left.x; x < top_right.x; x++) {
    for (int y = bottom_left.y; y < top_right.y; y++) {
      tiles_.set({x, y}, tile);
    }
  }
}

}  // namespace cpprl
