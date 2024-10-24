#include "components.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <memory>

#include "combat_system.hpp"
#include "entity_manager.hpp"
#include "game_entity.hpp"
#include "state.hpp"
#include "world.hpp"
#include <core/types.hpp>
#include <components/identity.hpp>
#include <components/physique.hpp>
#include <components/defence.hpp>

extern SupaRL::Coordinator g_coordinator;

namespace cpprl {

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
      // Remove the owner?
      return Success{};
    }
    return Failure{"There's nothing to pick up."};
  }

  ActionResult ConsumableComponent::drop(Entity* owner, Entity* wearer) {
    if (auto* container = &wearer->get_container(); container) {
      container->remove(owner);
      // TODO: This would then use the global coordinator to
      // get the transform of each entity and set the positions.
      auto owner_transform = g_coordinator.get_component<SupaRL::TransformComponent>(
          owner->get_id());
      auto wearer_position = g_coordinator.get_component<SupaRL::TransformComponent>(
          wearer->get_id()).position_;
      owner_transform.position_ = wearer_position;
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
    auto& wearer_defence = g_coordinator.get_component<SupaRL::DefenceComponent>(
        wearer->get_id());

    // Shouldn't hit this any more
    /*if (const DefenseComponent* defense_component =*/
    /*    &wearer->get_defense_component();*/
    /*    defense_component == nullptr) {*/
    /*  return Failure{"There's nothing to heal."};*/
    /*}*/

   ;

    if (const int amount_healed = wearer_defence.heal(amount_);
        amount_healed > 0) {
      ConsumableComponent::use(owner, wearer, world);
      auto healed_event = SupaRL::Event(SupaRL::Events::Heal::HEALED);
      healed_event.set_param(SupaRL::Events::Heal::AMOUNT, amount_healed);
      healed_event.set_param(SupaRL::Events::Heal::ENTITY, wearer->get_id());
      g_coordinator.send_event(healed_event);
      /*std::string message = fmt::format("You healed for {} HP.", amount_healed);*/
      /*world.get_message_log().add_message(message, GREEN);*/
      return Success{};
    }

    auto healed_event = SupaRL::Event(SupaRL::Events::Heal::HEALED);
    healed_event.set_param(SupaRL::Events::Heal::AMOUNT, 0);
    healed_event.set_param(SupaRL::Events::Heal::ENTITY, wearer->get_id());
    g_coordinator.send_event(healed_event);

    /*return Failure{"You are already at full health."};*/
    return Failure{};
  }

  ActionResult LightningBolt::use(Entity* owner, Entity* wearer, World& world) {
    auto wearer_position = g_coordinator.get_component<SupaRL::TransformComponent>(
        wearer->get_id()).position_;
    std::optional<std::reference_wrapper<Entity>> optional_closest_monster_ref =
      world.get_entities().get_closest_living_monster(
          wearer_position, range_);
    if (!optional_closest_monster_ref.has_value()) {
      return Failure{"No enemy is close enough to strike."};
    }

    Entity& closest_living_monster = optional_closest_monster_ref.value().get();

    /*int inflicted = combat_system::handle_spell(damage_, closest_living_monster);*/
    combat_system::handle_spell(damage_, closest_living_monster);
    ConsumableComponent::use(owner, wearer, world);
      /*auto& entity_name = g_coordinator.get_component<SupaRL::IdentityComponent>(*/
      /*    closest_living_monster.get_id()).name_;*/
      return Success{};
    /*if (inflicted > 0) {*/
    /*  world.get_message_log().add_message(*/
    /*      fmt::format(*/
    /*        "A lightning bolt strikes the {} with a loud "*/
    /*        "thunder! The damage is {} hit points.",*/
    /*        entity_name,*/
    /*        damage_),*/
    /*      GREEN);*/
    /**/
    /*  // TODO: this should be handled by spell cast*/
    /*  if (closest_living_monster.get_defense_component().is_dead()) {*/
    /*    auto action = DieEvent(world, &closest_living_monster);*/
    /*    action.execute();*/
    /*  }*/
    /*  return Success{};*/
    /*} else {*/
    /*  return Failure{fmt::format(*/
    /*      "The lightning bolt hits the {} but does no damage.",*/
    /*      entity_name)};*/
    /*}*/
  }

  ActionResult FireSpell::use(Entity* owner, Entity* wearer, World& world) {
    auto on_pick = [&, owner, wearer]() {
      ConsumableComponent::use(owner, wearer, world);
      for (Entity* entity : world.get_entities()) {

        const auto entity_position = g_coordinator.get_component<SupaRL::TransformComponent>(
            entity->get_id()).position_;
        const auto entity_name = g_coordinator.get_component<SupaRL::IdentityComponent>(
            entity->get_id()).name_;
        auto& defence_component = g_coordinator.get_component<SupaRL::DefenceComponent>(
            entity->get_id());

        if (defence_component.is_not_dead() &&
            entity_position.distance_to(
              world.get_map().get_highlight_tile()) <= aoe_) {
          // TODO: Send this as a message
          /*world.get_message_log().add_message(*/
          /*    fmt::format(*/
          /*      "The {} gets burned for {} hit points.",*/
          /*      entity_name,*/
          /*      damage_),*/
          /*    RED);*/
          /*int inflicted = combat_system::handle_spell(damage_, *entity);*/
          combat_system::handle_spell(damage_, *entity);
          // Combat system should handle firing the die event on spell damage.
          /*if (inflicted > 0) {*/
          /*  // TODO: this is repeated everywhere. Put it in take_damage*/
          /*  if (entity->get_defense_component().is_dead()) {*/
          /*    auto action = DieEvent(world, entity);*/
          /*    action.execute();*/
          /*  }*/
          /*}*/
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
        auto& entity_name = g_coordinator.get_component<SupaRL::IdentityComponent>(
            target.get_id()).name_;
        std::unique_ptr<AIComponent> old_ai = target.transfer_ai_component();

        std::unique_ptr<AIComponent> confusion_ai =
          std::make_unique<ConfusionAI>(num_turns_, std::move(old_ai));
        target.set_ai_component(std::move(confusion_ai));
        world.get_message_log().add_message(
            fmt::format(
              "The eyes of the {} look vacant, as it starts to "
              "stumble around!",
              entity_name),
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

}
