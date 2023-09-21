#ifndef MAP_HPP
#define MAP_HPP
#pragma once

#include <libtcod.hpp>

#include "nparray.hpp"

namespace cpprl {
enum class Tiles {
  wall = 0,
  floor,
};

class Map {
 public:
  Map(int width, int height);
  ~Map();
  void render(tcod::Console& console);
  int get_height() const { return height_; }
  int get_width() const { return width_; }
  Array2D<Tiles> get_tiles() const { return tiles_; }

 private:
  int width_, height_;
  Array2D<Tiles> tiles_;
  // bool visible_[][];
};

}  // namespace cpprl

#endif
