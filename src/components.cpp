#include "components.hpp"

#include <fmt/core.h>

#include <algorithm>

#include "combat_system.hpp"
#include "events/command.hpp"
#include "exceptions.hpp"
#include "game_entity.hpp"
#include "globals.hpp"
#include "main.hpp"
#include "state.hpp"
#include "world.hpp"

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

ActionResult ConsumableComponent::use(Entity* owner, Entity* wearer, World&) {
  if (wearer->get_container()) {
    wearer->get_container()->remove(owner);
    return Success{};
  }
  return Failure{""};
}

HealingConsumable::HealingConsumable(int amount) : amount_(amount){};

ActionResult HealingConsumable::use(
    Entity* owner, Entity* wearer, World& world) {
  if (!wearer->get_defense_component()) {
    return Failure{"There's nothing to heal."};
  }

  int amount_healed = wearer->get_defense_component()->heal(amount_);
  if (amount_healed > 0) {
    ConsumableComponent::use(owner, wearer, world);
    std::string message = fmt::format("You healed for {} HP.", amount_healed);
    world.get_message_log().add_message(message, GREEN);
    return Success{};
  }

  return Failure{"You are already at full health."};
}

ActionResult LightningBolt::use(Entity* owner, Entity* wearer, World& world) {
  Entity* closest_monster = nullptr;
  closest_monster = world.get_entities().get_closest_monster(
      wearer->get_transform_component()->get_position(), range_);
  if (!closest_monster) {
    return Failure{"No enemy is close enough to strike."};
  }
  // closest_monster->get_defense_component()->take_damage(damage_);
  int inflicted = combat_system::handle_spell(damage_, *closest_monster);
  ConsumableComponent::use(owner, wearer, world);
  if (inflicted > 0) {
    world.get_message_log().add_message(
        fmt::format(
            "A lightning bolt strikes the {} with a loud "
            "thunder! The damage is {} hit points.",
            closest_monster->get_name(),
            damage_),
        GREEN);

    if (closest_monster->get_defense_component()->is_dead()) {
      auto action = DieEvent(world, closest_monster);
      action.execute();
    }
    return Success{};
  } else {
    return Failure{fmt::format(
        "The lightning bolt hits the {} but does no damage.",
        closest_monster->get_name())};
  }
}

ActionResult FireSpell::use(Entity* owner, Entity* wearer, World& world) {
  auto on_pick = [&, owner, wearer]() {
    // TODO:: when I get here the pointers are garbage.
    ConsumableComponent::use(owner, wearer, world);
    for (Entity* entity : world.get_entities()) {
      if (entity->get_defense_component() &&
          entity->get_defense_component()->is_not_dead() &&
          entity->get_transform_component()->get_position().distance_to(
              world.get_map().get_highlight_tile()) <= max_range_) {
        world.get_message_log().add_message(
            fmt::format(
                "The {} gets burned for {} hit points.",
                entity->get_name(),
                damage_),
            RED);
        int inflicted = combat_system::handle_spell(damage_, *entity);
        if (inflicted > 0) {
          // TODO: this is repeated everywhere. Put it in take_damage
          if (entity->get_defense_component()->is_dead()) {
            auto action = DieEvent(world, entity);
            action.execute();
          }
        }
      }
    }
  };
  // world.set_targeting_tile(max_range_, on_pick);
  return Poll{std::make_unique<PickTileAOEState>(world, on_pick, max_range_)};
}
}  // namespace cpprl
