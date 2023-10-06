#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SDL2/SDL.h>

#include <libtcod.hpp>
#include <list>

#include "controller.hpp"
#include "dungeon.hpp"
#include "entity_manager.hpp"
#include "globals.hpp"
#include "message_log.hpp"
#include "ui_window.hpp"

namespace cpprl {

class InputHandler;
class GameEntity;
class Map;

class Engine {
 private:
  std::unique_ptr<Dungeon> dungeon_;
  std::unique_ptr<EntityManager> entities_;
  GameEntity* player_;
  UiWindow* health_bar_;
  Map* map_;
  MessageLog* message_log_;
  UiWindow* history_window_;
  std::unique_ptr<InputHandler> input_handler_;
  tcod::Context context_;
  Controller controller_;
  bool game_over_ = false;

  void generate_map(int width, int height);
  void handle_enemy_turns();

 public:
  Engine(int argc, char** argv);
  ~Engine();
  void handle_events();
  EntityManager& get_entities() { return *entities_; };
  void render();
  Map* get_map() { return map_; }
  GameEntity& get_player() { return *player_; }
  void handle_player_death();
  void reset_game();
  MessageLog& get_message_log() { return *message_log_; }
  Controller& get_controller() { return controller_; }
};
}  // namespace cpprl
#endif
