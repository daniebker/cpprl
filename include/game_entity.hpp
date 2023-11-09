#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <libtcod.hpp>
#include <string_view>

#include "persistent.hpp"
#include "types/math.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

class TransformComponent;
class ASCIIComponent;
class AttackComponent;
class DefenseComponent;
class ConsumableComponent;
class AIComponent;
class Container;

class Entity {
 protected:
  std::string name_;
  // PhysicsComponent physicsComponent_;
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
      std::string name,
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
  AIComponent& get_ai_component() { return *aiComponent_; };
  std::unique_ptr<AIComponent> transfer_ai_component() {
    return std::move(aiComponent_);
  };
  Container& get_container() { return *container_; };

  float get_distance_to(Entity* other);

  bool is_blocking() { return blocker_; };
  std::string get_name() { return name_; };

  void update(World& world);
  void set_blocking(bool blocker) { blocker_ = blocker; };
  void set_name(std::string name) { name_ = name; };
  void set_ascii_component(std::unique_ptr<ASCIIComponent> asciiComponent);
  void set_defense_component(
      std::unique_ptr<DefenseComponent> defenseComponent);
  void set_attack_component(std::unique_ptr<AttackComponent> attackComponent);
  void set_consumable_component(
      std::unique_ptr<ConsumableComponent> consumableComponent);
  void set_ai_component(std::unique_ptr<AIComponent> aiComponent);
  void set_container(std::unique_ptr<Container> container);
  void save(TCODZip& zip);
  void load(TCODZip& zip);
};

}  // namespace cpprl

#endif
