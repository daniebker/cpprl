#pragma once
#include "components.hpp"
#include "gui.hpp"
#include <core/math.hpp>

namespace cpprl {
  class XPBar : public UiWindow {
    private:
      StatsComponent& stats_;

    public:
      XPBar(int width, int height, SupaRL::Vector2D position, StatsComponent& stats);
      void render(tcod::Console& console) override;
  };

}
