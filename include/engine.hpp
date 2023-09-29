#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SDL2/SDL.h>

#include <libtcod.hpp>
#include <list>

#include "dungeon.hpp"
#include "entity_manager.hpp"

namespace cpprl {

class InputHandler;
class GameEntity;
class Map;

class Engine {
 private:
  Dungeon& dungeon_;
  EntityManager entities_;
  GameEntity* player_;
  Map* map_;
  InputHandler* input_handler_;

  void generate_map(int width, int height);
  void handle_enemy_turns();

 public:
  Engine(EntityManager& entities, Dungeon& dungeon);
  void handle_events(SDL_Event& event);
  EntityManager& get_entities() { return entities_; };
  void render(tcod::Console& console);
  Map* get_map() { return map_; }
  GameEntity& get_player() { return *player_; }
  void handle_player_death();
};
}  // namespace cpprl
#endif
