#include "input_handler.hpp"

#include "movement_command.hpp"
#include "quit_command.hpp"

namespace cpprl {
InputHandler::InputHandler() {
  quit_ = new QuitCommand();
  buttonRight_ = new MovementCommand(Vector2D{1, 0});
  buttonLeft_ = new MovementCommand(Vector2D{-1, 0});
  buttonUp_ = new MovementCommand(Vector2D{0, -1});
  buttonDown_ = new MovementCommand(Vector2D{0, 1});
}

InputHandler::~InputHandler() {}
Command* InputHandler::handle_input(SDL_Keycode key) {
  switch (key) {
    case SDLK_ESCAPE:
      return quit_;
      break;
    case SDLK_UP:
      return buttonUp_;
      break;
    case SDLK_DOWN:
      return buttonDown_;
      break;
    case SDLK_LEFT:
      return buttonLeft_;
      break;
    case SDLK_RIGHT:
      return buttonRight_;
      break;
  }
  return nullptr;
};
}  // namespace cpprl
