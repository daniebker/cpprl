#include "components.hpp"
#include "rendering.hpp"

namespace cpprl {
  void TCODRenderer::render(
      ASCIIComponent& sprite, SupaRL::TransformComponent& transform) {
    tcod::print(
        g_console,
        transform.position_,
        sprite.get_symbol(),
        sprite.get_colour(),
        std::nullopt);
  }
}
