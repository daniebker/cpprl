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
  ASCIIComponent* asciiComponent_;
  AttackComponent* attackComponent_;
  DefenseComponent* defenseComponent_;
  ConsumableComponent* consumableComponent_;
  AIComponent* aiComponent_;
  Container* container_;

 public:
  Entity(
      std::string name,
      bool blocker,
      std::unique_ptr<TransformComponent> transformComponent,
      ASCIIComponent* asciiComponent);

  ~Entity();

  TransformComponent& get_transform_component() {
    return *transformComponent_;
  };
  ASCIIComponent* get_sprite_component() { return asciiComponent_; };
  AttackComponent* get_attack_component() { return attackComponent_; };
  DefenseComponent* get_defense_component() { return defenseComponent_; };
  ConsumableComponent* get_consumable_component() {
    return consumableComponent_;
  };
  AIComponent* get_ai_component() { return aiComponent_; };
  Container* get_container() { return container_; };

  float get_distance_to(Entity* other);

  bool is_blocking() { return blocker_; };
  std::string get_name() { return name_; };

  void update(World& world);
  void set_blocking(bool blocker) { blocker_ = blocker; };
  void set_name(std::string name) { name_ = name; };
  void set_ascii_component(ASCIIComponent* asciiComponent) {
    asciiComponent_ = asciiComponent;
  };
  void set_defense_component(DefenseComponent* defenseComponent) {
    defenseComponent_ = defenseComponent;
  };
  void set_attack_component(AttackComponent* attackComponent) {
    attackComponent_ = attackComponent;
  };
  void set_consumable_component(ConsumableComponent* consumableComponent) {
    consumableComponent_ = consumableComponent;
  };
  void set_ai_component(AIComponent* aiComponent) {
    aiComponent_ = aiComponent;
  };
  void set_container(Container* container) { container_ = container; };
  void save(TCODZip& zip);
  void load(TCODZip& zip);
};

}  // namespace cpprl

#endif
