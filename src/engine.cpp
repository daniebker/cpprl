#include "../include/engine.hpp"

#include <SDL2/SDL.h>

#include <iostream>

#include "../include/events/engine_event.hpp"
#include "../include/input_handler.hpp"

namespace cpprl {

Engine::Engine(EntityManager& entities, Dungeon& dungeon)
    : dungeon_(dungeon), entities_(entities), player_(nullptr), map_(nullptr), input_handler_(nullptr) {
  generate_map(80, 40);
  // TODO: this is a dodgy way to do this
  // Input handler needs a ref to the engines
  // player so we must always ensure the player
  // is created before the input handler
  input_handler_ = std::make_unique<GameInputHandler>(*this);
}

Engine::~Engine() { delete map_; }

void Engine::handle_events(SDL_Event& event) {
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      EngineEvent* command = input_handler_->handle_input(event.key.keysym.sym);
      command->execute();
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
  for (auto it = rooms.begin() + 1; it != rooms.end(); ++it) {
    entities_.place_entities(*it, 2);
  }
  player_ = &entities_.spawn_player(rooms[0].get_center());
  map_->compute_fov(player_->get_position(), 4);
}

void Engine::render(tcod::Console& console) {
  map_->compute_fov(player_->get_position(), 4);
  map_->render(console);

  for (auto entity : entities_) {
    // TODO: Entity should have a render function
    if (map_->is_in_fov(entity.get_position())) {
      tcod::print(console, entity.get_position(), entity.get_symbol(), entity.get_colour(), std::nullopt);
    }
  }
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
  delete map_;
  player_ = nullptr;
  map_ = nullptr;
  generate_map(80, 40);
  // TODO: this is a dodgy way to do this
  // Input handler needs a ref to the engines
  // player so we must always ensure the player
  // is created before the input handler
  input_handler_ = std::make_unique<GameInputHandler>(*this);
}
}  // namespace cpprl
