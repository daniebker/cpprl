#pragma once

#include <SDL2/SDL.h>

#include <libtcod.hpp>

#include "globals.hpp"
#include "util.hpp"
#include <components/transform.hpp>

namespace cpprl {
  class ASCIIComponent;
  class TransformComponent;
  class Renderer {
    public:
      Renderer() {}
      virtual ~Renderer() {}

      virtual void render(
          ASCIIComponent& sprite, SupaRL::TransformComponent& transform) = 0;
  };

  class TCODRenderer : public Renderer {
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
      ~TCODRenderer() = default;

      virtual void render(ASCIIComponent& sprite, SupaRL::TransformComponent& transform);
  };
}
