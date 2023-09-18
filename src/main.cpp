#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__
#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>
#include <list>

#include "engine.hpp"
#include "game_entity.hpp"
#include "input_handler.hpp"

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

static constexpr auto WHITE = tcod::ColorRGB{255, 255, 255};
static constexpr auto RED = tcod::ColorRGB{255, 0, 0};

static tcod::Console g_console;  // The global console object.
static tcod::Context g_context;  // The global libtcod context.

static cpprl::GameEntity player(cpprl::Vector2D{0, 0}, "@", RED);
std::list<cpprl::GameEntity*> entities = {&player};
static cpprl::InputHandler inputHandler;
static cpprl::Engine engine(entities, inputHandler);

/// Game loop.
void main_loop() {
  // Rendering.
  // g_console.clear();

  // tcod::print(g_console, {player.get_x(), player.get_y()}, player.get_symbol(), player.get_colour(), std::nullopt);
  engine.render(g_console);
  g_context.present(g_console);

  // Handle input.
  SDL_Event event;
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      cpprl::Command* command = inputHandler.handle_input(event.key.keysym.sym);
      command->execute(player);
    } else if (event.type == SDL_QUIT) {
      std::exit(EXIT_SUCCESS);
    }
  }
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
#endif
  } catch (const std::exception& exc) {
    std::cerr << exc.what() << "\n";
    throw;
  }
}
