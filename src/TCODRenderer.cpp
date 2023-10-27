#include "components.hpp"
#include "rendering.hpp"

namespace cpprl {
void TCODRenderer::render(
    ASCIIComponent& sprite, TransformComponent& transform) {
  tcod::print(
      g_console,
      transform.get_position(),
      sprite.get_symbol(),
      sprite.get_colour(),
      std::nullopt);
}
}  // namespace cpprl
