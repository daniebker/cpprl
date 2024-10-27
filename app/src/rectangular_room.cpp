#include "rectangular_room.hpp"

namespace cpprl {

  std::tuple<SupaRL::Vector2D, SupaRL::Vector2D> RectangularRoom::innerBounds() const {
    SupaRL::Vector2D innerBottomLeft, innerTopRight;
    innerBottomLeft = {bottomLeft_.x + 1, bottomLeft_.y + 1};
    innerTopRight = {topRight_.x - 1, topRight_.y - 1};
    return std::tuple<SupaRL::Vector2D, SupaRL::Vector2D>(innerBottomLeft, innerTopRight);
  }

  bool RectangularRoom::intersects(RectangularRoom other) const {
    return (
        bottomLeft_.x <= other.topRight_.x && topRight_.x >= other.bottomLeft_.x && bottomLeft_.y <= other.topRight_.y &&
        topRight_.y >= other.bottomLeft_.y);
  }
}
