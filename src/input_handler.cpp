#include "input_handler.hpp"

#include "engine.hpp"
#include "events/directional_command.hpp"
#include "events/quit_command.hpp"

namespace cpprl {

EngineEvent& InputHandler::handle_input(SDL_Event event) {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_ESCAPE:
      return quitCommand;
      break;
    default:
      return noop;
      break;
  }
};

EngineEvent& GameInputHandler::handle_input(SDL_Event event) {
  // TODO: Move this to its own handler.
  //  probably want an event handler which has
  //  input handler for keyboard and another for mouse
  if (event.type == SDL_MOUSEMOTION) {
    g_context.convert_event_coordinates(event);
    engine_.get_controller().cursor = {event.motion.x, event.motion.y};
    engine_.get_map()->set_target_tile({event.motion.x, event.motion.y});
    return noop;
  }

  SDL_Keycode key = event.key.keysym.sym;

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
    case SDLK_v:
      return viewHistoryCommand;
      break;
    default:
      return InputHandler::handle_input(event);
      break;
  }
};

EngineEvent& MenuInputHandler::handle_input(SDL_Event event) {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_RETURN:
      return resetGameCommand;
      break;
    default:
      return InputHandler::handle_input(event);
      break;
  }
};

EngineEvent& HistoryViewInputHandler::handle_input(SDL_Event event) {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_q:
      return closeViewCommand_;
      break;
    default:
      return InputHandler::handle_input(event);
      break;
  }
}
}  // namespace cpprl
