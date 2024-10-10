#pragma once

#include "components.hpp"
#include "gui.hpp"
#include <core/math.hpp>

namespace cpprl {

class HealthBar : public UiWindow {
 private:
  DefenseComponent& health_;

 public:
  HealthBar(
      int width, int height, SupaRL::Vector2D position, DefenseComponent& defense);

  void render(tcod::Console& console) override;
};

}
