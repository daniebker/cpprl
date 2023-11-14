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

// void Entity::save(TCODZip& zip) {
//   zip.putString(name_.c_str());
//   zip.putInt(blocker_);
//   zip.putInt(transformComponent_ != NULL);
//   zip.putInt(defenseComponent_ != NULL);
//   zip.putInt(attackComponent_ != NULL);
//   zip.putInt(consumableComponent_ != NULL);
//   zip.putInt(asciiComponent_ != NULL);
//   zip.putInt(aiComponent_ != NULL);
//   zip.putInt(container_ != NULL);
//   if (transformComponent_) transformComponent_->save(zip);
//   if (asciiComponent_) asciiComponent_->save(zip);
//   if (attackComponent_) attackComponent_->save(zip);
//   if (defenseComponent_) defenseComponent_->save(zip);
//   if (consumableComponent_) consumableComponent_->save(zip);
//   if (aiComponent_) aiComponent_->save(zip);
//   if (container_) container_->save(zip);
// }
void Entity::save(cereal::JSONOutputArchive& archive) {
  archive(name_, blocker_);
  archive(transformComponent_ != nullptr);
  archive(defenseComponent_ != nullptr);
  archive(attackComponent_ != nullptr);
  archive(consumableComponent_ != nullptr);
  archive(asciiComponent_ != nullptr);
  archive(aiComponent_ != nullptr);
  archive(container_ != nullptr);
  if (transformComponent_) transformComponent_->save(archive);
  if (asciiComponent_) asciiComponent_->save(archive);
  if (attackComponent_) attackComponent_->save(archive);
  if (defenseComponent_) defenseComponent_->save(archive);
  if (consumableComponent_) consumableComponent_->save(archive);
  if (aiComponent_) aiComponent_->save(archive);
  if (container_) container_->save(archive);
}

void Entity::load(cereal::JSONInputArchive& archive) {
  archive(name_, blocker_);
  bool hasTransformComponent, hasDefenseComponent, hasAttackComponent,
      hasConsumableComponent, hasAsciiComponent, hasAIComponent, hasContainer;
  archive(hasTransformComponent);
  archive(hasDefenseComponent);
  archive(hasAttackComponent);
  archive(hasConsumableComponent);
  archive(hasAsciiComponent);
  archive(hasAIComponent);
  archive(hasContainer);
  if (hasTransformComponent) {
    transformComponent_ = std::make_unique<TransformComponent>(0, 0);
    transformComponent_->load(archive);
  }
  if (hasAsciiComponent) {
    asciiComponent_ = std::make_unique<ASCIIComponent>("", WHITE, 0);
    asciiComponent_->load(archive);
  }
  if (hasAttackComponent) {
    attackComponent_ = std::make_unique<AttackComponent>(0);
    attackComponent_->load(archive);
  }
  if (hasDefenseComponent) {
    defenseComponent_ = std::make_unique<DefenseComponent>(0, 0);
    defenseComponent_->load(archive);
  }
  if (hasConsumableComponent) {
    consumableComponent_ = ConsumableComponent::create(archive);
  }
  if (hasAIComponent) {
    aiComponent_ = AIComponent::create(archive);
  }
  if (hasContainer) {
    container_ = std::make_unique<Container>(0);
    container_->load(archive);
  }
}

// void Entity::load(TCODZip& zip) {
//   name_ = zip.getString();
//   blocker_ = zip.getInt();
//   bool hasTransformComponent = zip.getInt();
//   bool hasDefenseComponent = zip.getInt();
//   bool hasAttackComponent = zip.getInt();
//   bool hasConsumableComponent = zip.getInt();
//   bool hasAsciiComponent = zip.getInt();
//   bool hasAIComponent = zip.getInt();
//   bool hasContainer = zip.getInt();
//   if (hasTransformComponent) {
//     transformComponent_ = std::make_unique<TransformComponent>(0, 0);
//     transformComponent_->load(zip);
//   }
//   if (hasAsciiComponent) {
//     asciiComponent_ = std::make_unique<ASCIIComponent>("", WHITE, 0);
//     asciiComponent_->load(zip);
//   }
//   if (hasAttackComponent) {
//     attackComponent_ = std::make_unique<AttackComponent>(0);
//     attackComponent_->load(zip);
//   }
//   if (hasDefenseComponent) {
//     defenseComponent_ = std::make_unique<DefenseComponent>(0, 0);
//     defenseComponent_->load(zip);
//   }
//   if (hasConsumableComponent) {
//     consumableComponent_ = ConsumableComponent::create(zip);
//   }
//   if (hasAIComponent) {
//     aiComponent_ = AIComponent::create(zip);
//   }
//   if (hasContainer) {
//     container_ = std::make_unique<Container>(0);
//     container_->load(zip);
//   }
// }

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
