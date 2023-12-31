#include "world.hpp"

#include "controller.hpp"
#include "dungeon.hpp"
#include "entity_factory.hpp"
#include "entity_manager.hpp"
#include "exceptions.hpp"
#include "game_entity.hpp"
#include "health_bar.hpp"
#include "ui/dungeon_level.hpp"

namespace cpprl {
// TODO: When loading from state this constructor is not called
// need to ensure the orc and troll factories are set.
World::World()
    : entities_(std::make_unique<EntityManager>(
          std::make_unique<OrcFactory>(), std::make_unique<TrollFactory>())),
      current_window_(nullptr),
      player_(nullptr),
      ui_(nullptr) {
  controller_ = std::make_unique<Controller>();
  dungeon_ = std::make_unique<Dungeon>();
  message_log_ = std::make_unique<MessageLog>();
  // dungeon_level_ =
  //     std::make_unique<DungeonLevel>(20, 1, Vector2D{2, 35}, *dungeon_);
  ui_ = std::make_unique<UI>(*dungeon_);
  message_log_->add_message("Welcome to your eternal doom!", WHITE);
  // TODO: add help menu
  // message_log_->add_message("Press '?' for help.", WHITE);
  message_log_->add_message("Press 'ESC' to quit.", WHITE);
  message_log_->add_message("Press 'i' to open your inventory.", WHITE);
  message_log_->add_message("Press 'g' to pick up items.", WHITE);
  message_log_->add_message("Press 'd' in the inventory to drop items.", WHITE);
  message_log_->add_message("Press 'w' 'a' 's' 'd' to move.", WHITE);
  message_log_->add_message("Press 'v' to view your adventure log", WHITE);
  message_log_->add_message("Now, fly you fool!", WHITE);
}
void World::generate_map(int width, int height, bool with_entities) {
  // TODO: will need to pass the seed here
  dungeon_->generate(DungeonConfig{30, 6, 10, width, height, 2});

  if (!with_entities) {
    return;
  }

  std::vector<RectangularRoom> rooms = dungeon_->get_map().get_rooms();
  int room_count = rooms.size();
  entities_->reserve(room_count * 2);
  for (auto it = rooms.begin() + 1; it != rooms.end(); ++it) {
    entities_->place_entities(*it, 2, 1);
  }
}

void World::render(Renderer& renderer) {
  dungeon_->get_map().compute_fov(
      player_->get_transform_component().get_position(), 10);
  dungeon_->get_map().render(g_console);

  for (const auto& entity : *entities_) {
    if (dungeon_->get_map().is_in_fov(
            entity->get_transform_component().get_position())) {
      renderer.render(
          entity->get_sprite_component(), entity->get_transform_component());
    }
  }
  // health_bar_->render(g_console);
  ui_->render(g_console);

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
  for (const auto& entity : *entities_) {
    auto* ai_component = &entity->get_ai_component();
    if (ai_component && entity->get_defense_component().is_not_dead()) {
      entity->update(*this);
    }
  }
}

void World::spawn_player() {
  auto player_factory_ = std::make_unique<PlayerFactory>();
  Entity* player = player_factory_->create();
  player->get_transform_component().move(
      dungeon_->get_map().get_rooms().at(0).get_center());
  World::spawn_player(player);
}

void World::spawn_player(Entity* player) {
  player_ = entities_->spawn(player);
  dungeon_->get_map().compute_fov(
      player_->get_transform_component().get_position(), 4);
  DefenseComponent& player_defense = player_->get_defense_component();
  ui_->set_health_bar(player_defense);
  // health_bar_ = new HealthBar(20, 1, {2, 36}, player_defense);
  entities_->shrink_to_fit();
}

void World::reset() { entities_->clear(); }

void World::scroll_current_view(int scroll_amount) {
  if (current_window_) {
    current_window_->set_cursor(current_window_->get_cursor() + scroll_amount);
  }
}

}  // namespace cpprl
