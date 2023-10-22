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

}  // namespace cpprl
