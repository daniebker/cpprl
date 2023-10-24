#include "state.hpp"

#include "engine.hpp"
#include "exceptions.hpp"
#include "gui.hpp"
#include "rendering.hpp"
#include "world.hpp"

namespace cpprl {
void InGameState::on_enter() {
  input_handler_ =
      std::make_unique<GameInputHandler>(world_, world_.get_player());
}

StateResult InGameState::on_update(SDL_Event& event) {
  try {
    EngineEvent* command = input_handler_->handle_sdl_event(event);
    return command->execute();
  } catch (const Impossible& impossible) {
    world_.get_message_log().add_message(impossible.what(), RED);
    return {};
  }
}

void InGameState::on_exit() { world_.toggle_pause(); }
void InGameState::render(Renderer&) {}

void GuiViewState::render(Renderer&) { window_->render(g_console); }
void GuiViewState::on_enter() {}

void ViewMessageHistoryState::on_enter() {
  input_handler_ = std::make_unique<HistoryViewInputHandler>(world_, *window_);
}

StateResult ViewMessageHistoryState::on_update(SDL_Event& event) {
  try {
    EngineEvent* command = input_handler_->handle_sdl_event(event);
    return command->execute();
  } catch (const Impossible& impossible) {
    world_.get_message_log().add_message(impossible.what(), RED);
    return {};
  }
}

void ViewMessageHistoryState::on_exit() {}

void ViewMessageHistoryState::render(Renderer& renderer) {
  GuiViewState::render(renderer);
  // renderer.render(*world_.get_current_view());
}

void ViewInventoryState::on_enter() {
  input_handler_ = std::make_unique<InventoryInputHandler>(
      world_, world_.get_player(), *window_);
}

void ViewInventoryState::render(Renderer& renderer) {
  GuiViewState::render(renderer);
}

void ViewInventoryState::on_exit() {}
StateResult ViewInventoryState::on_update(SDL_Event& event) {
  try {
    EngineEvent* command = input_handler_->handle_sdl_event(event);
    return command->execute();
  } catch (const Impossible& impossible) {
    world_.get_message_log().add_message(impossible.what(), RED);
    return {};
  }
}

void PickTileAOEState::on_enter() {
  input_handler_ = std::make_unique<TargetingInputHandler>(world_);
}
StateResult PickTileAOEState::on_update(SDL_Event& event) {
  try {
    EngineEvent* command = input_handler_->handle_sdl_event(event);
    // here because the ptr is created in the function I need to
    // transfer the ownership. In the other functions the owner ship
    // is in the class.
    StateResult result = command->execute();
    delete command;
    return result;
  } catch (const Impossible& impossible) {
    world_.get_message_log().add_message(impossible.what(), RED);
    return {};
  }
}

void PickTileAOEState::on_exit() {
  // TODO: cast the spell
  // world_.toggle_pause();
  on_pick_();
}
void PickTileAOEState::render(Renderer&) {
  auto& map = world_.get_map();
  if (map.get_highlight_tile() != Vector2D{0, 0}) {
    with_indexes(map, [&, pos = map.get_highlight_tile()](int x, int y) {
      if (euclidean_squared(Vector2D{x, y} - pos) >= max_radius_squared_)
        return;
      if (!g_console.in_bounds({x, y})) return;
      auto& tile = g_console.at({x, y});
      tile = {tile.ch, tcod::ColorRGB{0, 0, 0}, tcod::ColorRGB{255, 255, 255}};
    });
  }
}
}  // namespace cpprl
