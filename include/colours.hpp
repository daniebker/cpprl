
#pragma once

#include <libtcod.hpp>

namespace cpprl {
static constexpr auto WHITE = tcod::ColorRGB{200, 200, 200};
static constexpr auto RED = tcod::ColorRGB{255, 0, 0};
static constexpr auto DARK_RED = tcod::ColorRGB{191, 0, 0};
static constexpr auto BLACK = tcod::ColorRGB{0, 0, 0};
static constexpr auto GREY = tcod::ColorRGB{128, 128, 128};
static constexpr auto GREEN = tcod::ColorRGB{0, 255, 0};
static constexpr auto DARK_GREEN = tcod::ColorRGB{0, 191, 0};
static constexpr auto BLUE = tcod::ColorRGB{0, 0, 255};
static constexpr auto DARK_BLUE = tcod::ColorRGB{0, 0, 191};

static constexpr auto BAR_TEXT = WHITE;
}  // namespace cpprl
