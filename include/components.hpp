#ifndef INCLUDE_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_HPP_

#include <libtcod.hpp>
#include <string_view>

#include "types/math.hpp"

namespace cpprl {

struct AttackComponent {
  int damage;
};

struct DefenseComponent {
  DefenseComponent(int defense, int maxHp) : defense(defense), hp(maxHp), max_hp(maxHp) {}
  int defense;
  int hp;
  int max_hp;
};

struct TransformComponent {
  Vector2D position;
};

// Or ASCII component?
struct SpriteComponent {
  std::string_view symbol;
  tcod::ColorRGB colour;
};
}  // namespace cpprl
#endif
