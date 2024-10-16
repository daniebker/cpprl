#pragma once

namespace SupaRL {
  struct ColourRGB {
    int r;
    int g;
    int b;
  };

  static constexpr ColourRGB WHITE = {255, 255, 255};
  static constexpr ColourRGB GREEN = {0, 255, 0};
  static constexpr ColourRGB BLUE = {0, 0, 255};
  static constexpr ColourRGB RED = {255, 0, 0};
}
