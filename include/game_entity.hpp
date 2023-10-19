#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <libtcod.hpp>
#include <string_view>

#include "basic_ai_component.hpp"
#include "colours.hpp"
#include "components.hpp"
#include "types/math.hpp"

namespace cpprl {
class Engine;
class Entity {
 protected:
  std::string name_;
  // PhysicsComponent physicsComponent_;
  bool blocker_;
  TransformComponent* transformComponent_;
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
      TransformComponent* transformComponent,
      ASCIIComponent* asciiComponent)
      : name_(name),
        blocker_(blocker),
        transformComponent_(std::move(transformComponent)),
        asciiComponent_(std::move(asciiComponent)),
        attackComponent_(nullptr),
        defenseComponent_(nullptr),
        consumableComponent_(nullptr),
        aiComponent_(nullptr),
        container_(nullptr) {}

  ~Entity() {
    if (attackComponent_) delete attackComponent_;
    if (defenseComponent_) delete defenseComponent_;
    if (consumableComponent_) delete consumableComponent_;
    if (aiComponent_) delete aiComponent_;
    if (container_) delete container_;
    if (transformComponent_) delete transformComponent_;
    if (asciiComponent_) delete asciiComponent_;
  };

  TransformComponent* get_transform_component() { return transformComponent_; };
  ASCIIComponent* get_sprite_component() { return asciiComponent_; };
  AttackComponent* get_attack_component() { return attackComponent_; };
  DefenseComponent* get_defense_component() { return defenseComponent_; };
  ConsumableComponent* get_consumable_component() {
    return consumableComponent_;
  };
   AIComponent* get_ai_component() { return aiComponent_; };
   Container* get_container() { return container_; };

   // TODO: not sure this belongs here
   float get_distance_to(Entity* other) {
     return transformComponent_->get_position().distance_to(
         other->get_transform_component()->get_position());
   };

   bool is_blocking() { return blocker_; };
   std::string get_name() { return name_; };

   void update(Engine& engine);
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
};

}  // namespace cpprl

#endif
