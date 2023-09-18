#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SDL.h>

#include <libtcod.hpp>
#include <list>

#include "input_handler.hpp"

namespace cpprl {
class Engine {
 private:
  std::list<GameEntity*> entities_;
  GameEntity& player_;
  InputHandler input_handler_;

 public:
  Engine(std::list<GameEntity*> entities, GameEntity& player, InputHandler input_handler);
  virtual ~Engine();
  void handle_events(SDL_Event& event);
  void render(TCOD_Console& console);
};
}  // namespace cpprl
#endif
