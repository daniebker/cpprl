#pragma once

#include <components/status_condition.hpp>
#include <components/identity.hpp>
#include "components.hpp"
#include <core/types.hpp>
#include <core/event.hpp>
#include "game_entity.hpp"
#include "libtcod.hpp"

extern SupaRL::Coordinator g_coordinator;

namespace cpprl::combat_system {
  inline auto handle_attack = [](Entity& attacker, Entity& target) -> int {
    auto attack_event = SupaRL::Event(SupaRL::Events::Combat::ATTACK);
    attack_event.set_param<>(SupaRL::Events::Combat::Attack::ATTACKER, attacker.get_id());
    attack_event.set_param<>(SupaRL::Events::Combat::Attack::DEFENDER, target.get_id());
    g_coordinator.send_event(attack_event);
  };

  inline auto handle_spell = [](int power, Entity& target) -> int {
    auto spell_event = SupaRL::Event(SupaRL::Events::Combat::SPELL);
    spell_event.set_param<>(SupaRL::Events::Combat::Spell::TARGET, target.get_id());
    spell_event.set_param<>(SupaRL::Events::Combat::Spell::POWER, power);
    g_coordinator.send_event(spell_event);

    int damage = power - target.get_defense_component().get_defense();
    if (damage > 0) {

      target.get_defense_component().take_damage(damage);

      return damage;
    }
    return 0;
  };
}



