#pragma once

#include "core/system.hpp"
#include "core/coordinator.hpp"
#include "components/attack.hpp"
#include "components/defence.hpp"
#include "components/status_condition.hpp"
#include "components/identity.hpp"
#include "core/types.hpp"
#include "core/event.hpp"
#include <random>


extern SupaRL::Coordinator g_coordinator;
namespace SupaRL
{
  static auto inline entity_is_dead(int hp) -> bool {
    return hp <= 0;
  }

  static auto inline send_die_event(Entity entity) -> void {
    auto die_event = SupaRL::Event(SupaRL::Events::Combat::Die::DIE);
    die_event.set_param<>(SupaRL::Events::Combat::Die::ENTITY, entity);
    g_coordinator.send_event(die_event);
  }

  static auto inline send_damage_event(Entity attacker, Entity target, int damage) -> void {
    auto damage_event = SupaRL::Event(SupaRL::Events::Combat::Damage::DEALT);
    damage_event.set_param<>(SupaRL::Events::Combat::Damage::DAMAGE, damage);
    damage_event.set_param<>(SupaRL::Events::Combat::Damage::ATTACKER, attacker);
    damage_event.set_param<>(SupaRL::Events::Combat::Damage::DEFENDER, target);
    g_coordinator.send_event(damage_event);
  }

  inline auto attack_event_listener = [](Event& event) -> void {
    Entity& attacker = event.get_param<Entity&>(SupaRL::Events::Combat::ATTACKER);
    Entity& target = event.get_param<Entity&>(SupaRL::Events::Combat::DEFENDER);

    const auto& attacker_attack = g_coordinator
      .get_component<SupaRL::AttackComponent>(attacker);

    auto& target_defence = g_coordinator
      .get_component<SupaRL::DefenceComponent>(target);

    int damage = attacker_attack.damage_ - target_defence.defence_;

    if (damage > 0) {
      target_defence.hp_ -= damage;

      std::random_device dev;
      std::mt19937 rng(dev());
      std::uniform_int_distribution<std::mt19937::result_type> dist_fifty_fifty(0,1); // distribution in range [1, 6]

      if (dist_fifty_fifty(rng) == 1) {
        auto& status_condition = g_coordinator.get_component<SupaRL::StatusConditionComponent>(
            target);

        status_condition.damage_per_tick_ = 1;
        status_condition.max_ticks_ = 10;
        status_condition.ticks_ = 0;
        status_condition.name_ = "Bleeding";
        auto entity_name = g_coordinator.get_component<SupaRL::IdentityComponent>(
            target).name_;

        auto status_event = SupaRL::Event(SupaRL::Events::StatusCondition::APPLY);
        status_event.set_param<>(SupaRL::Events::StatusCondition::NAME, entity_name);
        status_event.set_param<>(SupaRL::Events::StatusCondition::DAMAGE_PER_TICK, status_condition.damage_per_tick_);
        status_event.set_param<>(SupaRL::Events::StatusCondition::CONDITION, "bleeding");
        g_coordinator.send_event(status_event);

      }

      if(entity_is_dead(target_defence.hp_)) {
        send_die_event(target);
      }

    };
  };

  inline auto spell_event_listener = [](Event& event) -> void {
    Entity& attacker = event.get_param<Entity&>(SupaRL::Events::Combat::ATTACKER);
    Entity& target = event.get_param<Entity&>(SupaRL::Events::Combat::DEFENDER);
    int spell_power = event.get_param<int>(SupaRL::Events::Combat::Spell::POWER);

    auto& target_defence = g_coordinator
      .get_component<SupaRL::DefenceComponent>(target);

    int damage = spell_power - target_defence.defence_;

    if (damage > 0) {
      target_defence.hp_ -= damage;
    }

    if(entity_is_dead(target_defence.hp_)) {
      send_die_event(target);
    }

    send_damage_event(attacker, target, damage);
  };


}
