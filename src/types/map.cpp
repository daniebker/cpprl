#include "map.hpp"

namespace cpprl {
Map::Map(int width, int height) : width_(width), height_(height), tiles_(width, height) {
  tiles_ = {width, height};

  for (int px = 0; px < width; px++) {
    for (int py = 0; py < height; py++) {
      tiles_.at(px, py) = Tiles::floor;
    }
  }

  tiles_.at({30, 22}) = Tiles::wall;
  tiles_.at({50, 22}) = Tiles::wall;
}

Map::~Map() {}

bool Map::is_in_bounds(Vector2D position) const {
  return position.x >= 0 && position.x < width_ && position.y >= 0 && position.y < height_;
}

// void Map::render(tcod::Console& console) {
//   for (int px = 0; px < width_; px++) {
//     for (int py = 0; py < height; py++) {
//       console.at({x, y}) = tiles_.at(x, y) == Tiles::floor
//                                ? TCOD_ConsoleTile{'.', tcod::ColorRGB{128, 128, 128}, tcod::ColorRGB{0, 0, 0}}
//                                : TCOD_ConsoleTile{'#', tcod::ColorRGB{128, 128, 128}, tcod::ColorRGB{0, 0, 0}};
//     }
//   }
}  // namespace cpprl
