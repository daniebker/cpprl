#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif  // __EMSCRIPTEN__

#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE
#endif

#include <cstdlib>
#include <libtcod.hpp>

#include "engine.hpp"
#include "globals.hpp"
#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the
                                 // compiler handle termination.
#endif

static void main_loop() {
  cpprl::Engine& engine = cpprl::Engine::get_instance();
  engine.render();
  engine.handle_events();
}

static void main_init(int argc, char** argv) {
#ifndef __EMSCRIPTEN__
  std::atexit(SDL_Quit);
#endif

  cpprl::Engine& engine = cpprl::Engine::get_instance();
  engine.init(argc, argv);

#ifdef __EMSCRIPTEN__
  // clang-format off
  EM_ASM(
    FS.mkdir('/saves');
    FS.mount(IDBFS, {}, '/saves');
    FS.syncfs(true, function (err) {
      assert(!err);
      // ccall('after_main_init', null);
    });
  );
// clang-format on
#endif
}

extern "C" const char* EMSCRIPTEN_KEEPALIVE
on_page_unload(int, const void*, void*) {
  cpprl::Engine::get_instance().save();
  return nullptr;
}

/// Main program entry point.
int main(int argc, char** argv) {
  try {
    main_init(argc, argv);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
    emscripten_set_beforeunload_callback(nullptr, on_page_unload);
#else
    while (true) {
      main_loop();
    };
#ifndef __EMSCRIPTEN__
    cpprl::Engine::get_instance().save();
#endif
#endif
  } catch (const std::exception& exc) {
    throw;
  }
}
