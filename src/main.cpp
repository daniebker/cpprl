#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__
#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>
#include <list>

#include "dungeon.hpp"
#include "engine.hpp"
#include "game_entity.hpp"
#include "input_handler.hpp"
#include "types/map.hpp"

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

/// Return the data directory.
auto get_data_dir() -> std::filesystem::path {
  static auto root_directory = std::filesystem::path{"."};  // Begin at the working directory.
  while (!std::filesystem::exists(root_directory / "data")) {
    // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
    root_directory /= "..";
    if (!std::filesystem::exists(root_directory)) {
      throw std::runtime_error("Could not find the data directory.");
    }
  }
  return root_directory / "data";
};

static tcod::Console g_console;  // The global console object.
static tcod::Context g_context;  // The global libtcod context.

static cpprl::InputHandler* inputHandler = new cpprl::InputHandler();
static cpprl::Dungeon dungeon = cpprl::Dungeon();
static cpprl::EntityManager entityManager = cpprl::EntityManager();
static cpprl::Engine engine(entityManager, dungeon, inputHandler);

void main_loop() {
  engine.render(g_console);
  g_context.present(g_console);
  SDL_Event event;
  engine.handle_events(event);
}

/// Main program entry point.
int main(int argc, char** argv) {
  try {
    auto params = TCOD_ContextParams{};
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.argc = argc;
    params.argv = argv;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.vsync = 1;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.window_title = "rogue_like";

    auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();
    g_console = tcod::Console{80, 40};
    params.console = g_console.get();

    g_context = tcod::Context(params);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
#else
    while (true) main_loop();
    delete inputHandler;
#endif
  } catch (const std::exception& exc) {
    std::cerr << exc.what() << "\n";
    throw;
  }
}
