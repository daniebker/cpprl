#include "rendering.hpp"
#include <components/ascii.hpp>
#include <libtcod.hpp>

namespace cpprl {
  void TCODRenderer::render(
      SupaRL::AsciiComponent& sprite, SupaRL::TransformComponent& transform) {

    tcod::print(
        g_console,
        transform.position_,
        sprite.symbol_,
        tcod::ColorRGB{
          sprite.colour_.r,
          sprite.colour_.g,
          sprite.colour_.b
        },
        std::nullopt);
  }
}
