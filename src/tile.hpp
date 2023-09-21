#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <libtcod.hpp>

namespace cpprl {

struct Tile {
  std::string_view name;
  int character;
  tcod::ColorRGB foreground;
}

}  // namespace cpprl
#endif
