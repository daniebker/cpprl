#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include <SDL2/SDL.h>

#include <vector>

namespace cpprl {

class Engine;
class EngineEvent;

class InputHandler {
 public:
  InputHandler(Engine& engine) : engine_(engine){};
  EngineEvent& handle_input(SDL_Keycode key);

 private:
  Engine& engine_;
};

}  // namespace cpprl
#endif
