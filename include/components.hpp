#ifndef INCLUDE_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_HPP_

#include <libtcod.hpp>
#include <string_view>
#include <variant>

#include "persistent.hpp"
#include "types/action_result.hpp"
#include "types/entity_fwd.hpp"
#include "types/math.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

class AttackComponent : public Persistent {
 public:
  AttackComponent(int damage) : damage_(damage) {}
  int get_damage() { return damage_; }
  void load(TCODZip& zip);
  void save(TCODZip& zip);

 private:
  int damage_;
};

class DefenseComponent : public Persistent {
 public:
  DefenseComponent(int defense, int maxHp)
      : defense_(defense), hp_(maxHp), max_hp_(maxHp) {}

  int get_hp() { return hp_; }
  int get_max_hp() { return max_hp_; }
  int get_defense() { return defense_; }

  void take_damage(int damage) { hp_ -= damage; }
  int heal(int amount);
  bool is_dead() { return hp_ <= 0; }
  bool is_not_dead() { return !is_dead(); }
  void die(Entity& owner);
  void load(TCODZip& zip);
  void save(TCODZip& zip);

 private:
  int defense_;
  int hp_;
  int max_hp_;
};

class TransformComponent : public Persistent {
 public:
  TransformComponent(int x, int y) : position_({x, y}) {}
  Vector2D get_position() { return position_; }
  void move(Vector2D new_position) { position_ = new_position; }
  void load(TCODZip& zip);
  void save(TCODZip& zip);

 private:
  Vector2D position_;
};

class ASCIIComponent : public Persistent {
 public:
  ASCIIComponent(std::string_view symbol, tcod::ColorRGB colour, int layer)
      : symbol_(symbol), colour_(colour), layer_(layer) {}

  std::string_view get_symbol() { return symbol_; }
  tcod::ColorRGB get_colour() { return colour_; }
  int get_layer() { return layer_; }
  void load(TCODZip& zip);
  void save(TCODZip& zip);

 private:
  std::string symbol_;
  tcod::ColorRGB colour_;
  int layer_;
};

class Container : Persistent {
 private:
  size_t size_;
  std::vector<Entity*> inventory_;

 public:
  Container(int size);
  bool add(Entity* actor);
  void remove(Entity* actor);
  std::vector<Entity*> get_inventory() { return inventory_; }
  int get_size() { return size_; }

  void load(TCODZip& zip);
  void save(TCODZip& zip);
};

class ConsumableComponent : public Persistent {
 public:
  virtual ~ConsumableComponent() = default;
  // TODO: should also be an action result
  ActionResult pick_up(Entity* owner, Entity* wearer);
  ActionResult drop(Entity* owner, Entity* wearer);
  virtual ActionResult use(Entity* owner, Entity* wearer, World& world);
  virtual void load(TCODZip& zip) = 0;
  virtual void save(TCODZip& zip) = 0;
  static std::unique_ptr<ConsumableComponent> create(TCODZip& zip);

 protected:
  enum ConsumableType { HEALER, LIGHTNING_BOLT, CONFUSER, FIREBALL };
};

class HealingConsumable final : public ConsumableComponent {
 public:
  HealingConsumable(int amount);
  ActionResult use(Entity* owner, Entity* wearer, World& world);

  void load(TCODZip& zip);
  void save(TCODZip& zip);

 private:
  int amount_;
};

class LightningBolt final : public ConsumableComponent {
 private:
  float range_, damage_;

 public:
  LightningBolt(float range, float damage) : range_(range), damage_(damage) {}
  ~LightningBolt() = default;
  ActionResult use(Entity* owner, Entity* wearer, World& world);

  void load(TCODZip& zip);
  void save(TCODZip& zip);
};

class FireSpell final : public ConsumableComponent {
 private:
  float max_range_, aoe_, damage_;

 public:
  FireSpell(float max_range, float aoe, float damage)
      : max_range_(max_range), aoe_(aoe), damage_(damage) {}
  ~FireSpell() = default;

  ActionResult use(Entity* owner, Entity* Wearer, World& world);

  void load(TCODZip& zip);
  void save(TCODZip& zip);
};

class ConfusionSpell final : public ConsumableComponent {
 private:
  int num_turns_, max_range_;

 public:
  ConfusionSpell(int num_turns, int max_range)
      : num_turns_(num_turns), max_range_(max_range) {}
  ~ConfusionSpell() = default;

  ActionResult use(Entity* owner, Entity* wearer, World& world);

  void load(TCODZip& zip);
  void save(TCODZip& zip);
};

}  // namespace cpprl
#endif
