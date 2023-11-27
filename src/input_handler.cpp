#include "input_handler.hpp"

#include <memory>

#include "engine.hpp"
#include "events/command.hpp"
#include "gui.hpp"
#include "world.hpp"

namespace cpprl {
EngineEvent* EventHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_ESCAPE:
      return quitCommand_;
      break;
    default:
      return noop_;
      break;
  }
};

EngineEvent* TargetingInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  if (event.type == SDL_MOUSEMOTION) {
    g_context.convert_event_coordinates(event);
    auto mouse_input_event =
        new MouseInputEvent(world_, {event.motion.x, event.motion.y});
    return mouse_input_event;
  } else if (event.type == SDL_MOUSEBUTTONDOWN) {
    g_context.convert_event_coordinates(event);
    auto mouse_click_event =
        new MouseClickEvent(world_, {event.motion.x, event.motion.y});
    return mouse_click_event;
  } else if (event.type == SDL_KEYDOWN) {
    auto mouse_click_event =
        new MouseClickEvent(world_, {event.motion.x, event.motion.y});
    SDL_Keycode key = event.key.keysym.sym;
    switch (key) {
      case SDLK_q:
        return exit_targeting_mode_command_;
        break;
      case SDLK_RETURN:
        return mouse_click_event;
        break;
      default:
        return noop_;
        break;
    }
  }
  return noop_;
};

GameInputHandler::GameInputHandler(World& world, Entity* controllable_entity)
    : EventHandler(world),
      button_right_(
          new DirectionalCommand(world_, controllable_entity, Vector2D{1, 0})),
      button_up_(
          new DirectionalCommand(world_, controllable_entity, Vector2D{0, -1})),
      button_down_(
          new DirectionalCommand(world_, controllable_entity, Vector2D{0, 1})),
      button_up_right_(
          new DirectionalCommand(world_, controllable_entity, Vector2D{1, -1})),
      button_up_left_(new DirectionalCommand(
          world_, controllable_entity, Vector2D{-1, -1})),
      button_left_(
          new DirectionalCommand(world_, controllable_entity, Vector2D{-1, 0})),
      button_down_right_(
          new DirectionalCommand(world_, controllable_entity, Vector2D{1, 1})),
      button_down_left_(
          new DirectionalCommand(world_, controllable_entity, Vector2D{-1, 1})),
      view_history_command_(new ViewHistoryCommand(world_)),
      pick_up_command_(new PickupCommand(world, controllable_entity)),
      inventory_command_(new InventoryCommand(world, controllable_entity)),
      main_menu_command_(new MainMenuCommand(world)),
      controllable_entity_(controllable_entity){};

EngineEvent* GameInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  // TODO: Move this to its own handler.
  //  probably want an event handler which has
  //  input handler for keyboard and another for mouse
  if (event.type == SDL_MOUSEMOTION) {
    g_context.convert_event_coordinates(event);
    world_.get_map().set_highlight_tile({event.motion.x, event.motion.y});
    return noop_;
  }

  SDL_Keycode key = event.key.keysym.sym;

  if (key == SDLK_e) {
    return button_up_right_;
  } else if (key == SDLK_q) {
    return button_up_left_;
  } else if (key == SDLK_z) {
    return button_down_left_;
  } else if (key == SDLK_c) {
    return button_down_right_;
  } else if (key == SDLK_w || key == SDLK_UP) {
    return button_up_;
  } else if (key == SDLK_s || key == SDLK_DOWN) {
    return button_down_;
  } else if (key == SDLK_a || key == SDLK_LEFT) {
    return button_left_;
  } else if (key == SDLK_d || key == SDLK_RIGHT) {
    return button_right_;
  } else if (key == SDLK_v) {
    return view_history_command_;
  } else if (key == SDLK_g) {
    return pick_up_command_;
  } else if (key == SDLK_i) {
    return inventory_command_;
  } else if (key == SDLK_COMMA) {
    return main_menu_command_;
  } else if (key == SDLK_LEFTBRACKET) {
    return new UseCommand(
        world_, controllable_entity_->get_transform_component().get_position());
  } else {
    return EventHandler::handle_sdl_event(event);
  }
};

EngineEvent* MenuInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_RETURN:
      return resetGameCommand_;
      break;
    default:
      return EventHandler::handle_sdl_event(event);
      break;
  }
};

GuiInputHandler::GuiInputHandler(World& world, UiWindow& ui_window)
    : EventHandler(world),
      closeViewCommand_(new CloseViewCommand(world)),
      scrollDownCommand_(new ScrollCommand(world, ui_window, 1)),
      scrollUpCommand_(new ScrollCommand(world, ui_window, -1)),
      jumpUpCommand_(new ScrollCommand(world, ui_window, -10)),
      jumpDownCommand_(new ScrollCommand(world, ui_window, 10)),
      jumpToHome_(new ScrollCommand(world, ui_window, 0)){};

EngineEvent* GuiInputHandler::handle_sdl_event(SDL_Event event) noexcept {
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

EngineEvent* HistoryViewInputHandler::handle_sdl_event(
    SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_v:
      return closeViewCommand_;
      break;
    default:
      return GuiInputHandler::handle_sdl_event(event);
      break;
  }
}

EngineEvent* InventoryInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case (SDLK_RETURN):
      return selectItemCommand_;
      break;
    case (SDLK_d):
      return dropItemCommand_;
      break;
    case SDLK_i:
      return closeViewCommand_;
    default:
      return GuiInputHandler::handle_sdl_event(event);
      break;
  }
}

EngineEvent* MainMenuInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_RETURN:
      return selectMenuItemCommand_;
      break;
    case SDLK_COMMA:
      return closeViewCommand_;
    default:
      return GuiInputHandler::handle_sdl_event(event);
      break;
  }
}
}  // namespace cpprl
