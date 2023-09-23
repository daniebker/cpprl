#include "map.hpp"

namespace cpprl {
Map::Map(int width, int height) : width_(width), height_(height), tiles_(width, height, Tiles::wall) {}

Map::~Map() {}

bool Map::is_in_bounds(Vector2D position) const {
  return position.x >= 0 && position.x < width_ && position.y >= 0 && position.y < height_;
}

void Map::set_tiles_at(std::tuple<Vector2D, Vector2D> bounds, Tiles tile) { tiles_.set_range(bounds, tile); }

}  // namespace cpprl
