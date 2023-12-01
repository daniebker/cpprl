#pragma once

#include "dungeon.hpp"
#include "gui.hpp"

namespace cpprl {
class DungeonLevel final : public UiWindow {
 private:
  Dungeon& dungeon_;

 public:
  DungeonLevel(int width, int height, Vector2D position, Dungeon& dungeon)
      : UiWindow(width, height, position), dungeon_(dungeon) {}

  void render(tcod::Console& console) override;
};
}  // namespace cpprl
