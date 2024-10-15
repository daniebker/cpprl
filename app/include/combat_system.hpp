#pragma once

#include <components/status_condition.hpp>
#include <components/identity.hpp>
#include "components.hpp"
#include "game_entity.hpp"
#include "libtcod.hpp"
#include <iostream>

extern SupaRL::Coordinator g_coordinator;

namespace cpprl::combat_system {
  inline auto handle_attack = [](Entity& attacker, Entity& target) -> int {
    int damage = attacker.get_attack_component().get_damage() -
      target.get_defense_component().get_defense();
    if (damage > 0) {
      target.get_defense_component().take_damage(damage);
      auto* rng = TCODRandom::getInstance();
      int rand = rng->getInt(0, 1);
      std::cout << "Rand: " << rand << std::endl;
      if (rand == 1) {
        auto& status_condition = g_coordinator.get_component<SupaRL::StatusConditionComponent>(
            target.get_id());

        status_condition.damage_per_tick_ = 1;
        status_condition.max_ticks_ = 10;
        status_condition.ticks_ = 0;
        status_condition.name_ = "Bleeding";
        auto entity_name = g_coordinator.get_component<SupaRL::IdentityComponent>(
            target.get_id()).name_;
        std::cout << entity_name << " is bleeding profusely!" << std::endl;
      }
      return damage;
    }
    return 0;
  };

  inline auto handle_spell = [](int power, Entity& target) -> int {
    int damage = power - target.get_defense_component().get_defense();
    if (damage > 0) {

      target.get_defense_component().take_damage(damage);

      return damage;
    }
    return 0;
  };
}



