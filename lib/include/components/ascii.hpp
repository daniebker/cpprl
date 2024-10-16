#pragma once

#include <string>

#include "core/colour.hpp"

namespace SupaRL{
  struct AsciiComponent {
      std::string symbol_;
      ColourRGB colour_;
      int layer_;
  };
}
