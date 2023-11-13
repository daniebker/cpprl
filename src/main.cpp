#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif  // __EMSCRIPTEN__

#include <cstdlib>
#include <libtcod.hpp>

#include "engine.hpp"
#include "globals.hpp"
#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the
                                 // compiler handle termination.
#endif

static void main_loop(void* arg) {
  cpprl::Engine* engine = static_cast<cpprl::Engine*>(arg);
  engine->render();
  engine->handle_events();
}

/// Main program entry point.
int main(int argc, char** argv) {
  try {
    cpprl::Engine* engine = new cpprl::Engine(argc, argv);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, engine, 0, 0);
#else
    while (true) {
      main_loop(engine);
    };
    engine->save();
    delete engine;
#endif
  } catch (const std::exception& exc) {
    throw;
  }
}
