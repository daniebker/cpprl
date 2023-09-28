#include "../include/input_handler.hpp"

#include "../include/engine.hpp"
#include "../include/events/directional_command.hpp"
#include "../include/events/quit_command.hpp"

namespace cpprl {

EngineEvent& InputHandler::handle_input(SDL_Keycode key) {
  static QuitCommand quitCommand(engine_);
  static DirectionalCommand buttonRight(engine_, engine_.get_player(), Vector2D{1, 0});
  static DirectionalCommand buttonUpRight(engine_, engine_.get_player(), Vector2D{1, -1});
  static DirectionalCommand buttonLeft(engine_, engine_.get_player(), Vector2D{-1, 0});
  static DirectionalCommand buttonDownLeft(engine_, engine_.get_player(), Vector2D{-1, 1});
  static DirectionalCommand buttonDownRight(engine_, engine_.get_player(), Vector2D{1, 1});
  static DirectionalCommand buttonUpLeft(engine_, engine_.get_player(), Vector2D{-1, -1});
  static DirectionalCommand buttonUp(engine_, engine_.get_player(), Vector2D{0, -1});
  static DirectionalCommand buttonDown(engine_, engine_.get_player(), Vector2D{0, 1});
  static NoOpEvent noop(engine_);

  switch (key) {
    case SDLK_ESCAPE:
      return quitCommand;
      break;
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
      return noop;
      break;
  }
};
}  // namespace cpprl
