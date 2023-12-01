#pragma once

#include "health_bar.hpp"
#include "ui/dungeon_level.hpp"

namespace cpprl {
class UI {
 private:
  std::unique_ptr<DungeonLevel> dungeon_level_;
  std::unique_ptr<HealthBar> health_bar_;

 public:
  UI(Dungeon& dungeon);

  void set_health_bar(DefenseComponent& defense_component);

  void render(tcod::Console& console);
};
}  // namespace cpprl
