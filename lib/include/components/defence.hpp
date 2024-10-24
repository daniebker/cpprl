#pragma once

namespace SupaRL{
  struct DefenceComponent {
    short defence_;
    short hp_;
    short max_hp_;

    bool is_dead() { return hp_ <= 0; }
    bool is_not_dead() { return !is_dead(); }
    int heal(short amount) {
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
  };
}
