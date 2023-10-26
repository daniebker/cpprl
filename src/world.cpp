#include "world.hpp"

#include "controller.hpp"
#include "dungeon.hpp"
#include "entity_manager.hpp"
#include "exceptions.hpp"
#include "game_entity.hpp"
#include "health_bar.hpp"

namespace cpprl {
World::World()
    : entities_(std::make_unique<EntityManager>()),
      player_(nullptr),
      map_(nullptr),
      current_window_(nullptr),
      show_view_(false) {
  controller_ = std::make_unique<Controller>();
  dungeon_ = std::make_unique<Dungeon>();
  message_log_ = std::make_unique<MessageLog>();

  message_log_->add_message("Welcome to your eternal doom!", RED);
  message_log_->add_message("May you travel safe", RED);
  message_log_->add_message(
      "Use W, A, S, D, Q, E, Z, C to move around the map. ", RED);
  message_log_->add_message("Press ESC to quit.", RED);
  message_log_->add_message("V opens your message log.", RED);
  message_log_->add_message(
      "Use J, K, PG U, PG D to scroll through messages. Use Q to quit.", RED);
}
void World::generate_map(int width, int height) {
  map_ = dungeon_->generate(DungeonConfig{30, 6, 10, width, height, 2});

  std::vector<RectangularRoom> rooms = map_->get_rooms();
  int room_count = rooms.size();
  entities_->reserve(room_count * 2);
  for (auto it = rooms.begin() + 1; it != rooms.end(); ++it) {
    entities_->place_entities(*it, 2, 1);
  }
  // TODO: move all this code to the factory pattern
  Entity* entity = new Entity(
      "player",
      true,
      new TransformComponent(
          {rooms[0].get_center().x, rooms[0].get_center().y}),
      new ASCIIComponent("@", RED, 1));
  entity->set_attack_component(new AttackComponent(5));
  entity->set_defense_component(new DefenseComponent(2, 30));
  entity->set_container(new Container(26));
  player_ = entities_->spawn(entity);

  map_->compute_fov(player_->get_transform_component()->get_position(), 4);
  auto& player_defense = *player_->get_defense_component();
  health_bar_ = new HealthBar(20, 1, {2, 36}, player_defense);
  entities_->shrink_to_fit();
}

void World::render(Renderer& renderer) {
  map_->compute_fov(player_->get_transform_component()->get_position(), 10);
  map_->render(g_console);

  for (Entity* entity : *entities_) {
    if (map_->is_in_fov(entity->get_transform_component()->get_position())) {
      renderer.render(
          *entity->get_sprite_component(), *entity->get_transform_component());
    }
  }
  health_bar_->render(g_console);

  message_log_->render(g_console, 23, 35, 45, 5);
  auto entities_at = entities_->get_entities_at(controller_->cursor);
  if (entities_at.size() > 0) {
    std::string names;
    for (auto& entity : entities_at) {
      names += entity->get_name() + ", ";
      tcod::print_rect(
          g_console,
          {controller_->cursor.x, controller_->cursor.y - 1, 20, 1},
          names,
          WHITE,
          std::nullopt,
          TCOD_LEFT);
    }
  }
}

void World::handle_enemy_turns() {
  for (Entity* entity : *entities_) {
    if (entity->get_ai_component() &&
        entity->get_defense_component()->is_not_dead()) {
      try {
        // dance puppet dance!
        entity->update(*this);
      } catch (Impossible&) {
      }
    }
  }
}

void World::reset() {}

void World::scroll_current_view(int scroll_amount) {
  if (current_window_) {
    current_window_->set_cursor(current_window_->get_cursor() + scroll_amount);
  }
}

void World::handle_player_death() {
  // TODO: what happens when the player dies?
  // game_over_ = true;
  // delete input_handler_;
  // input_handler_ = new MenuInputHandler(*this);
}

}  // namespace cpprl
