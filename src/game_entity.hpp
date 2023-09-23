#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H
#include <libtcod.hpp>
#include <string_view>

#include "types/math.hpp"

namespace cpprl {
class GameEntity {
 public:
  GameEntity(Vector2D position, std::string_view symbol, const tcod::ColorRGB& colour);
  virtual ~GameEntity();
  void move(Vector2D& vector2D);
  std::string_view get_symbol() { return symbol_; };
  tcod::ColorRGB get_colour() { return colour_; };
  Vector2D get_position() { return position_; };

  [[deprecated("Use get_position() instead.")]] int get_x() { return position_.x; }
  [[deprecated("Use get_position() instead.")]] int get_y() { return position_.y; };

 private:
  Vector2D position_;
  std::string_view symbol_;
  tcod::ColorRGB colour_;
};
}  // namespace cpprl

#endif
