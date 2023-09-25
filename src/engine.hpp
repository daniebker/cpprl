#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SDL2/SDL.h>

#include <libtcod.hpp>
#include <list>

#include "input_handler.hpp"
#include "types/map.hpp"

namespace cpprl {
class Engine {
 private:
  std::list<GameEntity*> entities_;
  GameEntity& player_;
  Map* map_;
  InputHandler* input_handler_;

 public:
  Engine(std::list<GameEntity*> entities, GameEntity& player, Map* gameMap, InputHandler* input_handler);
  void handle_events(SDL_Event& event);
  void render(tcod::Console& console);
};
}  // namespace cpprl
#endif
