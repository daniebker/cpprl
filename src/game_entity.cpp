#include "game_entity.hpp"

namespace cpprl {
GameEntity::GameEntity(Vector2D position, char symbol, RGB_Colour colour) {
  position_ = position;
  symbol_ = symbol;
  colour_ = colour;
}

GameEntity::~GameEntity() {}

int GameEntity::get_x() { return position_.x; }
int GameEntity::get_y() { return position_.y; }

void GameEntity::move(cpprl::Vector2D& vector2D) {
  position_.x += vector2D.x;
  position_.y += vector2D.y;
}
}  // namespace cpprl
