#ifndef INCLUDE_XP_BAR_HPP_
#define INCLUDE_XP_BAR_HPP_

#include "components.hpp"
#include "gui.hpp"

namespace cpprl {

class XPBar : public UiWindow {
 private:
  StatsComponent& stats_;

 public:
  XPBar(int width, int height, Vector2D position, StatsComponent& stats);
  void render(tcod::Console& console) override;
};

}  // namespace cpprl
#endif  // INCLUDE_XP_BAR_HPP_
