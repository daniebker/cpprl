#include "input_handler.hpp"

#include "movement_command.hpp"
#include "quit_command.hpp"

namespace cpprl {
// InputHandler::InputHandler(Engine& engine) {
//   quit_ = new QuitCommand();
//   buttonRight_ = new MovementCommand(Vector2D{1, 0});
//   buttonLeft_ = new MovementCommand(Vector2D{-1, 0});
//   buttonUp_ = new MovementCommand(Vector2D{0, -1});
//   buttonDown_ = new MovementCommand(Vector2D{0, 1});
// }

EngineEvent& InputHandler::handle_input(SDL_Keycode key) {
  static QuitCommand quitCommand(engine_);
  static MovementCommand buttonRight(engine_, engine_.get_player(), Vector2D{1, 0});
  static MovementCommand buttonLeft(engine_, engine_.get_player(), Vector2D{-1, 0});
  static MovementCommand buttonUp(engine_, engine_.get_player(), Vector2D{0, -1});
  static MovementCommand buttonDown(engine_, engine_.get_player(), Vector2D{0, 1});
  static NoOpEvent noop(engine_);

  switch (key) {
    case SDLK_ESCAPE:
      return quitCommand;
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
