#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <SDL2/SDL.h>

#include <libtcod.hpp>

#include "components.hpp"
#include "globals.hpp"
#include "util.hpp"

namespace cpprl {
class Renderer {
 public:
  Renderer() {}

  virtual void render(SpriteComponent sprite, TransformComponent transform) = 0;
};

class TCODRenderer final : public Renderer {
 public:
  TCODRenderer(int argc, char** argv) : Renderer() {
    g_console = tcod::Console{80, 40};
    auto params = TCOD_ContextParams{};
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.argc = argc;
    params.argv = argv;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.vsync = 1;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.window_title = "rogue_like";
    auto tileset = tcod::load_tilesheet(
        cpprl::util::get_data_dir() / "dejavu16x16_gs_tc.png",
        {32, 8},
        tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();
    params.console = g_console.get();
    g_context = tcod::Context{params};
  };

  virtual void render(SpriteComponent sprite, TransformComponent transform);
};
}  // namespace cpprl
#endif
