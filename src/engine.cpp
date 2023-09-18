#include "engine.hpp"

namespace cpprl {

Engine::Engine(std::list<GameEntity*> entities, InputHandler input_handler) {
  entities_ = entities;
  input_handler_ = input_handler;
}
Engine::~Engine() {}

void Engine::handle_events() {}
void Engine::render(TCOD_Console& console) {
  console.clear();
  for (auto& entity : entities_) {
    tcod::print(console, {entity->get_x(), entity->get_y()}, entity->get_symbol(), entity->get_colour(), std::nullopt);
  }
}
}  // namespace cpprl
