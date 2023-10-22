#include "input_handler.hpp"

#include "engine.hpp"
#include "events/command.hpp"
#include "world.hpp"

namespace cpprl {

EngineEvent& EventHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_ESCAPE:
      return quitCommand_;
      break;
    default:
      return noop;
      break;
  }
};

EngineEvent& TargetingInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  if (event.type == SDL_MOUSEMOTION) {
    g_context.convert_event_coordinates(event);
    auto mouse_input_event =
        MouseInputEvent{world_, {event.motion.x, event.motion.y}};
    return mouse_input_event;
  }

  else if (event.type == SDL_MOUSEBUTTONDOWN) {
    g_context.convert_event_coordinates(event);
    auto mouse_click_event =
        MouseClickEvent{world_, {event.motion.x, event.motion.y}};
    return mouse_click_event;
  } else if (event.type == SDL_KEYDOWN) {
    SDL_Keycode key = event.key.keysym.sym;
    switch (key) {
      case SDLK_q:
        return exit_targeting_mode_command_;
        break;
      default:
        return noop;
        break;
    }
  }
  return noop;
};

GameInputHandler::GameInputHandler(World& world, Entity* controllable_entity)
    : EventHandler(world),
      buttonRight(world_, controllable_entity, Vector2D{1, 0}),
      buttonUp(world_, controllable_entity, Vector2D{0, -1}),
      buttonDown(world_, controllable_entity, Vector2D{0, 1}),
      buttonUpRight(world_, controllable_entity, Vector2D{1, -1}),
      buttonUpLeft(world_, controllable_entity, Vector2D{-1, -1}),
      buttonLeft(world_, controllable_entity, Vector2D{-1, 0}),
      buttonDownRight(world_, controllable_entity, Vector2D{1, 1}),
      buttonDownLeft(world_, controllable_entity, Vector2D{-1, 1}),
      viewHistoryCommand(world_),
      pickupCommand_(world, controllable_entity),
      inventoryCommand_(world, controllable_entity){};

EngineEvent& GameInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  // TODO: Move this to its own handler.
  //  probably want an event handler which has
  //  input handler for keyboard and another for mouse
  if (event.type == SDL_MOUSEMOTION) {
    g_context.convert_event_coordinates(event);
    world_.get_map().set_highlight_tile({event.motion.x, event.motion.y});
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

GuiInputHandler::GuiInputHandler(World& world)
    : EventHandler(world),
      closeViewCommand_(world),
      scrollDownCommand_(world, 1),
      scrollUpCommand_(world, -1),
      jumpUpCommand_(world, -10),
      jumpDownCommand_(world, 10),
      jumpToHome_(world, 0){};

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
