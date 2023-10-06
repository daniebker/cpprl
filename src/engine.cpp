
#include "engine.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

#include "events/engine_event.hpp"
#include "health_bar.hpp"
#include "history_window.hpp"
#include "input_handler.hpp"
#include "types/math.hpp"
#include "util.hpp"

namespace cpprl {

Engine::Engine(int argc, char** argv)
    : dungeon_(nullptr),
      entities_(nullptr),
      player_(nullptr),
      health_bar_(nullptr),
      map_(nullptr),
      message_log_(nullptr),
      history_window_(nullptr),
      input_handler_(nullptr),
      renderer_(nullptr) {
  dungeon_ = std::make_unique<Dungeon>();
  entities_ = std::make_unique<EntityManager>();
  renderer_ = std::make_unique<TCODRenderer>(argc, argv);
  generate_map(80, 35);
  message_log_ = std::make_unique<MessageLog>();
  message_log_->add_message("Welcome to your eternal doom!", RED);
  history_window_ = new HistoryWindow(80, 40, {0, 0}, *message_log_);
  input_handler_ = std::make_unique<GameInputHandler>(*this, *player_);
}

Engine::~Engine() { delete map_; }

void Engine::handle_events() {
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't
  // compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
  SDL_Event event;
#endif
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      EngineEvent& command = input_handler_->handle_input(event);
      command.execute();
      if (!game_over_ && !is_paused_) {
        handle_enemy_turns();
      }
    } else if (event.type == SDL_MOUSEMOTION) {
      input_handler_->handle_input(event);
    } else if (event.type == SDL_QUIT) {
      std::exit(EXIT_SUCCESS);
    }
  }
}

void Engine::generate_map(int width, int height) {
  map_ = dungeon_->generate(DungeonConfig{30, 6, 10, width, height, 2});

  std::vector<RectangularRoom> rooms = map_->get_rooms();
  int room_count = rooms.size();
  entities_->reserve(room_count * 2);
  for (auto it = rooms.begin() + 1; it != rooms.end(); ++it) {
    entities_->place_entities(*it, 2);
  }
  player_ = &entities_->spawn_player(rooms[0].get_center());
  map_->compute_fov(player_->get_position(), 4);
  health_bar_ = new HealthBar(20, 1, {2, 36}, player_->get_defense_component());
}

void Engine::render() {
  map_->compute_fov(player_->get_transform_component().position, 10);
  map_->render(g_console);

  for (GameEntity entity : *entities_) {
    // TODO: Entity should have a render function
    if (map_->is_in_fov(entity.get_transform_component().position)) {
      renderer_->render(
          entity.get_sprite_component(), entity.get_transform_component());
    }
  }
  health_bar_->render(g_console);
  if (show_history_view_) {
    history_window_->render(g_console);
  }

  message_log_->render(g_console, 23, 35, 45, 5);
  auto entities_at = entities_->get_entities_at(controller_.cursor);
  if (entities_at.size() > 0) {
    std::string names;
    for (auto& entity : entities_at) {
      names += entity->get_name() + ", ";
    }
    tcod::print_rect(
        g_console,
        {controller_.cursor.x, controller_.cursor.y - 1, 20, 1},
        names,
        WHITE,
        std::nullopt,
        TCOD_LEFT);
  }
  g_context.present(g_console);
}

void Engine::handle_enemy_turns() {
  for (GameEntity& entity : *entities_) {
    if (entity.get_name() != "player" && entity.is_not_dead()) {
      entity.act(*this);
    }
  }
}

void Engine::handle_player_death() {  //
  game_over_ = true;
  input_handler_ = std::make_unique<MenuInputHandler>(*this);
}

void Engine::reset_game() {
  game_over_ = false;
  // player is already freed?
  // delete player_;
  entities_->clear();
  // delete health_bar_;
  delete map_;
  dungeon_ = nullptr;
  entities_ = nullptr;
  player_ = nullptr;
  map_ = nullptr;
  health_bar_ = nullptr;
  generate_map(80, 40);
  set_input_handler(std::make_unique<GameInputHandler>(*this, *player_));
}

void Engine::set_input_handler(std::unique_ptr<InputHandler> input_handler) {
  input_handler_ = std::move(input_handler);
}
}  // namespace cpprl
