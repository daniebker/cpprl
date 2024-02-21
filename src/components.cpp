#include "components.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <memory>

#include "basic_ai_component.hpp"
#include "combat_system.hpp"
#include "entity_manager.hpp"
#include "events/command.hpp"
#include "exceptions.hpp"
#include "game_entity.hpp"
#include "globals.hpp"
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

void DefenseComponent::die(Entity& the_deceased) const {
  the_deceased.set_name("corpse of " + the_deceased.get_name());
  the_deceased.set_ascii_component(std::make_unique<ASCIIComponent>("%", RED, -1));
  the_deceased.set_blocking(false);
  the_deceased.set_ai_component(nullptr);
}

Container::Container(int size) : size_(size), inventory_({}) {
  inventory_.reserve(size);
}

bool Container::add(Entity* entity) {
  if (size_ > 0 && inventory_.size() >= size_) {
    return false;
  }
  inventory_.push_back(std::move(entity));
  return true;
}

void Container::remove(const Entity* entityToRemove) {
  inventory_.erase(
      std::begin(std::ranges::remove_if(
          inventory_,
          [&entityToRemove](const Entity* entity) {
            return entity == entityToRemove;
          })),
      inventory_.end());
}

ActionResult ConsumableComponent::pick_up(Entity* owner, Entity* wearer) {
  if (auto* container = &wearer->get_container();
      container && container->add(std::move(owner))) {
    // remove the owner?
    return Success{};
  }
  return Failure{"There's nothing to pick up."};
}

ActionResult ConsumableComponent::drop(Entity* owner, Entity* wearer) {
  if (auto* container = &wearer->get_container(); container) {
    container->remove(owner);
    owner->get_transform_component().move(
        wearer->get_transform_component().get_position());
    return Success{};
  }
  return Failure{};
}

ActionResult ConsumableComponent::use(Entity* owner, Entity* wearer, World&) {
  if (auto* container = &wearer->get_container(); container) {
    container->remove(owner);
    return Success{};
  }
  return Failure{""};
}

HealingConsumable::HealingConsumable(int amount) : amount_(amount){};

ActionResult HealingConsumable::use(
    Entity* owner, Entity* wearer, World& world) {
  if (const DefenseComponent* defense_component =
          &wearer->get_defense_component();
      defense_component == nullptr) {
    return Failure{"There's nothing to heal."};
  }

  if (const int amount_healed = wearer->get_defense_component().heal(amount_);
      amount_healed > 0) {
    ConsumableComponent::use(owner, wearer, world);
    std::string message = fmt::format("You healed for {} HP.", amount_healed);
    world.get_message_log().add_message(message, GREEN);
    return Success{};
  }

  return Failure{"You are already at full health."};
}

ActionResult LightningBolt::use(Entity* owner, Entity* wearer, World& world) {
  std::optional<std::reference_wrapper<Entity>> optional_closest_monster_ref =
      world.get_entities().get_closest_living_monster(
          wearer->get_transform_component().get_position(), range_);
  if (!optional_closest_monster_ref.has_value()) {
    return Failure{"No enemy is close enough to strike."};
  }

  Entity& closest_living_monster = optional_closest_monster_ref.value().get();

  int inflicted = combat_system::handle_spell(damage_, closest_living_monster);
  ConsumableComponent::use(owner, wearer, world);
  if (inflicted > 0) {
    world.get_message_log().add_message(
        fmt::format(
            "A lightning bolt strikes the {} with a loud "
            "thunder! The damage is {} hit points.",
            closest_living_monster.get_name(),
            damage_),
        GREEN);

    if (closest_living_monster.get_defense_component().is_dead()) {
      auto action = DieEvent(world, &closest_living_monster);
      action.execute();
    }
    return Success{};
  } else {
    return Failure{fmt::format(
        "The lightning bolt hits the {} but does no damage.",
        closest_living_monster.get_name())};
  }
}

ActionResult FireSpell::use(Entity* owner, Entity* wearer, World& world) {
  auto on_pick = [&, owner, wearer]() {
    ConsumableComponent::use(owner, wearer, world);
    for (Entity* entity : world.get_entities()) {
      if (const auto* defense_component = &entity->get_defense_component();
          defense_component && defense_component->is_not_dead() &&
          entity->get_transform_component().get_position().distance_to(
              world.get_map().get_highlight_tile()) <= aoe_) {
        world.get_message_log().add_message(
            fmt::format(
                "The {} gets burned for {} hit points.",
                entity->get_name(),
                damage_),
            RED);
        int inflicted = combat_system::handle_spell(damage_, *entity);
        if (inflicted > 0) {
          // TODO: this is repeated everywhere. Put it in take_damage
          if (entity->get_defense_component().is_dead()) {
            auto action = DieEvent(world, entity);
            action.execute();
          }
        }
      }
    }
  };
  return Poll{
      std::make_unique<PickTileAOEState>(world, on_pick, max_range_, aoe_)};
}

ActionResult ConfusionSpell::use(Entity* owner, Entity* wearer, World& world) {
  auto on_pick = [&, owner, wearer]() -> StateResult {
    std::optional<std::reference_wrapper<Entity>> optional_ref_target =
        world.get_entities().get_blocking_entity_at(
            world.get_map().get_highlight_tile());
    if (optional_ref_target.has_value()) {
      auto& target = optional_ref_target.value().get();
      std::unique_ptr<AIComponent> old_ai = target.transfer_ai_component();

      std::unique_ptr<AIComponent> confusion_ai =
          std::make_unique<ConfusionAI>(num_turns_, std::move(old_ai));
      target.set_ai_component(std::move(confusion_ai));
      world.get_message_log().add_message(
          fmt::format(
              "The eyes of the {} look vacant, as it starts to "
              "stumble around!",
              target.get_name()),
          GREEN);
      ConsumableComponent::use(owner, wearer, world);
      return {};

    } else {
      return NoOp{"There is no enemy at that location."};
    }
  };
  return Poll{std::make_unique<PickTileState>(world, on_pick, max_range_)};
}

void StatsComponent::add_xp(int xp) {
  stats_data_.xp_ += xp;
  if (stats_data_.xp_ >= this->get_next_level_xp()) {
    level_up();
  }
}

void StatsComponent::level_up() {
  stats_data_.level_++;
  stats_data_.xp_ = 0;

  stats_data_.level_up_base_ =
      static_cast<int>(stats_data_.level_up_base_ * 1.5);
  stats_data_.stats_points_ += 2;
}

}  // namespace cpprl
