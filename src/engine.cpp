#include "engine.hpp"

#include <iostream>

namespace cpprl {

Engine::Engine(GameEntity& player, Map* map, InputHandler* input_handler)
    : player_(player), map_(map), input_handler_(input_handler) {}

void Engine::handle_events(SDL_Event& event) {
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      cpprl::Command* command = input_handler_->handle_input(event.key.keysym.sym);
      if (command) {
        command->execute(map_, player_);
      }
    } else if (event.type == SDL_QUIT) {
      std::exit(EXIT_SUCCESS);
    }
  }
}
void Engine::render(tcod::Console& console) {
  console.clear();
  // TODO: Should happen in the map render function
  map_->compute_fov(player_.get_position(), 4);
  for (int y{0}; y < map_->get_height(); ++y) {
    for (int x{0}; x < map_->get_width(); ++x) {
      if (!console.in_bounds({x, y})) continue;
      bool isFloor = map_->get_tiles().at({x, y}).type == TileType::floor;
      if (map_->is_in_fov({x, y})) {
        map_->set_is_explored({x, y});
        console.at({x, y}) = isFloor ? map_->get_floor_tile().light : map_->get_wall_tile().light;
      } else if (map_->is_explored({x, y})) {
        console.at({x, y}) = isFloor ? map_->get_floor_tile().dark : map_->get_wall_tile().dark;
      } else {
        console.at({x, y}) = TCOD_ConsoleTile{' ', tcod::ColorRGB{0, 0, 0}, tcod::ColorRGB{0, 0, 0}};
      }
    }
  }
  map_->render(console);
}
}  // namespace cpprl
