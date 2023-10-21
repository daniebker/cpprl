#ifndef INCLUDE_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_HPP_

#include <libtcod.hpp>
#include <string_view>

#include "state.hpp"
#include "types/math.hpp"

namespace cpprl {
class Entity;
class Engine;
struct Success {};

struct Failure {
  std::string message;
};

struct Poll {
  std::unique_ptr<State> new_state;
};

using ActionResult = std::variant<Success, Failure, Poll>;

class AttackComponent {
 public:
  AttackComponent(int damage) : damage_(damage) {}
  int get_damage() { return damage_; }

 private:
  int damage_;
};

class DefenseComponent {
 public:
  DefenseComponent(int defense, int maxHp)
      : defense(defense), hp_(maxHp), max_hp_(maxHp) {}

  int get_hp() { return hp_; }
  int get_max_hp() { return max_hp_; }
  int get_defense() { return defense; }

  void take_damage(int damage) { hp_ -= damage; }
  int heal(int amount);
  bool is_dead() { return hp_ <= 0; }
  bool is_not_dead() { return !is_dead(); }
  void die(Entity& owner);

 private:
  int defense;
  int hp_;
  int max_hp_;
};

class TransformComponent {
 public:
  TransformComponent(int x, int y) : position_({x, y}) {}
  Vector2D get_position() { return position_; }
  void move(Vector2D new_position) { position_ = new_position; }

 private:
  Vector2D position_;
};

class ASCIIComponent {
 public:
  ASCIIComponent(std::string_view symbol, tcod::ColorRGB colour, int layer)
      : symbol_(symbol), colour_(colour), layer_(layer) {}

  std::string_view get_symbol() { return symbol_; }
  tcod::ColorRGB get_colour() { return colour_; }
  int get_layer() { return layer_; }

 private:
  std::string_view symbol_;
  tcod::ColorRGB colour_;
  int layer_;
};

class Container {
 private:
  size_t size_;
  std::vector<Entity*> inventory_;

 public:
  Container(int size);
  bool add(Entity* actor);
  void remove(Entity* actor);
  std::vector<Entity*> get_inventory() { return inventory_; }
  int get_size() { return size_; }
};

class ConsumableComponent {
 public:
  virtual ~ConsumableComponent() = default;
  bool pick_up(Entity* owner, Entity* wearer);
  virtual ActionResult use(Entity* owner, Entity* wearer, Engine& engine);
};

class HealingConsumable final : public ConsumableComponent {
 public:
  HealingConsumable(int amount);
  ActionResult use(Entity* owner, Entity* wearer, Engine& engine);

 private:
  int amount_;
};

class LightningBolt final : public ConsumableComponent {
 private:
  float range_, damage_;

 public:
  LightningBolt(float range, float damage) : range_(range), damage_(damage) {}
  ~LightningBolt() = default;
  ActionResult use(Entity* owner, Entity* wearer, Engine& engine);
};

class FireSpell final : public ConsumableComponent {
 private:
  float max_range_, damage_;

 public:
  FireSpell(float max_range, float damage)
      : max_range_(max_range), damage_(damage) {}
  ~FireSpell() = default;

  ActionResult use(Entity* owner, Entity* Wearer, Engine& engine);
};
}  // namespace cpprl
#endif
