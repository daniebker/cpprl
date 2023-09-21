#include "game_entity.hpp"

#include <libtcod.hpp>
#include <string_view>

namespace cpprl {
GameEntity::GameEntity(Vector2D position, std::string_view symbol, const tcod::ColorRGB& colour) {
  position_ = position;
  symbol_ = symbol;
  colour_ = colour;
}

GameEntity::~GameEntity() {}

void GameEntity::move(cpprl::Vector2D& vector2D) {
  position_.x += vector2D.x;
  position_.y += vector2D.y;
}
}  // namespace cpprl
