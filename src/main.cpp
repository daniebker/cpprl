#include "main.hpp"
#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the
                                 // compiler handle termination.
#endif

/// Main program entry point.
int main(int argc, char** argv) {
  try {
    cpprl::Engine engine(argc, argv);
    engine.load();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
#else
    while (true) {
      engine.render();
      engine.handle_events();
    };
    engine.save();
#endif
  } catch (const std::exception& exc) {
    throw;
  }
}
