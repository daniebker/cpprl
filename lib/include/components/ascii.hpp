#pragma once

#include <string>

namespace SupaRL{
  struct ColorRGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };

  struct AsciiComponent {
      std::string symbol_;
      ColorRGB colour_;
      int layer_;
  };
}
