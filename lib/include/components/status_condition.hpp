#pragma once
#include <string>

namespace SupaRL{
  struct StatusConditionComponent {
    short damage_per_tick_;
    short max_ticks_;
    short ticks_;
    std::string name_;
  };
}
