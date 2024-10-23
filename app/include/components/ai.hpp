#pragma once

namespace cpprl {
  enum class AIType {
    HOSTILE,
    CONFUSION
  };

  // AI component
  struct AIComponent {
    AIType type_;
    AIType prevuious_type_;
  };
}
