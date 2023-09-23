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

  return map_;
}
}  // namespace cpprl
