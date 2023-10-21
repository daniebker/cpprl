#include "world.hpp"

namespace cpprl {
World::generate_map(int width, int height) {
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
      "Fire Scroll",
      false,
      new TransformComponent({0, 0}),
      new ASCIIComponent("#", DARK_RED, 0));
  firstScroll->set_consumable_component(new FireSpell(5, 20));
  entity->get_container()->add(firstScroll);
  player_ = entities_->spawn(entity);

  map_->compute_fov(player_->get_transform_component()->get_position(), 4);
  health_bar_ =
      new HealthBar(20, 1, {2, 36}, *player_->get_defense_component());
  entities_->shrink_to_fit();
}

void World::render() {
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
  // if (show_view_) {
  //   current_window_->render(g_console);
  // }
  g_context.present(g_console);
}

void World::handle_eney_turns() {
  for (Entity* entity : *entities_) {
    if (entity->get_ai_component() &&
        entity->get_defense_component()->is_not_dead()) {
      // dance puppet dance!
      entity->update(*this);
    }
  }
}

}  // namespace cpprl
