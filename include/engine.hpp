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

class Engine {
 private:
  // std::unique_ptr<Dungeon> dungeon_;
  UiWindow* health_bar_;
  // std::unique_ptr<MessageLog> message_log_;
  // UiWindow* current_window_;
  // EventHandler* input_handler_;
  std::unique_ptr<Renderer> renderer_;
  tcod::Context context_;
  // Controller controller_;
  // bool is_paused_ = false;
  // bool game_over_ = false;
  // bool show_view_ = false;
  // bool targeting_mode_ = false;
  std::unique_ptr<State> engine_state_;
  std::unique_ptr<World> world_;

  void generate_map(int width, int height);
  void handle_enemy_turns();

 public:
  Engine(int argc, char** argv);
  ~Engine();
  void handle_events();
  // EntityManager& get_entities() { return *entities_; };
  void render();
  // Map* get_map() { return map_; }
  // Entity* get_player() { return player_; }
  void handle_player_death();
  void reset_game();
  // MessageLog& get_message_log() { return *message_log_; }
  // Controller& get_controller() { return controller_; }
  // UiWindow& get_current_view() { return *current_window_; }
  // void toggle_pause() { is_paused_ = !is_paused_; }
  // void toggle_view() { show_view_ = !show_view_; }
  // void set_input_handler(EventHandler* input_handler);
  // void set_current_view(UiWindow* current_window) {
  //   current_window_ = current_window;
  // };
  // void scroll_current_view(int scroll_amount);
  // void toggle_targeting_mode() { targeting_mode_ = !targeting_mode_; }
  // void set_targeting_tile(
  //     float max_range = 0.0f, std::function<void()> callback = nullptr);
};
}  // namespace cpprl
#endif
