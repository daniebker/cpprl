#include "map.hpp"

namespace cpprl {
Map::Map(int width, int height) : width_(width), height_(height), tiles_(width, height, Tiles::wall) {}

Map::~Map() {}

bool Map::is_in_bounds(Vector2D position) const {
  return position.x >= 0 && position.x < width_ && position.y >= 0 && position.y < height_;
}

}  // namespace cpprl
