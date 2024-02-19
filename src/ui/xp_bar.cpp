#include "xp_bar.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <libtcod/console.hpp>

#include "colours.hpp"

namespace cpprl {

XPBar::XPBar(int width, int height, Vector2D position, StatsComponent& stats)
    : UiWindow(width, height, position), stats_(stats) {}

void XPBar::render(tcod::Console& console) {
  const auto bar_width =
      (int)((float)stats_.get_xp() / (float)stats_.get_next_level_xp() *
            (float)width_);
  tcod::draw_rect(
      console,
      {position_.x, position_.y, width_, height_},
      0,
      std::nullopt,
      DARK_GREY);
  tcod::draw_rect(
      console,
      {position_.x, position_.y, bar_width, height_},
      0,
      std::nullopt,
      LIGHT_BLUE);

  tcod::print_rect(
      console,
      {position_.x, position_.y, width_, height_},
      fmt::format("XP: {}/{}", stats_.get_xp(), stats_.get_next_level_xp()),
      WHITE,
      std::nullopt,
      TCOD_CENTER);
}
}  // namespace cpprl
