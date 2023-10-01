#ifndef INCLUDE_HEALTH_BAR_HPP_
#define INCLUDE_HEALTH_BAR_HPP_

#include "components.hpp"
#include "ui_window.hpp"

namespace cpprl {

class HealthBar : public UiWindow {
 private:
  const DefenseComponent& health_;

 public:
  HealthBar(
      int width, int height, Vector2D position, DefenseComponent& defense);

  void render(tcod::Console& console) const override;
};

}  // namespace cpprl
#endif
