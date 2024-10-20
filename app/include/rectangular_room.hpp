#pragma once

#include <SDL2/SDL.h>
#include <core/math.hpp>

#include <tuple>

namespace cpprl {
  class RectangularRoom {
    private:
      SupaRL::Vector2D bottomLeft_, topRight_, center_;

    public:
      RectangularRoom(SupaRL::Vector2D bottomLeft, int width, int height) {
        bottomLeft_ = bottomLeft;
        topRight_ = {bottomLeft.x + width, bottomLeft.y + height};
        center_ = (bottomLeft_ + topRight_) / 2;
      };
      ~RectangularRoom(){};

      /** Return the innerBounds area of this room as a 2D array index */
      std::tuple<SupaRL::Vector2D, SupaRL::Vector2D> innerBounds() const;
      bool intersects(RectangularRoom other) const;
      SupaRL::Vector2D get_bottom_left() const { return bottomLeft_; }
      SupaRL::Vector2D get_top_right() const { return topRight_; }
      SupaRL::Vector2D get_center() { return center_; }
  };
}
