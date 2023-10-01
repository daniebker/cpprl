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
      Vector2D position,
      std::string_view symbol,
      const tcod::ColorRGB& colour,
      bool blocker,
      AttackComponent attackComponent,
      DefenseComponent defenseComponent);

  void move(Vector2D& vector2D);
  std::string_view get_symbol() { return symbol_; };
  tcod::ColorRGB get_colour() { return colour_; };
  Vector2D get_position() { return position_; };
  void set_position(Vector2D position) { position_ = position; };
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
  Vector2D position_;
  std::string_view symbol_;
  tcod::ColorRGB colour_;
  bool blocker_;
  AttackComponent attackComponent_;
  DefenseComponent defenseComponent_;
};

static const GameEntity PLAYER{"player", {0, 0}, "@", RED, true, {5}, {2, 30}};
static const GameEntity ORC{"orc", {0, 0}, "o", WHITE, true, {3}, {0, 10}};
static const GameEntity TROLL{"troll", {0, 0}, "T", WHITE, true, {4}, {1, 16}};
}  // namespace cpprl

#endif
