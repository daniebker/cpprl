#include "game_entity.hpp"

#include <libtcod.hpp>
#include <string_view>

#include "basic_ai_component.hpp"
#include "components.hpp"
#include "world.hpp"

namespace cpprl {

Entity::Entity(
    std::string name,
    bool blocker,
    std::unique_ptr<TransformComponent> transformComponent,
    std::unique_ptr<ASCIIComponent> asciiComponent)
    : name_(name),
      blocker_(blocker),
      transformComponent_(std::move(transformComponent)),
      asciiComponent_(std::move(asciiComponent)),
      attackComponent_(nullptr),
      defenseComponent_(nullptr),
      consumableComponent_(nullptr),
      aiComponent_(nullptr),
      container_(nullptr) {}

void Entity::update(World& world) { aiComponent_->update(world, this); }

// TODO: not sure this belongs here
float Entity::get_distance_to(Entity* other) {
  return transformComponent_->get_position().distance_to(
      other->get_transform_component().get_position());
};

void Entity::set_ascii_component(
    std::unique_ptr<ASCIIComponent> asciiComponent) {
  asciiComponent_ = std::move(asciiComponent);
};

void Entity::set_attack_component(
    std::unique_ptr<AttackComponent> attackComponent) {
  attackComponent_ = std::move(attackComponent);
};

void Entity::set_defense_component(
    std::unique_ptr<DefenseComponent> defenseComponent) {
  defenseComponent_ = std::move(defenseComponent);
};

void Entity::set_consumable_component(
    std::unique_ptr<ConsumableComponent> consumableComponent) {
  consumableComponent_ = std::move(consumableComponent);
};

void Entity::set_ai_component(std::unique_ptr<AIComponent> aiComponent) {
  aiComponent_ = std::move(aiComponent);
};

void Entity::set_container(std::unique_ptr<Container> container) {
  container_ = std::move(container);
};

std::unique_ptr<AIComponent> Entity::transfer_ai_component() {
  return std::move(aiComponent_);
};

}  // namespace cpprl
