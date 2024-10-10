#include "health_bar.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <libtcod/console.hpp>

#include "colours.hpp"

namespace cpprl {

  HealthBar::HealthBar(
      int width, int height, SupaRL::Vector2D position, DefenseComponent& defense)
    : UiWindow(width, height, position), health_(defense) {}

  void HealthBar::render(tcod::Console& console) {
    const auto bar_width = (int)((float)health_.get_hp() /
        (float)health_.get_max_hp() * (float)width_);
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

    tcod::print_rect(
        console,
        {position_.x, position_.y, width_, height_},
        fmt::format("HP: {}/{}", health_.get_hp(), health_.get_max_hp()),
        WHITE,
        std::nullopt,
        TCOD_CENTER);
  }
}
