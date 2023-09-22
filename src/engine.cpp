#include "engine.hpp"

#include <iostream>

namespace cpprl {

Engine::Engine(std::list<GameEntity*> entities, GameEntity& player, Map& map, InputHandler input_handler)
    : entities_(entities), player_(player), map_(map), input_handler_(input_handler) {}
Engine::~Engine() {}

void Engine::handle_events(SDL_Event& event) {
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      cpprl::Command* command = input_handler_.handle_input(event.key.keysym.sym);
      command->execute(map_, player_);
    } else if (event.type == SDL_QUIT) {
      std::exit(EXIT_SUCCESS);
    }
  }
}
void Engine::render(tcod::Console& console) {
  console.clear();
  for (int y{0}; y < map_.get_height(); ++y) {
    for (int x{0}; x < map_.get_width(); ++x) {
      if (!console.in_bounds({x, y})) continue;
      bool isFloor = map_.get_tiles().at(x, y) == Tiles::floor;
      console.at({x, y}) = isFloor ? TCOD_ConsoleTile{'.', tcod::ColorRGB{128, 128, 128}, tcod::ColorRGB{0, 0, 0}}
                                   : TCOD_ConsoleTile{'#', tcod::ColorRGB{128, 128, 128}, tcod::ColorRGB{0, 0, 0}};
    }
  }
  for (auto entity : entities_) {
    tcod::print(console, {entity->get_x(), entity->get_y()}, entity->get_symbol(), entity->get_colour(), std::nullopt);
  }
}
}  // namespace cpprl
