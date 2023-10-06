#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H
#include <libtcod.hpp>
#include <string_view>

#include "colours.hpp"
#include "components.hpp"
#include "types/math.hpp"

namespace cpprl {
class Engine;
class GameEntity {
 public:
  GameEntity(
      std::string name,
      bool blocker,
      TransformComponent transformComponent,
      SpriteComponent spriteComponent,
      AttackComponent attackComponent,
      DefenseComponent defenseComponent);

  void move(Vector2D& vector2D);
  [[deprecated("Use get_sprite_component()")]] std::string_view get_symbol() {
    return spriteComponent_.symbol;
  };
  [[deprecated("Use get_sprite_component()")]] tcod::ColorRGB get_colour() {
    return spriteComponent_.colour;
  };
  [[deprecated("Use get_transform_component()")]] Vector2D get_position() {
    return transformComponent_.position;
  };
  void set_position(Vector2D position) {
    transformComponent_.position = position;
  };
  TransformComponent& get_transform_component() { return transformComponent_; };
  SpriteComponent& get_sprite_component() { return spriteComponent_; };
  bool is_blocking() { return blocker_; };
  std::string get_name() { return name_; };
  void act(Engine& engine);
  void die();
  bool is_dead() { return defenseComponent_.hp <= 0; };
  bool is_not_dead() { return !is_dead(); };
  AttackComponent& get_attack_component() { return attackComponent_; };
  DefenseComponent& get_defense_component() { return defenseComponent_; };
  void take_damage(int damage) { defenseComponent_.hp -= damage; };

 private:
  std::string name_;
  // PhysicsComponent physicsComponent_;
  bool blocker_;
  TransformComponent transformComponent_;
  SpriteComponent spriteComponent_;
  AttackComponent attackComponent_;
  DefenseComponent defenseComponent_;
};

static const GameEntity PLAYER{
    "player", true, {0, 0}, {"@", RED}, {5}, {2, 30}};
static const GameEntity ORC{"orc", true, {0, 0}, {"o", WHITE}, {3}, {0, 10}};
static const GameEntity TROLL{
    "troll", true, {0, 0}, {"T", WHITE}, {4}, {1, 16}};
}  // namespace cpprl

#endif
