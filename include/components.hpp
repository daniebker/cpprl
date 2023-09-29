#ifndef INCLUDE_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_HPP_

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
}  // namespace cpprl
#endif
