#pragma once

#include "health_bar.hpp"
#include "ui/dungeon_level.hpp"
#include <components/defence.hpp>
#include "xp_bar.hpp"

namespace cpprl {
class UI {
 private:
  std::unique_ptr<DungeonLevel> dungeon_level_;
  std::unique_ptr<HealthBar> health_bar_;
  std::unique_ptr<XPBar> xp_bar_;

 public:
  explicit UI(Dungeon& dungeon);

  void set_health_bar(SupaRL::DefenceComponent& defense_component);
  void set_xp_bar(StatsComponent& stats_component);

  void render(tcod::Console& console);
};
}  // namespace cpprl
