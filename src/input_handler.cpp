#include "../include/input_handler.hpp"

#include "../include/engine.hpp"
#include "../include/events/directional_command.hpp"
#include "../include/events/quit_command.hpp"

namespace cpprl {

EngineEvent& InputHandler::handle_input(SDL_Keycode key) {
  switch (key) {
    case SDLK_ESCAPE:
      return quitCommand;
      break;
    default:
      return noop;
      break;
  }
};

EngineEvent& GameInputHandler::handle_input(SDL_Keycode key) {
  switch (key) {
    case SDLK_e:
      return buttonUpRight;
      break;
    case SDLK_q:
      return buttonUpLeft;
      break;
    case SDLK_z:
      return buttonDownLeft;
      break;
    case SDLK_c:
      return buttonDownRight;
      break;
    case SDLK_w:
    case SDLK_UP:
      return buttonUp;
      break;
    case SDLK_s:
    case SDLK_DOWN:
      return buttonDown;
      break;
    case SDLK_a:
    case SDLK_LEFT:
      return buttonLeft;
      break;
    case SDLK_d:
    case SDLK_RIGHT:
      return buttonRight;
      break;
    default:
      return InputHandler::handle_input(key);
      break;
  }
};

EngineEvent& MenuInputHandler::handle_input(SDL_Keycode key) {
  switch (key) {
    case SDLK_RETURN:
      return resetGameCommand;
      break;
    default:
      return InputHandler::handle_input(key);
      break;
  }
};
}  // namespace cpprl
