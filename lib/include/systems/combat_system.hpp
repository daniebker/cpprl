#pragma once

#include "core/system.hpp"
#include "core/coordinator.hpp"
#include "core/types.hpp"
#include "core/event.hpp"

extern SupaRL::Coordinator g_coordinator;
namespace SupaRL
{
  inline auto attack_event_listener = [](Event& event) -> void {
    Entity& attacker  event.get_param<Entity>(SupaRL::Events::Combat::Attack::ATTACKER);
    Entity& target = event.get_param<Entity>(SupaRL::Events::Combat::Attack::DEFENDER);
    int damage = attacker.get_attack_component().get_damage() -
      target.get_defense_component().get_defense();
    if (damage > 0) {
      target.get_defense_component().take_damage(damage);
      auto* rng = TCODRandom::getInstance();
      int rand = rng->getInt(0, 1);
      if (rand == 1) {
        auto& status_condition = g_coordinator.get_component<SupaRL::StatusConditionComponent>(
            target.get_id());

        status_condition.damage_per_tick_ = 1;
        status_condition.max_ticks_ = 10;
        status_condition.ticks_ = 0;
        status_condition.name_ = "Bleeding";
        auto entity_name = g_coordinator.get_component<SupaRL::IdentityComponent>(
            target.get_id()).name_;
      }
      // TODO: Fire combat resolved event with damage dealt
    }
    // TODO: Fire combat resolved event with damage dealt (0)
  };
}
