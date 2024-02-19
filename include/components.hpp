#ifndef INCLUDE_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_HPP_

#include <cereal/types/polymorphic.hpp>
#include <libtcod.hpp>
#include <string_view>
#include <variant>

#include "game_entity.hpp"
#include "types/action_result.hpp"
#include "types/math.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

class AttackComponent {
 public:
  AttackComponent() = default;
  AttackComponent(int damage) : damage_(damage) {}
  virtual ~AttackComponent() = default;
  int get_damage() const { return damage_; }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(damage_);
  }

 private:
  int damage_;
};

class DefenseComponent {
 public:
  DefenseComponent() = default;
  DefenseComponent(int defense, int maxHp)
      : defense_(defense), hp_(maxHp), max_hp_(maxHp) {}
  virtual ~DefenseComponent() = default;

  int get_hp() const { return hp_; }
  int get_max_hp() const { return max_hp_; }
  int get_defense() const { return defense_; }

  void take_damage(int damage) { hp_ -= damage; }
  int heal(int amount);
  bool is_dead() const { return hp_ <= 0; }
  bool is_not_dead() const { return !is_dead(); }
  void die(Entity* owner);

  template <class Archive>
  void serialize(Archive& archive) {
    archive(defense_, hp_, max_hp_);
  }

 private:
  int defense_;
  int hp_;
  int max_hp_;
};

class TransformComponent {
 public:
  TransformComponent() = default;
  TransformComponent(int x, int y) : position_({x, y}) {}
  virtual ~TransformComponent() = default;
  Vector2D get_position() const { return position_; }
  void move(Vector2D new_position) { position_ = new_position; }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(position_);
  }

 private:
  Vector2D position_;
};

class ASCIIComponent {
 public:
  ASCIIComponent() = default;
  ASCIIComponent(std::string_view symbol, tcod::ColorRGB colour, int layer)
      : symbol_(symbol), colour_(colour), layer_(layer) {}
  virtual ~ASCIIComponent() = default;

  std::string_view get_symbol() const { return symbol_; }
  tcod::ColorRGB get_colour() const { return colour_; }
  int get_layer() const { return layer_; }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(symbol_, colour_, layer_);
  }

 private:
  std::string symbol_;
  tcod::ColorRGB colour_;
  int layer_;
};

class Container {
 private:
  size_t size_;
  std::vector<Entity*> inventory_;

 public:
  Container() = default;
  Container(int size);
  virtual ~Container() = default;
  bool add(Entity* actor);
  void remove(const Entity* actor);
  std::vector<Entity*> get_inventory() const { return inventory_; }
  size_t get_size() const { return size_; }

  template <class Archive>
  void save(Archive& archive) const {
    archive(size_);
    archive(inventory_.size());
    for (auto& item : inventory_) {
      item->pack(archive);
    }
  }
  template <class Archive>
  void load(Archive& archive) {
    archive(size_);
    int nb_items;
    archive(nb_items);
    for (int i = 0; i < nb_items; i++) {
      Entity* entity = new Entity("", false, nullptr, nullptr);
      entity->unpack(archive);
      inventory_.emplace_back(entity);
    }
  }
};

class ConsumableComponent {
 public:
  ConsumableComponent() = default;
  virtual ~ConsumableComponent() = default;
  // TODO: should also be an action result
  ActionResult pick_up(Entity* owner, Entity* wearer);
  ActionResult drop(Entity* owner, Entity* wearer);
  virtual ActionResult use(Entity* owner, Entity* wearer, World& world);

  template <class Archive>
  void serialize(Archive&) {}

 protected:
  enum ConsumableType { HEALER, LIGHTNING_BOLT, CONFUSER, FIREBALL };
};

class HealingConsumable final : public ConsumableComponent {
 public:
  HealingConsumable() = default;
  HealingConsumable(int amount);
  virtual ~HealingConsumable() = default;
  ActionResult use(Entity* owner, Entity* wearer, World& world) override;
  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<ConsumableComponent>(this), amount_);
  }

 private:
  int amount_;
};

class LightningBolt final : public ConsumableComponent {
 private:
  float range_;
  float damage_;

 public:
  LightningBolt() = default;
  LightningBolt(float range, float damage) : range_(range), damage_(damage) {}
  virtual ~LightningBolt() = default;
  ActionResult use(Entity* owner, Entity* wearer, World& world) override;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<ConsumableComponent>(this), range_, damage_);
  }
};

class FireSpell final : public ConsumableComponent {
 private:
  float max_range_;
  float aoe_;
  float damage_;

 public:
  FireSpell() = default;
  FireSpell(float max_range, float aoe, float damage)
      : max_range_(max_range), aoe_(aoe), damage_(damage) {}
  virtual ~FireSpell() = default;

  ActionResult use(Entity* owner, Entity* Wearer, World& world) override;
  template <class Archive>
  void serialize(Archive& archive) {
    archive(
        cereal::base_class<ConsumableComponent>(this),
        max_range_,
        aoe_,
        damage_);
  }
};

class ConfusionSpell final : public ConsumableComponent {
 private:
  int num_turns_;
  int max_range_;

 public:
  ConfusionSpell() = default;
  ConfusionSpell(int num_turns, int max_range)
      : num_turns_(num_turns), max_range_(max_range) {}
  virtual ~ConfusionSpell() = default;

  ActionResult use(Entity* owner, Entity* wearer, World& world) override;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(
        cereal::base_class<ConsumableComponent>(this), num_turns_, max_range_);
  }
};

}  // namespace cpprl

CEREAL_REGISTER_TYPE(cpprl::HealingConsumable);
CEREAL_REGISTER_TYPE(cpprl::LightningBolt);
CEREAL_REGISTER_TYPE(cpprl::FireSpell);
CEREAL_REGISTER_TYPE(cpprl::ConfusionSpell);

#endif
