#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SDL2/SDL.h>

#include <libtcod.hpp>
#include <list>

#include "dungeon.hpp"
#include "entity_manager.hpp"
#include "message_log.hpp"
#include "ui_window.hpp"

namespace cpprl {

class InputHandler;
class GameEntity;
class Map;

class Engine {
 private:
  Dungeon& dungeon_;
  EntityManager entities_;
  GameEntity* player_;
  UiWindow* health_bar_;
  Map* map_;
  MessageLog* message_log_;
  std::unique_ptr<InputHandler> input_handler_;
  bool game_over_ = false;

  void generate_map(int width, int height);
  void handle_enemy_turns();

 public:
  Engine(EntityManager& entities, Dungeon& dungeon);
  ~Engine();
  void handle_events(SDL_Event& event);
  EntityManager& get_entities() { return entities_; };
  void render(tcod::Console& console);
  Map* get_map() { return map_; }
  GameEntity& get_player() { return *player_; }
  void handle_player_death();
  void reset_game();
  MessageLog& get_message_log() { return *message_log_; }
};
}  // namespace cpprl
#endif
