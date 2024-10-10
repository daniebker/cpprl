#pragma once

#include <libtcod.hpp>

namespace cpprl {

  struct Tile {
    std::string_view name;
    int character;
    tcod::ColorRGB foreground;
  };

}
