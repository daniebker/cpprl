#include "health_bar.hpp"

#include <libtcod/console.hpp>

#include "colours.hpp"

namespace cpprl {

HealthBar::HealthBar(
    int width, int height, Vector2D position, DefenseComponent& defense)
    : UiWindow(width, height, position), health_(defense) {}

void HealthBar::render(tcod::Console& console) const {
  const auto bar_width =
      (int)((float)health_.hp / (float)health_.max_hp * (float)width_);
  tcod::draw_rect(
      console,
      {position_.x, position_.y, width_, height_},
      0,
      std::nullopt,
      DARK_RED);
  tcod::draw_rect(
      console,
      {position_.x, position_.y, bar_width, height_},
      0,
      std::nullopt,
      DARK_GREEN);

  // health bar text
  // std::string_view health_text =
  //     std::format("{}/{}", health_.hp, health_.max_hp);
  tcod::print_rect(
      console,
      {position_.x, position_.y, width_, height_},
      "HP",
      WHITE,
      std::nullopt,
      TCOD_CENTER);
}
}  // namespace cpprl
