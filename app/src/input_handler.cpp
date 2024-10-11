#include "input_handler.hpp"

#include <memory>
#include <core/math.hpp>

#include "engine.hpp"
#include "events/command.hpp"
#include "gui.hpp"
#include "world.hpp"

extern SupaRL::Coordinator g_coordinator;

namespace cpprl {
EngineEvent& EventHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  if (key == SDLK_ESCAPE) {
    return *quitCommand_;
  } else {
    return *noop_;
  }
};

EngineEvent& TargetingInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  if (event.type == SDL_MOUSEMOTION) {
    g_context.convert_event_coordinates(event);
    mouse_input_event_ = std::make_unique<MouseInputEvent>(
        world_, SupaRL::Vector2D{event.motion.x, event.motion.y});
    return *mouse_input_event_;
  } else if (event.type == SDL_MOUSEBUTTONDOWN) {
    g_context.convert_event_coordinates(event);
    mouse_click_event_ = std::make_unique<MouseClickEvent>(
        world_, SupaRL::Vector2D{event.motion.x, event.motion.y});
    return *mouse_click_event_;
  } else if (event.type == SDL_KEYDOWN) {
    mouse_click_event_ = std::make_unique<MouseClickEvent>(
        world_, SupaRL::Vector2D{event.motion.x, event.motion.y});

    SDL_Keycode key = event.key.keysym.sym;

    switch (key) {
      case SDLK_q:
        return *exit_targeting_mode_command_;
        break;
      case SDLK_RETURN:
        return *mouse_click_event_;
        break;
      default:
        return *noop_;
        break;
    }
  }
  return *noop_;
};

GameInputHandler::GameInputHandler(World& world, Entity* controllable_entity)
    : EventHandler(world),
      button_right_(std::make_unique<DirectionalCommand>(
          world_, controllable_entity, SupaRL::Vector2D{1, 0})),
      button_up_(std::make_unique<DirectionalCommand>(
          world_, controllable_entity, SupaRL::Vector2D{0, -1})),
      button_down_(std::make_unique<DirectionalCommand>(
          world_, controllable_entity, SupaRL::Vector2D{0, 1})),
      button_up_right_(std::make_unique<DirectionalCommand>(
          world_, controllable_entity, SupaRL::Vector2D{1, -1})),
      button_up_left_(std::make_unique<DirectionalCommand>(
          world_, controllable_entity, SupaRL::Vector2D{-1, -1})),
      button_left_(std::make_unique<DirectionalCommand>(
          world_, controllable_entity, SupaRL::Vector2D{-1, 0})),
      button_down_right_(std::make_unique<DirectionalCommand>(
          world_, controllable_entity, SupaRL::Vector2D{1, 1})),
      button_down_left_(std::make_unique<DirectionalCommand>(
          world_, controllable_entity, SupaRL::Vector2D{-1, 1})),
      view_history_command_(std::make_unique<ViewHistoryCommand>(world_)),
      pick_up_command_(
          std::make_unique<PickupCommand>(world, controllable_entity)),
      inventory_command_(
          std::make_unique<InventoryCommand>(world, controllable_entity)),
      main_menu_command_(std::make_unique<MainMenuCommand>(world)),
      use_command_(nullptr),
      character_menu_command_(
          std::make_unique<CharacterMenuCommand>(world, controllable_entity)),
      controllable_entity_(controllable_entity){};

EngineEvent& GameInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  // TODO: Move this to its own handler.
  //  probably want an event handler which has
  //  input handler for keyboard and another for mouse
  if (event.type == SDL_MOUSEMOTION) {
    g_context.convert_event_coordinates(event);
    world_.get_map().set_highlight_tile({event.motion.x, event.motion.y});
    return *noop_;
  }

  SDL_Keycode key = event.key.keysym.sym;

  if (key == SDLK_e) {
    return *button_up_right_;
  } else if (key == SDLK_q) {
    return *button_up_left_;
  } else if (key == SDLK_z) {
    return *button_down_left_;
  } else if (key == SDLK_c) {
    return *button_down_right_;
  } else if (key == SDLK_w || key == SDLK_UP) {
    return *button_up_;
  } else if (key == SDLK_s || key == SDLK_DOWN) {
    return *button_down_;
  } else if (key == SDLK_a || key == SDLK_LEFT) {
    return *button_left_;
  } else if (key == SDLK_d || key == SDLK_RIGHT) {
    return *button_right_;
  } else if (key == SDLK_v) {
    return *view_history_command_;
  } else if (key == SDLK_g) {
    return *pick_up_command_;
  } else if (key == SDLK_i) {
    return *inventory_command_;
  } else if (key == SDLK_COMMA) {
    return *main_menu_command_;
  } else if (key == SDLK_PERIOD) {
    return *character_menu_command_;
  } else if (key == SDLK_LEFTBRACKET) {
    auto entity_position = g_coordinator.get_component<SupaRL::TransformComponent>(
        controllable_entity_->get_id()).position_;
    use_command_ = std::make_unique<UseCommand>(
        world_, entity_position);
    return *use_command_;
  } else {
    return EventHandler::handle_sdl_event(event);
  }
};

EngineEvent& MenuInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_RETURN:
      return *resetGameCommand_;
      break;
    default:
      return EventHandler::handle_sdl_event(event);
      break;
  }
};

GuiInputHandler::GuiInputHandler(World& world, UiWindow& ui_window)
    : EventHandler(world),
      closeViewCommand_(std::make_unique<CloseViewCommand>(world)),
      scrollDownCommand_(std::make_unique<ScrollCommand>(world, ui_window, 1)),
      scrollUpCommand_(std::make_unique<ScrollCommand>(world, ui_window, -1)),
      jumpUpCommand_(std::make_unique<ScrollCommand>(world, ui_window, -10)),
      jumpDownCommand_(std::make_unique<ScrollCommand>(world, ui_window, 10)),
      jumpToHome_(std::make_unique<ScrollCommand>(world, ui_window, 0)){};

EngineEvent& GuiInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_j:
      return *scrollDownCommand_;
      break;
    case SDLK_k:
      return *scrollUpCommand_;
      break;
    case SDLK_HOME:
      return *jumpToHome_;
      break;
    case SDLK_PAGEUP:
      return *jumpUpCommand_;
      break;
    case SDLK_PAGEDOWN:
      return *jumpDownCommand_;
      break;
    case SDLK_q:
      return *closeViewCommand_;
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
    case SDLK_v:
      return *closeViewCommand_;
      break;
    default:
      return GuiInputHandler::handle_sdl_event(event);
      break;
  }
}

EngineEvent& InventoryInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case (SDLK_RETURN):
      return *selectItemCommand_;
      break;
    case (SDLK_d):
      return *dropItemCommand_;
      break;
    case SDLK_i:
      return *closeViewCommand_;
    default:
      return GuiInputHandler::handle_sdl_event(event);
      break;
  }
}

EngineEvent& MainMenuInputHandler::handle_sdl_event(SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_RETURN:
      return *selectMenuItemCommand_;
      break;
    case SDLK_COMMA:
      return *closeViewCommand_;
    default:
      return GuiInputHandler::handle_sdl_event(event);
      break;
  }
}

EngineEvent& CharacterMenuInputHandler::handle_sdl_event(
    SDL_Event event) noexcept {
  SDL_Keycode key = event.key.keysym.sym;
  switch (key) {
    case SDLK_RETURN:
      return *boost_stat_command_;
      break;
    case SDLK_COMMA:
      return *closeViewCommand_;
    default:
      return GuiInputHandler::handle_sdl_event(event);
      break;
  }
}
}  // namespace cpprl
