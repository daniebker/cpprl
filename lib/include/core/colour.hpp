#pragma once

#include <cstdint>

namespace SupaRL {
  struct ColourRGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };

  static constexpr ColourRGB WHITE = {255, 255, 255};
  static constexpr ColourRGB GREEN = {0, 255, 0};
  static constexpr ColourRGB BLUE = {0, 0, 255};
  static constexpr ColourRGB RED = {255, 0, 0};
}
