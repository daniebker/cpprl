#pragma once

#include <libtcod.hpp>

#include "../math.hpp"
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
  bool is_in_bounds(Vector2D position) const;
  bool is_not_in_bounds(Vector2D position) const { return !is_in_bounds(position); }
  Array2D<Tiles>& get_tiles() { return tiles_; }
  void set_tiles_at(std::tuple<Vector2D, Vector2D> bounds, Tiles tile);

 private:
  int width_, height_;
  Array2D<Tiles> tiles_;
};

}  // namespace cpprl
