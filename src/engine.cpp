
#include "engine.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

#include "events/command.hpp"
#include "exceptions.hpp"
#include "state.hpp"
#include "types/math.hpp"
#include "types/state_result.hpp"
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

void Engine::reset_game() {
  world_->reset();
  world_->generate_map(80, 40);
}
}  // namespace cpprl
