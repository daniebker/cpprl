#include "components.hpp"

#include <fmt/core.h>

#include <algorithm>

#include "exceptions.hpp"
#include "game_entity.hpp"
#include "globals.hpp"
#include "main.hpp"

namespace cpprl {
int DefenseComponent::heal(int amount) {
  if (hp_ == max_hp_) {
    return 0;
  };
  int new_hp = hp_ + amount;
  if (new_hp > max_hp_) {
    new_hp = max_hp_;
  }

  int healed = new_hp - hp_;

  hp_ = new_hp;

  return healed;
}

void DefenseComponent::die(Entity& owner) {
  owner.set_name("corpse of " + owner.get_name());
  owner.set_ascii_component(new ASCIIComponent("%", RED, -1));
  owner.set_blocking(false);
  owner.set_ai_component(nullptr);
}

Container::Container(int size) : size_(size), inventory_({}) {
  inventory_.reserve(size);
}

bool Container::add(Entity* entity) {
  if (size_ > 0 && inventory_.size() >= size_) {
    return false;
  }
  inventory_.push_back(entity);
  return true;
}

void Container::remove(Entity* entityToRemove) {
  inventory_.erase(
      std::remove_if(
          inventory_.begin(),
          inventory_.end(),
          [&entityToRemove](const Entity* entity) {
            return entity == entityToRemove;
          }),
      inventory_.end());
}

bool ConsumableComponent::pick_up(Entity* owner, Entity* wearer) {
  if (wearer->get_container() && wearer->get_container()->add(owner)) {
    // remove the owner?
    return true;
  }
  return false;
}

ActionResult ConsumableComponent::use(Entity* owner, Entity* wearer, Engine&) {
  if (wearer->get_container()) {
    wearer->get_container()->remove(owner);
    return {true, ""};
  }
  return {false, ""};
}

HealingConsumable::HealingConsumable(int amount) : amount_(amount){};

ActionResult HealingConsumable::use(
    Entity* owner, Entity* wearer, Engine& engine) {
  if (!wearer->get_defense_component()) {
    return {false, "There's nothing to heal."};
  }

  int amount_healed = wearer->get_defense_component()->heal(amount_);
  if (amount_healed > 0) {
    ActionResult result = ConsumableComponent::use(owner, wearer, engine);
    result.message = fmt::format("You healed for {} HP.", amount_healed);
    return result;
  }

  return {false, "You are already at full health."};
}

ActionResult LightningBolt::use(Entity* owner, Entity* wearer, Engine& engine) {
  Entity* closest_monster = nullptr;
  closest_monster = engine.get_entities().get_closest_monster(
      wearer->get_transform_component()->get_position(), range_);
  if (!closest_monster) {
    return {false, "No enemy is close enough to strike."};
  }
  closest_monster->get_defense_component()->take_damage(damage_);
  ConsumableComponent::use(owner, wearer, engine);
  return {
      true,
      fmt::format(
          "A lightning bolt strikes the {} with a loud "
          "thunder! The damage is {} hit points.",
          closest_monster->get_name(),
          damage_)};
}

}  // namespace cpprl
