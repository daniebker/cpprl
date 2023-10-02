#include "../include/engine.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

#include "events/engine_event.hpp"
#include "health_bar.hpp"
#include "input_handler.hpp"
#include "types/math.hpp"

namespace cpprl {

Engine::Engine(EntityManager& entities, Dungeon& dungeon)
    : dungeon_(dungeon),
      entities_(entities),
      player_(nullptr),
      health_bar_(nullptr),
      map_(nullptr),
      message_log_(nullptr),
      input_handler_(nullptr) {
  generate_map(80, 40);
  message_log_ = new MessageLog(5);
  message_log_->add_message("Welcome to cpprl!");
  input_handler_ = std::make_unique<GameInputHandler>(*this, *player_);
}

Engine::~Engine() { delete map_; }

void Engine::handle_events(SDL_Event& event) {
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't
  // compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      EngineEvent& command = input_handler_->handle_input(event.key.keysym.sym);
      command.execute();
      if (!game_over_) {
        handle_enemy_turns();
      }
    } else if (event.type == SDL_QUIT) {
      std::exit(EXIT_SUCCESS);
    }
  }
}

void Engine::generate_map(int width, int height) {
  map_ = dungeon_.generate(DungeonConfig{30, 6, 10, width, height, 2});

  std::vector<RectangularRoom> rooms = map_->get_rooms();
  int room_count = rooms.size();
  entities_.reserve(room_count * 2);
  for (auto it = rooms.begin() + 1; it != rooms.end(); ++it) {
    entities_.place_entities(*it, 2);
  }
  player_ = &entities_.spawn_player(rooms[0].get_center());
  map_->compute_fov(player_->get_position(), 4);
  health_bar_ = new HealthBar(20, 1, {2, 36}, player_->get_defense_component());
}

void Engine::render(tcod::Console& console) {
  map_->compute_fov(player_->get_position(), 10);
  map_->render(console);

  for (auto entity : entities_) {
    // TODO: Entity should have a render function
    if (map_->is_in_fov(entity.get_position())) {
      tcod::print(
          console,
          entity.get_position(),
          entity.get_symbol(),
          entity.get_colour(),
          std::nullopt);
    }
  }
  health_bar_->render(console);

  message_log_->render(console, 2, 2, 76, 4);
}

void Engine::handle_enemy_turns() {
  for (GameEntity& entity : entities_) {
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
  entities_.clear();
  delete health_bar_;
  delete map_;
  player_ = nullptr;
  map_ = nullptr;
  health_bar_ = nullptr;
  generate_map(80, 40);
  input_handler_ = std::make_unique<GameInputHandler>(*this, *player_);
}
}  // namespace cpprl
