#pragma once

#include "gui.hpp"
#include <core/math.hpp>

#include <components/defence.hpp>

namespace cpprl {

class HealthBar : public UiWindow {
 private:
   SupaRL::DefenceComponent& health_;

 public:
  HealthBar(
      int width, int height, SupaRL::Vector2D position, SupaRL::DefenceComponent& defense);

  void render(tcod::Console& console) override;
};

}
