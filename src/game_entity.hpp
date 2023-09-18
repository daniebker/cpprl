#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H
#include "draw.hpp"
#include "math.hpp"

namespace cpprl {
class GameEntity {
 public:
  GameEntity(Vector2D position, char symbol, RGB_Colour colour);
  virtual ~GameEntity();
  void move(Vector2D& vector2D);
  int get_x();
  int get_y();

 private:
  Vector2D position_;
  char symbol_;
  RGB_Colour colour_;
};
}  // namespace cpprl

#endif
