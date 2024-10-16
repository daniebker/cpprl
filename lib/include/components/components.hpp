#pragma once
#include <string>

namespace SupaRL {

  /**
   * @brief The StatusConditionComponent struct
   * This struct is used to store the status condition of an entity.
   */
  struct StatusConditionComponent {
    short damage_per_turn_;
    short turns_remaining_;
    short turns_until_update_;
    std::string name_;
  };
}
