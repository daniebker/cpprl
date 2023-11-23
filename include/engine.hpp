#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SDL2/SDL.h>

#include <libtcod.hpp>
#include <list>

#include "controller.hpp"
#include "dungeon.hpp"
#include "entity_manager.hpp"
#include "globals.hpp"
#include "gui.hpp"
#include "message_log.hpp"
#include "rendering.hpp"

namespace cpprl {

class EventHandler;
class GameInputHandler;
class GameActor;
class Map;
class State;

// singleton engine
class Engine {
 private:
  std::unique_ptr<Renderer> renderer_;
  tcod::Context context_;
  std::unique_ptr<World> world_;
  std::unique_ptr<State> engine_state_;
  int argc_;
  char** argv_;

  void generate_map(int width, int height);
  void handle_enemy_turns();

 public:
  Engine();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  ~Engine();

  static Engine& get_instance();
  void handle_events();
  void render();
  void handle_player_death();
  void reset_game();
  void load();
  void save();
  void init(int argc, char** argv);
};
}  // namespace cpprl
#endif
