#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <libtcod.hpp>
#include <string_view>

#include "basic_ai_component.hpp"
#include "types/math.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

class TransformComponent;
class ASCIIComponent;
class AttackComponent;
class DefenseComponent;
class ConsumableComponent;
class Container;

class Entity {
 private:
  std::string name_;
  bool blocker_;
  std::unique_ptr<TransformComponent> transformComponent_;
  std::unique_ptr<ASCIIComponent> asciiComponent_;
  std::unique_ptr<AttackComponent> attackComponent_;
  std::unique_ptr<DefenseComponent> defenseComponent_;
  std::unique_ptr<ConsumableComponent> consumableComponent_;
  std::unique_ptr<AIComponent> aiComponent_;
  std::unique_ptr<Container> container_;

 public:
  Entity(
      std::string const& name,
      bool blocker,
      std::unique_ptr<TransformComponent> transformComponent,
      std::unique_ptr<ASCIIComponent> asciiComponent);

  ~Entity() = default;

  TransformComponent& get_transform_component() {
    return *transformComponent_;
  };
  ASCIIComponent& get_sprite_component() { return *asciiComponent_; };
  AttackComponent& get_attack_component() { return *attackComponent_; };
  DefenseComponent& get_defense_component() { return *defenseComponent_; };
  ConsumableComponent& get_consumable_component() {
    return *consumableComponent_;
  };
  std::optional<std::reference_wrapper<AIComponent>> get_ai_component() {
    if (aiComponent_) {
      return std::ref(*aiComponent_);
    } else {
      return std::nullopt;
    }
  };
  std::unique_ptr<AIComponent> transfer_ai_component();
  Container& get_container() { return *container_; };
  float get_distance_to(Entity* other) const;

  bool is_blocking() const { return blocker_; };
  std::string get_name() const { return name_; };

  void update(World& world);
  void set_blocking(bool blocker) { blocker_ = blocker; };
  void set_name(std::string_view name) { name_ = name; };
  void set_ascii_component(std::unique_ptr<ASCIIComponent> asciiComponent);
  void set_defense_component(
      std::unique_ptr<DefenseComponent> defenseComponent);
  void set_attack_component(std::unique_ptr<AttackComponent> attackComponent);
  void set_consumable_component(
      std::unique_ptr<ConsumableComponent> consumableComponent);
  void set_ai_component(std::unique_ptr<AIComponent> aiComponent);
  void set_container(std::unique_ptr<Container> container);

  template <class Archive>
  void pack(Archive& archive) {
    archive(name_, blocker_);
    archive(transformComponent_ != nullptr);
    archive(defenseComponent_ != nullptr);
    archive(attackComponent_ != nullptr);
    archive(consumableComponent_ != nullptr);
    archive(asciiComponent_ != nullptr);
    archive(aiComponent_ != nullptr);
    archive(container_ != nullptr);
    if (transformComponent_) archive(transformComponent_);

    if (asciiComponent_) archive(asciiComponent_);
    if (attackComponent_) archive(attackComponent_);
    if (defenseComponent_) archive(defenseComponent_);
    if (consumableComponent_) archive(consumableComponent_);
    if (aiComponent_) archive(aiComponent_);
    if (container_) archive(container_);
  }

  template <class Archive>
  void unpack(Archive& archive) {
    bool hasTransformComponent;
    bool hasDefenseComponent;
    bool hasAttackComponent;
    bool hasConsumableComponent;
    bool hasAsciiComponent;
    bool hasAIComponent;
    bool hasContainer;

    archive(name_, blocker_);
    archive(hasTransformComponent);
    archive(hasDefenseComponent);
    archive(hasAttackComponent);
    archive(hasConsumableComponent);
    archive(hasAsciiComponent);
    archive(hasAIComponent);
    archive(hasContainer);

    if (hasTransformComponent) {
      archive(transformComponent_);
    }
    if (hasAsciiComponent) {
      archive(asciiComponent_);
    }
    if (hasAttackComponent) {
      archive(attackComponent_);
    }
    if (hasDefenseComponent) {
      archive(defenseComponent_);
    }
    if (hasConsumableComponent) {
      archive(consumableComponent_);
    }
    if (hasAIComponent) {
      archive(aiComponent_);
    }
    if (hasContainer) {
      archive(container_);
    }
  }
};

}  // namespace cpprl

#endif
