#pragma once

#include "core/system.hpp"

namespace SupaRL {
  class PhysicsSystem : public System
  {
    public:
      PhysicsSystem() = default;
      ~PhysicsSystem() = default;

      void init();
      void update();
  };
}
