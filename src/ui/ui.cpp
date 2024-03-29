#include "ui/ui.hpp"

#include <memory>

namespace cpprl {
UI::UI(Dungeon& dungeon) {
  dungeon_level_ =
      std::make_unique<DungeonLevel>(20, 1, Vector2D{2, 35}, dungeon);
}

void UI::set_health_bar(DefenseComponent& defense_component) {
  health_bar_ =
      std::make_unique<HealthBar>(20, 1, Vector2D{2, 36}, defense_component);
}

void UI::set_xp_bar(StatsComponent& stats_component) {
  xp_bar_ = std::make_unique<XPBar>(20, 1, Vector2D{2, 37}, stats_component);
}

void UI::render(tcod::Console& console) {
  health_bar_->render(console);
  dungeon_level_->render(console);
  xp_bar_->render(console);
}
}  // namespace cpprl
