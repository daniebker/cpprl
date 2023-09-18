#include "engine.hpp"

namespace cpprl {

Engine::Engine(std::list<GameEntity*> entities, GameEntity& player, InputHandler input_handler)
    : entities_(entities), player_(player), input_handler_(input_handler) {}
Engine::~Engine() {}

void Engine::handle_events(SDL_Event& event) {
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      cpprl::Command* command = input_handler_.handle_input(event.key.keysym.sym);
      command->execute(player_);
    } else if (event.type == SDL_QUIT) {
      std::exit(EXIT_SUCCESS);
    }
  }
}
void Engine::render(TCOD_Console& console) {
  console.clear();
  for (auto& entity : entities_) {
    tcod::print(console, {entity->get_x(), entity->get_y()}, entity->get_symbol(), entity->get_colour(), std::nullopt);
  }
}
}  // namespace cpprl
