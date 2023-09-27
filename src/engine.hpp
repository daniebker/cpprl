#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SDL2/SDL.h>

#include <libtcod.hpp>
#include <list>

#include "dungeon.hpp"
#include "entity_manager.hpp"
#include "input_handler.hpp"
#include "types/map.hpp"

namespace cpprl {
class Engine {
 private:
  Dungeon& dungeon_;
  EntityManager entities_;
  GameEntity* player_;
  Map* map_;
  InputHandler* input_handler_;

  void generate_map(int width, int height);

 public:
  Engine(EntityManager& entities, Dungeon& dungeon, InputHandler* input_handler);
  void handle_events(SDL_Event& event);
  void render(tcod::Console& console);
};
}  // namespace cpprl
#endif
