#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include <SDL2/SDL.h>

#include "command.hpp"

namespace cpprl {
class InputHandler {
 public:
  InputHandler();
  ~InputHandler();
  Command* handle_input(SDL_Keycode key);

 private:
  Command* buttonRight_;
  Command* buttonLeft_;
  Command* buttonUp_;
  Command* buttonDown_;
  Command* quit_;
};
}  // namespace cpprl
#endif
