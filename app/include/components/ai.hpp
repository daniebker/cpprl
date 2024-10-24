#pragma once

namespace cpprl {
  enum class AIType {
    HOSTILE,
    CONFUSION,
    NONE
  };

  // AI component
  struct AIComponent {
    AIType type_;
    AIType previous_type_;
  };
}
