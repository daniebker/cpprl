
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__
#include "engine.hpp"

#include <SDL2/SDL.h>

#include <cereal/archives/binary.hpp>
// #include <cereal/types/memory.hpp>
#include <fstream>
#include <iostream>
#include <memory>

#include "events/command.hpp"
#include "exceptions.hpp"
#include "state.hpp"
#include "types/math.hpp"
#include "types/state_result.hpp"
#include "world.hpp"

namespace cpprl {

Engine::Engine(){};
Engine::~Engine(){};

Engine& Engine::get_instance() {
  static Engine instance_;
  return instance_;
}

void Engine::init(int argc, char** argv) {
  argc_ = argc;
  argv_ = argv;
  renderer_ = std::make_unique<TCODRenderer>(argc, argv);
  engine_state_ = std::make_unique<MainMenuState>(
      *world_, new MainMenuWindow(60, 35, {0, 0}));
  engine_state_->on_enter();
  // world_->generate_map(80, 35, true);
  // engine_state_->on_enter();
}

void Engine::save() {
  std::filesystem::create_directories("saves");
  if (world_->get_player()->get_defense_component().is_dead()) {
    // TCODSystem::deleteFile("game.sav");
    TCODSystem::deleteFile("saves/game.sav");

  } else {
    // TCODZip zip;
    std::ofstream os("saves/game.sav", std::ios::binary);
    // std::ofstream file("game.sav");
    cereal::JSONOutputArchive archive(os);
    world_->save(archive);
    // zip.saveToFile("game.sav");
  }
#ifdef __EMSCRIPTEN__
  // clang-format off
  EM_ASM(
    FS.syncfs(false, function (err) {
      assert(!err);
      console.log("SyncFS finished.");
    });
  );
  // clang-format on
#endif
}

void Engine::load() {
  if (TCODSystem::fileExists("game.sav")) {
    // TCODZip zip;
    // zip.loadFromFile("game.sav");
    std::ifstream is("game.sav", std::ios::binary);
    cereal::JSONInputArchive archive(is);

    world_ = std::make_unique<World>();

    // Use load function for loading
    // archive(world_);
    world_->load(archive);
    engine_state_->on_exit();
    engine_state_ = std::make_unique<InGameState>(*world_);
    engine_state_->on_enter();
  } else {
    init(argc_, argv_);
  }
}

void Engine::handle_events() {
  SDL_Event event;
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't
  // compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
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
        } else if (std::holds_alternative<LoadGame>(result)) {
          load();
        } else if (std::holds_alternative<EndTurn>(result)) {
          world_->handle_enemy_turns();
          if (world_->get_player()->get_defense_component().is_dead()) {
            engine_state_->on_exit();
            engine_state_ = std::make_unique<GameOverState>(*world_);
            engine_state_->on_enter();
          }
        } else if (std::holds_alternative<Quit>(result)) {
          save();
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
  if (world_) {
    world_->render(*renderer_);
  }
  engine_state_->render(*renderer_);
  g_context.present(g_console);
}

void Engine::reset_game() {
  world_ = std::make_unique<World>();
  engine_state_->on_exit();
  engine_state_ = std::make_unique<InGameState>(*world_);
  world_->generate_map(80, 35, true);
  engine_state_->on_enter();
}
}  // namespace cpprl
