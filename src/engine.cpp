
#include "engine.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

#include "events/engine_event.hpp"
#include "exceptions.hpp"
#include "gui.hpp"
#include "health_bar.hpp"
#include "input_handler.hpp"
#include "types/math.hpp"
#include "util.hpp"

namespace cpprl {

Engine::Engine(int argc, char** argv)
    : dungeon_(nullptr),
      entities_(nullptr),
      player_(nullptr),
      health_bar_(nullptr),
      map_(nullptr),
      message_log_(nullptr),
      current_window_(nullptr),
      input_handler_(nullptr),
      renderer_(nullptr) {
  dungeon_ = std::make_unique<Dungeon>();
  entities_ = std::make_unique<EntityManager>();
  renderer_ = std::make_unique<TCODRenderer>(argc, argv);
  generate_map(80, 35);
  message_log_ = std::make_unique<MessageLog>();
  message_log_->add_message("Welcome to your eternal doom!", RED);
  message_log_->add_message("May you travel safe", RED);
  message_log_->add_message(
      "Use W, A, S, D, Q, E, Z, C to move around the map. ", RED);
  message_log_->add_message("Press ESC to quit.", RED);
  message_log_->add_message("V opens your message log.", RED);
  message_log_->add_message(
      "Use J, K, PG U, PG D to scroll through messages. Use Q to quit.", RED);
  input_handler_ = new GameInputHandler(*this, player_);
}

Engine::~Engine() { delete map_; }

void Engine::handle_events() {
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't
  // compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
  SDL_Event event;
#endif
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      try {
        EngineEvent& command = input_handler_->handle_sdl_event(event);
        command.execute();
        if (!game_over_ && !is_paused_) {
          handle_enemy_turns();
        }
      } catch (const Impossible& impossible) {
        message_log_->add_message(impossible.what(), RED);
      }
    } else if (event.type == SDL_MOUSEMOTION) {
      input_handler_->handle_sdl_event(event);
    } else if (event.type == SDL_QUIT) {
      std::exit(EXIT_SUCCESS);
    }
  }
}

void Engine::generate_map(int width, int height) {
  map_ = dungeon_->generate(DungeonConfig{30, 6, 10, width, height, 2});

  std::vector<RectangularRoom> rooms = map_->get_rooms();
  int room_count = rooms.size();
  entities_->reserve(room_count * 2);
  for (auto it = rooms.begin() + 1; it != rooms.end(); ++it) {
    entities_->place_entities(*it, 2, 1);
  }
  Entity* entity = new Entity(
      "player",
      true,
      new TransformComponent(
          {rooms[0].get_center().x, rooms[0].get_center().y}),
      new ASCIIComponent("@", RED, 1));
  entity->set_attack_component(new AttackComponent(5));
  entity->set_defense_component(new DefenseComponent(2, 30));
  entity->set_container(new Container(26));
  Entity* first_potion = new Entity(
      "Healing Potion",
      false,
      new TransformComponent({0, 0}),
      new ASCIIComponent("!", WHITE, 1));
  first_potion->set_consumable_component(new HealingConsumable(10));
  entity->get_container()->add(first_potion);
  Entity* firstScroll = new Entity(
      "Lightning Scroll",
      false,
      new TransformComponent({0, 0}),
      new ASCIIComponent("#", DARK_RED, 0));
  firstScroll->set_consumable_component(new LightningBolt(5, 20));
  entity->get_container()->add(firstScroll);
  player_ = entities_->spawn(entity);

  map_->compute_fov(player_->get_transform_component()->get_position(), 4);
  health_bar_ =
      new HealthBar(20, 1, {2, 36}, *player_->get_defense_component());
  entities_->shrink_to_fit();
}

void Engine::render() {
  map_->compute_fov(player_->get_transform_component()->get_position(), 10);
  map_->render(g_console);

  for (Entity* entity : *entities_) {
    if (map_->is_in_fov(entity->get_transform_component()->get_position())) {
      renderer_->render(
          *entity->get_sprite_component(), *entity->get_transform_component());
    }
  }
  health_bar_->render(g_console);

  message_log_->render(g_console, 23, 35, 45, 5);
  auto entities_at = entities_->get_entities_at(controller_.cursor);
  if (entities_at.size() > 0) {
    std::string names;
    for (auto& entity : entities_at) {
      names += entity->get_name() + ", ";
      tcod::print_rect(
          g_console,
          {controller_.cursor.x, controller_.cursor.y - 1, 20, 1},
          names,
          WHITE,
          std::nullopt,
          TCOD_LEFT);
  }
  }
  if (show_view_) {
    current_window_->render(g_console);
  }
  g_context.present(g_console);
}

void Engine::handle_enemy_turns() {
  for (Entity* entity : *entities_) {
    if (entity->get_ai_component() &&
        entity->get_defense_component()->is_not_dead()) {
      // dance puppet dance!
      entity->update(*this);
    }
  }
}

void Engine::handle_player_death() {
  game_over_ = true;
  delete input_handler_;
  input_handler_ = new MenuInputHandler(*this);
}

void Engine::reset_game() {
  game_over_ = false;
  entities_->clear();
  map_ = nullptr;
  dungeon_ = nullptr;
  entities_ = nullptr;
  player_ = nullptr;
  map_ = nullptr;
  health_bar_ = nullptr;
  input_handler_ = nullptr;
  generate_map(80, 40);
  set_input_handler(new GameInputHandler(*this, player_));
}

void Engine::set_input_handler(EventHandler* input_handler) {
  delete input_handler_;
  input_handler_ = input_handler;
}

void Engine::scroll_current_view(int scroll_amount) {
  if (show_view_) {
    current_window_->set_cursor(current_window_->get_cursor() + scroll_amount);
  }
}
}  // namespace cpprl
