#include "input_handler.hpp"

#include "engine.hpp"
#include "events/directional_command.hpp"
#include "events/quit_command.hpp"

namespace cpprl {

EngineEvent& EventHandler::handle_sdl_event(SDL_Event event) noexcept {
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

EngineEvent& GameInputHandler::handle_sdl_event(SDL_Event event) noexcept {
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
    case SDLK_g:
      return pickupCommand_;
      break;
    case SDLK_i:
      return inventoryCommand_;
      break;
    default:
      return EventHandler::handle_sdl_event(event);
      break;
  }
};

EngineEvent& MenuInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_RETURN:
      return resetGameCommand;
      break;
    default:
      return EventHandler::handle_sdl_event(event);
      break;
  }
};

EngineEvent& GuiInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_j:
      return scrollDownCommand_;
      break;
    case SDLK_k:
      return scrollUpCommand_;
      break;
    case SDLK_HOME:
      return jumpToHome_;
      break;
    case SDLK_PAGEUP:
      return jumpUpCommand_;
      break;
    case SDLK_PAGEDOWN:
      return jumpDownCommand_;
      break;
    case SDLK_q:
      return closeViewCommand_;
      break;
    default:
      return EventHandler::handle_sdl_event(event);
      break;
  }
}

EngineEvent& HistoryViewInputHandler::handle_sdl_event(
    SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    default:
      return GuiInputHandler::handle_sdl_event(event);
      break;
  }
}

EngineEvent& InventoryInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case (SDLK_RETURN):
      return selectItemCommand_;
      break;
    default:
      return GuiInputHandler::handle_sdl_event(event);
      break;
  }
}

}  // namespace cpprl
