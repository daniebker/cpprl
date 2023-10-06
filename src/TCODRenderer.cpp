#include "rendering.hpp"

namespace cpprl {
void TCODRenderer::render(
    SpriteComponent sprite, TransformComponent transform) {
  tcod::print(
      g_console,
      transform.position,
      sprite.symbol,
      sprite.colour,
      std::nullopt);
}
}  // namespace cpprl
