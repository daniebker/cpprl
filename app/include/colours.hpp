#pragma once

#include <libtcod.hpp>

namespace cpprl {
  static constexpr auto WHITE = tcod::ColorRGB{200, 200, 200};
  static constexpr auto RED = tcod::ColorRGB{255, 0, 0};
  static constexpr auto DARK_RED = tcod::ColorRGB{191, 0, 0};
  static constexpr auto BLACK = tcod::ColorRGB{40, 42, 54};
  static constexpr auto BLACK_DARK = tcod::ColorRGB{18, 19, 18};
  static constexpr auto GREY = tcod::ColorRGB{128, 128, 128};
  static constexpr auto GREEN = tcod::ColorRGB{0, 255, 0};
  static constexpr auto DARK_GREEN = tcod::ColorRGB{0, 191, 0};
  static constexpr auto BLUE = tcod::ColorRGB{0, 0, 255};
  static constexpr auto DARK_BLUE = tcod::ColorRGB{0, 0, 191};
  static constexpr auto TEAL = tcod::ColorRGB{0, 128, 128};
  static constexpr auto DARK_GREY = tcod::ColorRGB{64, 64, 64};
  static constexpr auto LIGHT_BLUE = tcod::ColorRGB{0, 128, 255};

  static constexpr auto BAR_TEXT = WHITE;
  static constexpr auto INVALID = tcod::ColorRGB{0xFF, 0xFF, 0x00};
  static constexpr auto IMPOSSIBLE = tcod::ColorRGB(0x80, 0x80, 0x80);
  static constexpr auto ERROR = tcod::ColorRGB(0xFF, 0x40, 0x40);
  static constexpr auto HEALTH_RECOVERED = tcod::ColorRGB(0x0, 0xFF, 0x0);

}
