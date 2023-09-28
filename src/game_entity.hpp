#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H
#include <libtcod.hpp>
#include <string_view>

#include "colours.hpp"
#include "types/math.hpp"

namespace cpprl {
class GameEntity {
 public:
  GameEntity(
      std::string_view name, Vector2D position, std::string_view symbol, const tcod::ColorRGB& colour, bool blocker);
  void move(Vector2D& vector2D);
  std::string_view get_symbol() { return symbol_; };
  tcod::ColorRGB get_colour() { return colour_; };
  Vector2D get_position() { return position_; };
  void set_position(Vector2D position) { position_ = position; };
  bool is_blocking() { return blocker_; };
  std::string_view get_name() { return name_; };

 private:
  std::string_view name_;
  Vector2D position_;
  std::string_view symbol_;
  tcod::ColorRGB colour_;
  bool blocker_;
};

static const GameEntity PLAYER{"player", {0, 0}, "@", RED, true};
static const GameEntity ORC{"orc", {0, 0}, "o", WHITE, true};
static const GameEntity TROLL{"troll", {0, 0}, "T", WHITE, true};
}  // namespace cpprl

#endif
