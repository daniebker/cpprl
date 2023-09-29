#ifndef INCLUDE_COMBAT_SYSTEM_HPP_
#define INCLUDE_COMBAT_SYSTEM_HPP_

#include "game_entity.hpp"

namespace cpprl::combat_system {
auto handle_attack = [](GameEntity& attacker, GameEntity& target) -> int {
  int damage = attacker.get_attack_component().damage - target.get_defense_component().defense;
  if (damage > 0) {
    target.take_damage(damage);
    return damage;
  }
  return 0;
};
}

#endif  // INCLUDE_COMBAT_SYSTEM_HPP_
