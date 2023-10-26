#include "state.hpp"

#include "engine.hpp"
#include "exceptions.hpp"
#include "gui.hpp"
#include "rendering.hpp"
#include "world.hpp"

namespace cpprl {

StateResult State::on_update(SDL_Event& event) {
  try {
    EngineEvent* command = input_handler_->handle_sdl_event(event);
    return command->execute();
  } catch (const Impossible& impossible) {
    world_.get_message_log().add_message(impossible.what(), RED);
    return {};
  }
}

void InGameState::on_enter() {
  input_handler_ =
      std::make_unique<GameInputHandler>(world_, world_.get_player());
}

void GuiViewState::render(Renderer&) { window_->render(g_console); }

void ViewMessageHistoryState::on_enter() {
  input_handler_ = std::make_unique<HistoryViewInputHandler>(world_, *window_);
}

void ViewInventoryState::on_enter() {
  input_handler_ = std::make_unique<InventoryInputHandler>(
      world_, world_.get_player(), *window_);
}

void PickTileState::on_enter() {
  input_handler_ = std::make_unique<TargetingInputHandler>(world_);
}

StateResult PickTileState::on_update(SDL_Event& event) {
  try {
    EngineEvent* command = input_handler_->handle_sdl_event(event);
    StateResult result = command->execute();
    delete command;
    return result;
  } catch (const Impossible& impossible) {
    world_.get_message_log().add_message(impossible.what(), RED);
    return {};
  }
}

void PickTileState::on_exit() { on_pick_(); }

void PickTileState::render(Renderer&) {
  auto& map = world_.get_map();
  Vector2D position = map.get_highlight_tile();
  auto& tile = g_console.at(position);
  tile = {tile.ch, tcod::ColorRGB{0, 0, 0}, tcod::ColorRGB{255, 255, 255}};
}

void PickTileAOEState::render(Renderer&) {
  auto& map = world_.get_map();
  if (map.get_highlight_tile() != Vector2D{0, 0}) {
    with_indexes(map, [&, pos = map.get_highlight_tile()](int x, int y) {
      if (euclidean_squared(Vector2D{x, y} - pos) >= aoe_squared_) return;
      if (!g_console.in_bounds({x, y})) return;
      auto& tile = g_console.at({x, y});
      tile = {tile.ch, tcod::ColorRGB{0, 0, 0}, tcod::ColorRGB{255, 255, 255}};
    });
  }
}

void GameOverState::on_enter() {
  input_handler_ = std::make_unique<MenuInputHandler>(world_);
  window_ = new GameOverWindow(20, 10, {0, 0}, "Game Over");
}
}  // namespace cpprl
