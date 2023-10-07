#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__
#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>
#include <list>

#include "../include/dungeon.hpp"
#include "../include/engine.hpp"
#include "../include/game_entity.hpp"
#include "../include/input_handler.hpp"
#include "../include/types/map.hpp"

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

/// Main program entry point.
int main(int argc, char** argv) {
  try {
    static cpprl::Engine engine(argc, argv);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
#else
    while (true) {
      engine.render();
      engine.handle_events();
    };
#endif
  } catch (const std::exception& exc) {
    throw;
  }
}
