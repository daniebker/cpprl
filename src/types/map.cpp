#include "types/map.hpp"

#include "colours.hpp"
#include "components.hpp"

namespace cpprl {
Map::Map(int width, int height)
    : width_(width),
      height_(height),
      tiles_(width, height, {false, TileType::wall}),
      tcod_map_(width, height) {
  wall_tile_.light = TCOD_ConsoleTile{'#', WHITE, BLACK};
  wall_tile_.dark = TCOD_ConsoleTile{'#', GREY, BLACK};
  floor_tile_.light = TCOD_ConsoleTile{'.', WHITE, BLACK};
  floor_tile_.target = TCOD_ConsoleTile{'.', TEAL, BLACK};
  floor_tile_.dark = TCOD_ConsoleTile{'.', GREY, BLACK};
  target_tile_ = Vector2D{0, 0};
}

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

  for (int y{0}; y < get_height(); ++y) {
    for (int x{0}; x < get_width(); ++x) {
      if (!console.in_bounds({x, y})) continue;
      bool isFloor = get_tiles().at({x, y}).type == TileType::floor;
      if (is_in_fov({x, y})) {
        set_is_explored({x, y});
        // if mode is targeting change the tile colours to highlight
        TCOD_ConsoleTile floor =
            target_mode_ ? get_floor_tile().target : get_floor_tile().light;
        console.at({x, y}) = isFloor ? floor : get_wall_tile().light;
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

void Map::set_highlight_tile(Vector2D position) { target_tile_ = position; }

// void Map::save(TCODZip& zip) {
//   for (int y = 0; y < get_height(); y++) {
//     for (int x = 0; x < get_width(); x++) {
//       zip.putInt(tiles_.at({x, y}).explored);
//     }
//   }
// }
// void Map::save(cereal::JSONOutputArchive& archive) {
//   for (int y = 0; y < get_height(); y++) {
//     for (int x = 0; x < get_width(); x++) {
//       archive(tiles_.at({x, y}).explored);
//     }
//   }
// }

// void Map::load(TCODZip& zip) {
//   for (int y = 0; y < get_height(); y++) {
//     for (int x = 0; x < get_width(); x++) {
//       bool explored = zip.getInt();
//       tiles_.at({x, y}).explored = explored;
//     }
//   }
// }
// void Map::load(cereal::JSONInputArchive& archive) {
//   for (int y = 0; y < get_height(); y++) {
//     for (int x = 0; x < get_width(); x++) {
//       bool explored;
//       archive(explored);
//       tiles_.at({x, y}).explored = explored;
//     }
//   }
// }
}  // namespace cpprl
