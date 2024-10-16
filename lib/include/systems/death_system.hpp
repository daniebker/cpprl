#pragma once

#include "core/system.hpp"

namespace SupaRL {

  class DeathSystem : public System
  {
    public:
      DeathSystem() = default;
      ~DeathSystem() = default;

      void update();
  };
}
