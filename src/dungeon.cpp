#include "dungeon.hpp"

#include <tuple>

#include "rectangular_room.hpp"
#include "types/map.hpp"

namespace cpprl {
Map Dungeon::generate(int width, int height) {
  map_ = Map(width, height);

  RectangularRoom room1 = RectangularRoom({20, 15}, 10, 15);
  RectangularRoom room2 = RectangularRoom({35, 15}, 10, 15);

  Vector2D room1InnerBottomLeft, room1InnerTopRight;
  std::tie(room1InnerBottomLeft, room1InnerTopRight) = room1.innerBounds();
  for (int x = room1InnerBottomLeft.x; x < room1InnerTopRight.x; x++) {
    for (int y = room1InnerBottomLeft.y; y < room1InnerTopRight.y; y++) {
      map_.get_tiles().set({x, y}, Tiles::floor);
    }
  }

  Vector2D room2InnerBottomLeft, room2InnerTopRight;
  std::tie(room2InnerBottomLeft, room2InnerTopRight) = room2.innerBounds();
  for (int x = room2InnerBottomLeft.x; x < room2InnerTopRight.x; x++) {
    for (int y = room2InnerBottomLeft.y; y < room2InnerTopRight.y; y++) {
      map_.get_tiles().set({x, y}, Tiles::floor);
    }
  }

  return map_;
}
}  // namespace cpprl
