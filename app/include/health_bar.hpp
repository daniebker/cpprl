#ifndef INCLUDE_HEALTH_BAR_HPP_
#define INCLUDE_HEALTH_BAR_HPP_

#include "components.hpp"
#include "gui.hpp"

namespace cpprl {

class HealthBar : public UiWindow {
 private:
  DefenseComponent& health_;

 public:
  HealthBar(
      int width, int height, Vector2D position, DefenseComponent& defense);

  void render(tcod::Console& console) override;
};

}  // namespace cpprl
#endif
