#pragma once
#include "world.hpp"

#include <core/system.hpp>

#include <memory>

namespace cpprl {
  class AISystem : public SupaRL::System
  {
    private:
      SupaRL::Entity player_;

    public:
      AISystem(SupaRL::Entity player) : player_(player) {}
      void set_player(SupaRL::Entity player) { player_ = player; }
      void update();
  };

}
