#pragma once

#include <SDL2/SDL.h>

#include <tuple>

#include "types/math.hpp"

namespace cpprl {
class RectangularRoom {
 private:
  Vector2D bottomLeft_, topRight_, center_;

 public:
  RectangularRoom(Vector2D bottomLeft, int width, int height) {
    bottomLeft_ = bottomLeft;
    topRight_ = {bottomLeft.x + width, bottomLeft.y + height};
    center_ = (bottomLeft_ + topRight_) / 2;
  };
  ~RectangularRoom(){};

  /** Return the innerBounds area of this room as a 2D array index */
  std::tuple<Vector2D, Vector2D> innerBounds() const;
  bool intersects(RectangularRoom other) const;
  Vector2D get_bottom_left() const { return bottomLeft_; }
  Vector2D get_top_right() const { return topRight_; }
  Vector2D get_center() { return center_; }
};
}  // namespace cpprl
