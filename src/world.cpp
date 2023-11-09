#include "world.hpp"

#include "controller.hpp"
#include "dungeon.hpp"
#include "entity_factory.hpp"
#include "entity_manager.hpp"
#include "exceptions.hpp"
#include "game_entity.hpp"
#include "health_bar.hpp"

namespace cpprl {
World::World()
    : entities_(std::make_unique<EntityManager>(
          std::make_unique<OrcFactory>(), std::make_unique<TrollFactory>())),
      player_(nullptr),
      map_(nullptr),
      current_window_(nullptr) {
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
void World::generate_map(int width, int height, bool with_entities) {
  // TODO: will need to pass the seed here
  map_ = dungeon_->generate(DungeonConfig{30, 6, 10, width, height, 2});

  if (!with_entities) {
    return;
  }

  std::vector<RectangularRoom> rooms = map_->get_rooms();
  int room_count = rooms.size();
  entities_->reserve(room_count * 2);
  for (auto it = rooms.begin() + 1; it != rooms.end(); ++it) {
    entities_->place_entities(*it, 2, 1);
  }

  auto player_factory_ = std::make_unique<PlayerFactory>();
  Entity* player = player_factory_->create();
  player_ = entities_->spawn(
      player, {rooms[0].get_center().x, rooms[0].get_center().y});
  map_->compute_fov(player_->get_transform_component().get_position(), 4);
  DefenseComponent& player_defense = player_->get_defense_component();
  health_bar_ = new HealthBar(20, 1, {2, 36}, player_defense);
  entities_->shrink_to_fit();
}

void World::render(Renderer& renderer) {
  map_->compute_fov(player_->get_transform_component().get_position(), 10);
  map_->render(g_console);

  for (Entity* entity : *entities_) {
    if (map_->is_in_fov(entity->get_transform_component().get_position())) {
      renderer.render(
          entity->get_sprite_component(), entity->get_transform_component());
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
    auto* ai_component = &entity->get_ai_component();
    if (ai_component && entity->get_defense_component().is_not_dead()) {
      try {
        // dance puppet dance!
        entity->update(*this);
      } catch (Impossible&) {
      }
    }
  }
}

void World::reset() { entities_->clear(); }

void World::scroll_current_view(int scroll_amount) {
  if (current_window_) {
    current_window_->set_cursor(current_window_->get_cursor() + scroll_amount);
  }
}

void World::save(TCODZip& zip) {
  zip.putInt(get_map().get_width());
  zip.putInt(get_map().get_height());
  dungeon_->save(zip);
  map_->save(zip);
  get_player()->save(zip);
  zip.putInt(get_entities().size() - 1);
  for (auto& entity : get_entities()) {
    if (entity->get_name() != "player") {
      entity->save(zip);
    }
  }
  get_message_log().save(zip);
}

void World::load(TCODZip& zip) {
  int width = zip.getInt();
  int height = zip.getInt();
  dungeon_->load(zip);
  generate_map(width, height, false);
  map_->load(zip);
  Entity* player = new Entity("", false, nullptr, nullptr);
  player->load(zip);
  player_ = entities_->spawn(player);
  DefenseComponent& player_defense = player_->get_defense_component();
  health_bar_ = new HealthBar(20, 1, {2, 36}, player_defense);
  int num_entities = zip.getInt();
  for (int i = 0; i < num_entities; ++i) {
    Entity* entity = new Entity("", false, nullptr, nullptr);
    entity->load(zip);
    entities_->spawn(entity);
  }
  get_message_log().load(zip);
}

}  // namespace cpprl
