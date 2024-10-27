#include "state.hpp"

#include "engine.hpp"
#include "exceptions.hpp"
#include "gui.hpp"
#include "rendering.hpp"
#include "world.hpp"
#include <components/defence.hpp>

extern SupaRL::Coordinator g_coordinator;

namespace cpprl {

  StateResult State::on_update(SDL_Event& event) {
    try {
      EngineEvent& command = input_handler_->handle_sdl_event(event);
      return command.execute();
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

  void NextLevelState::on_enter() {
    // gen a new dungeon?
    world_.get_message_log().add_message(
        "You take a moment to rest to recover your strength.", WHITE);
    auto player_id = world_.get_player()->get_id();
    auto& player_defence = g_coordinator.get_component<SupaRL::DefenceComponent>(player_id);

    player_defence.heal(player_defence.max_hp_ / 2);

    world_.get_message_log().add_message(
        "After a rare moment of peace, you descend deeper into the heart of the "
        "dungeon...",
        WHITE);

    world_.get_entities().clear();
    // TODO: Should this go in on exit?
    world_.get_dungeon().increase_level();
    world_.generate_map(80, 35, true);
    // Add the player back to the entities.
    auto& player_transform = g_coordinator.get_component<SupaRL::TransformComponent>(
        world_.get_player()->get_id());
    player_transform.position_ = world_.get_map().get_rooms().at(0).get_center();
    world_.get_entities().spawn(world_.get_player());
  }

  StateResult NextLevelState::on_update(SDL_Event&) {
    return Change{std::make_unique<InGameState>(world_)};
  }

  void PickTileState::on_enter() {
    input_handler_ = std::make_unique<TargetingInputHandler>(world_);
  }

  StateResult PickTileState::on_update(SDL_Event& event) {
    try {
      EngineEvent& command = input_handler_->handle_sdl_event(event);
      StateResult result = command.execute();
      return result;
    } catch (const Impossible& impossible) {
      world_.get_message_log().add_message(impossible.what(), RED);
      return {};
    }
  }

  void PickTileState::on_exit() { on_pick_(); }

  void PickTileState::render(Renderer&) {
    auto& map = world_.get_map();
    SupaRL::Vector2D position = map.get_highlight_tile();
    auto& tile = g_console.at(position);
    tile = {tile.ch, tcod::ColorRGB{0, 0, 0}, tcod::ColorRGB{255, 255, 255}};
  }

  void PickTileAOEState::render(Renderer&) {
    auto& map = world_.get_map();
    if (map.get_highlight_tile() != SupaRL::Vector2D{0, 0}) {
      with_indexes(map, [&, pos = map.get_highlight_tile()](int x, int y) {
          if (euclidean_squared(SupaRL::Vector2D{x, y} - pos) >= aoe_squared_) return;
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

  void MainMenuState::on_enter() {
    window_ = new MainMenuWindow(20, 10, {0, 0}, "Main Menu");
    input_handler_ = std::make_unique<MainMenuInputHandler>(world_, *window_);
  }

  void CharacterMenuState::on_enter() {
    window_ = new CharacterMenuWindow(
        20, 10, {0, 0}, world_.get_player(), "Character Menu");
    input_handler_ =
      std::make_unique<CharacterMenuInputHandler>(world_, *window_);
  }
}
