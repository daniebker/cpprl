#include "game_entity.hpp"

#include <libtcod.hpp>
#include <string_view>

namespace cpprl {
GameEntity::GameEntity(
    std::string_view name, Vector2D position, std::string_view symbol, const tcod::ColorRGB& colour, bool blocker) {
  position_ = position;
  symbol_ = symbol;
  colour_ = colour;
  name_ = name;
  blocker_ = blocker;
}

void GameEntity::move(cpprl::Vector2D& vector2D) { position_ = vector2D; }
}  // namespace cpprl
