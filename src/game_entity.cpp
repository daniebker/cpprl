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

Entity::~Entity() {
  if (attackComponent_) delete attackComponent_;
  if (defenseComponent_) delete defenseComponent_;
  if (consumableComponent_) delete consumableComponent_;
  if (aiComponent_) delete aiComponent_;
  if (container_) delete container_;
  if (transformComponent_) delete transformComponent_;
  if (asciiComponent_) delete asciiComponent_;
}

void Entity::update(World& world) { aiComponent_->update(world, this); }

// TODO: not sure this belongs here
float Entity::get_distance_to(Entity* other) {
  return transformComponent_->get_position().distance_to(
      other->get_transform_component()->get_position());
};

void Entity::save(TCODZip& zip) {
  zip.putString(name_.c_str());
  zip.putInt(blocker_);
  zip.putInt(transformComponent_ != NULL);
  zip.putInt(defenseComponent_ != NULL);
  zip.putInt(attackComponent_ != NULL);
  zip.putInt(consumableComponent_ != NULL);
  zip.putInt(asciiComponent_ != NULL);
  zip.putInt(aiComponent_ != NULL);
  zip.putInt(container_ != NULL);
  if (transformComponent_) transformComponent_->save(zip);
  if (asciiComponent_) asciiComponent_->save(zip);
  if (attackComponent_) attackComponent_->save(zip);
  if (defenseComponent_) defenseComponent_->save(zip);
  if (consumableComponent_) consumableComponent_->save(zip);
  if (aiComponent_) aiComponent_->save(zip);
  if (container_) container_->save(zip);
}

void Entity::load(TCODZip& zip) {
  name_ = zip.getString();
  blocker_ = zip.getInt();
  bool hasTransformComponent = zip.getInt();
  bool hasDefenseComponent = zip.getInt();
  bool hasAttackComponent = zip.getInt();
  bool hasConsumableComponent = zip.getInt();
  bool hasAsciiComponent = zip.getInt();
  bool hasAIComponent = zip.getInt();
  bool hasContainer = zip.getInt();
  if (hasTransformComponent) {
    transformComponent_ = new TransformComponent(0, 0);
    transformComponent_->load(zip);
  }
  if (hasAsciiComponent) {
    asciiComponent_ = new ASCIIComponent("", WHITE, 0);
    asciiComponent_->load(zip);
  }
  if (hasAttackComponent) {
    attackComponent_ = new AttackComponent(0);
    attackComponent_->load(zip);
  }
  if (hasDefenseComponent) {
    defenseComponent_ = new DefenseComponent(0, 0);
    defenseComponent_->load(zip);
  }
  if (hasConsumableComponent) {
    consumableComponent_ = ConsumableComponent::create(zip);
  }
  if (hasAIComponent) {
    aiComponent_ = AIComponent::create(zip);
  }
  if (hasContainer) {
    container_ = new Container(0);
    container_->load(zip);
  }
}
}  // namespace cpprl
