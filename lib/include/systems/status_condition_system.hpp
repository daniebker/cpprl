#pragma once

#include "core/system.hpp"

namespace SupaRL
{
  class StatusConditionSystem : public System
  {
  public:
    StatusConditionSystem() = default;
    ~StatusConditionSystem() = default;

    void init();
    void update();
  };
}
