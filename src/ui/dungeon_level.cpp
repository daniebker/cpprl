#include "ui/dungeon_level.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <libtcod/console.hpp>

#include "colours.hpp"

namespace cpprl {
void DungeonLevel::render(tcod::Console& console) {
  tcod::print_rect(
      console,
      {position_.x, position_.y, width_, height_},
      fmt::format("Level: {}", dungeon_.get_level()),
      WHITE,
      std::nullopt,
      TCOD_CENTER);
}
}  // namespace cpprl
