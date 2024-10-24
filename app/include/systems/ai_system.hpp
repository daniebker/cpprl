#pragma once
#include "world.hpp"

#include <core/system.hpp>

#include <memory>

namespace cpprl {
  class AISystem : public SupaRL::System
  {
    private:
      SupaRL::Entity player_;
      std::shared_ptr<World> world_;

    public:
      // implicit copy constructor when passing reference
      void set_world(std::shared_ptr<World> world) { world_ = world; }
      void set_player(SupaRL::Entity player) { player_ = player; }
      void update();
  };

}
