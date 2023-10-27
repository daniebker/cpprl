#ifndef INCLUDE_COMBAT_SYSTEM_HPP_
#define INCLUDE_COMBAT_SYSTEM_HPP_

#include "components.hpp"
#include "game_entity.hpp"

namespace cpprl::combat_system {
inline auto handle_attack = [](Entity& attacker, Entity& target) -> int {
  int damage = attacker.get_attack_component()->get_damage() -
               target.get_defense_component()->get_defense();
  if (damage > 0) {
    target.get_defense_component()->take_damage(damage);
    return damage;
  }
  return 0;
};

inline auto handle_spell = [](int power, Entity& target) -> int {
  int damage = power - target.get_defense_component()->get_defense();
  if (damage > 0) {
    target.get_defense_component()->take_damage(damage);
    return damage;
  }
  return 0;
};
}

#endif  // INCLUDE_COMBAT_SYSTEM_HPP_
