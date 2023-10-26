
#include "engine.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

#include "events/command.hpp"
#include "exceptions.hpp"
#include "gui.hpp"
#include "health_bar.hpp"
#include "input_handler.hpp"
#include "state.hpp"
#include "types/math.hpp"
#include "types/state_result.hpp"
#include "util.hpp"
#include "world.hpp"

namespace cpprl {

Engine::Engine(int argc, char** argv)
    : renderer_(std::make_unique<TCODRenderer>(argc, argv)),
      world_(std::make_unique<World>()),
      engine_state_(std::make_unique<InGameState>(*world_)) {
  world_->generate_map(80, 35);
  engine_state_->on_enter();
}
Engine::~Engine() {

}

void Engine::handle_events() {
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't
  // compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
  SDL_Event event;
#endif
  while (SDL_PollEvent(&event)) {
    // call on_update of state which can return change
    if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEMOTION ||
        event.type == SDL_MOUSEBUTTONDOWN) {
      try {
        StateResult result = engine_state_->on_update(event);
        if (std::holds_alternative<std::monostate>(result)) {
        } else if (std::holds_alternative<Change>(result)) {
          engine_state_->on_exit();
          engine_state_ = std::move(std::get<Change>(result).next_state);
          engine_state_->on_enter();
        } else if (std::holds_alternative<Reset>(result)) {
          reset_game();
        } else if (std::holds_alternative<EndTurn>(result)) {
          world_->handle_enemy_turns();
        } else if (std::holds_alternative<Quit>(result)) {
          std::exit(EXIT_SUCCESS);
        }
      } catch (Impossible& e) {
        world_->get_message_log().add_message(e.what(), RED);
      }
    } else if (event.type == SDL_QUIT) {
      std::exit(EXIT_SUCCESS);
    }
  }
}

void Engine::render() {
  world_->render(*renderer_);
  engine_state_->render(*renderer_);
  g_context.present(g_console);
}

// void Engine::handle_enemy_turns() {}

// void Engine::handle_player_death() {
//   game_over_ = true;
//   delete input_handler_;
//   input_handler_ = new MenuInputHandler(*this);
// }

void Engine::reset_game() {
  // game_over_ = false;
  // entities_->clear();
  world_->reset();
  // map_ = nullptr;
  // dungeon_ = nullptr;
  // entities_ = nullptr;
  // player_ = nullptr;
  // map_ = nullptr;
  // health_bar_ = nullptr;
  // input_handler_ = nullptr;
  world_->generate_map(80, 40);
  // generate_map(80, 40);
  // set_input_handler(new GameInputHandler(*this, player_));
}

// void Engine::set_input_handler(EventHandler* input_handler) {
//   delete input_handler_;
//   input_handler_ = input_handler;
// }

// void Engine::scroll_current_view(int scroll_amount) {
//   if (show_view_) {
//     current_window_->set_cursor(current_window_->get_cursor() +
//     scroll_amount);
//   }
// }

// // TODO: move this to state, then add a callback to calc damage.
// void Engine::set_targeting_tile(
//     float max_range, std::function<void()> callback) {
//   input_handler_ = new TargetingInputHandler(*this);
//   targeting_mode_ = !targeting_mode_;
//   map_->toggle_target_mode(max_range);
//   // SDL_WaitEvent(nullptr);
//   // SDL_Event event;
//   // EngineEvent& command = input_handler_->handle_sdl_event(event);
//   // command.execute();
//   // Engine::render();
//   // if (!targeting_mode_) {
//   //   callback();
//   // }
// }
}  // namespace cpprl
