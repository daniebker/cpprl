#pragma once

#include "core/system.hpp"

namespace SupaRL
{
  class CombatSystem : public System
  {
  public:
    CombatSystem() = default;
    ~CombatSystem() = default;

    void init();
    void update();
  };
}
