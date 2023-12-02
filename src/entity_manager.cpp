#include "../include/entity_manager.hpp"

#include <SDL2/SDL.h>

#include <optional>

#include "basic_ai_component.hpp"
#include "colours.hpp"
#include "components.hpp"
#include "consumable_factory.hpp"
#include "util.hpp"

namespace cpprl {

void EntityManager::clear() { entities_.clear(); }

void EntityManager::clear_except_player() {
  entities_.erase(
      std::remove_if(
          entities_.begin(),
          entities_.end(),
          [](const Entity* entity) { return entity->get_name() != "Player"; }),
      entities_.end());
}

void EntityManager::place_entities(
    RectangularRoom room, int max_monsters_per_room, int max_items_per_room) {
  auto* random = TCODRandom::getInstance();
  int number_of_monsters = random->getInt(0, max_monsters_per_room);
  int number_of_items = random->getInt(0, max_items_per_room);

  for (int i = 0; i < number_of_monsters; i++) {
    Vector2D bottom_left, top_right;
    std::tie(bottom_left, top_right) = room.innerBounds();
    int x = random->getInt(bottom_left.x + 1, top_right.x - 1);
    int y = random->getInt(bottom_left.y + 1, top_right.y - 1);

    auto iterator = util::find_entity_at(entities_, x, y);

    if (iterator != entities_.end()) {
      continue;
    }

    if (random->getFloat(0.0f, 1.0f) < 0.8f) {
      Entity* entity = orc_factory_->create();
      entity->get_transform_component().move({x, y});
      spawn(std::move(entity));
    } else {
      Entity* entity = troll_factory_->create();
      spawn(std::move(entity), {x, y});
    }
  }

  for (int i = 0; i < number_of_items; i++) {
    Vector2D bottom_left, top_right;
    std::tie(bottom_left, top_right) = room.innerBounds();
    int x = random->getInt(bottom_left.x + 1, top_right.x - 1);
    int y = random->getInt(bottom_left.y + 1, top_right.y - 1);

    auto iterator = util::find_entity_at(entities_, x, y);

    if (iterator != entities_.end()) {
      continue;
    }

    float dice = random->getFloat(.0f, 1.0f);
    if (dice <= 0.7f) {
      auto health_potion_factory = std::make_unique<HealthPotionFactory>();
      Entity* entity = health_potion_factory->create();
      spawn(std::move(entity), {x, y});
    } else if (dice <= .8f) {
      auto lighting_scroll_factory = std::make_unique<LightningScrollFactory>();
      Entity* entity = lighting_scroll_factory->create();
      spawn(std::move(entity), {x, y});
    } else if (dice <= .9f) {
      auto fireball_scroll_factory = std::make_unique<FireballScrollFactory>();
      Entity* entity = fireball_scroll_factory->create();
      spawn(std::move(entity), {x, y});
    } else if (dice <= 1.0f) {
      auto confusion_scroll_factory =
          std::make_unique<ConfusionScrollFactory>();
      Entity* entity = confusion_scroll_factory->create();
      spawn(std::move(entity), {x, y});
    }
  }
}

Entity* EntityManager::spawn(Entity* src) {
  return entities_.emplace_back(src);
}

Entity* EntityManager::spawn(Entity* src, Vector2D position) {
  Entity* entity = spawn(src);

  if (position != entity->get_transform_component().get_position()) {
    entity->get_transform_component().move(position);
  }

  return entity;
}

std::vector<Entity*> EntityManager::get_entities_at(Vector2D position) {
  std::vector<Entity*> entities_at_position;
  // At max there can be 3? things at a position.
  // Corpse, Item, Actor...
  entities_at_position.reserve(3);
  for (auto& entity : entities_) {
    if (entity->get_transform_component().get_position() == position) {
      entities_at_position.push_back(entity);
    }
  }
  entities_at_position.shrink_to_fit();
  return entities_at_position;
}

std::optional<std::reference_wrapper<Entity>>
EntityManager::get_blocking_entity_at(Vector2D position) {
  for (const auto& entity : entities_) {
    if (entity->is_blocking() &&
        entity->get_transform_component().get_position() == position) {
      return std::reference_wrapper<Entity>(*entity);
    }
  }
  return std::nullopt;
}

std::optional<std::reference_wrapper<Entity>>
EntityManager::get_non_blocking_entity_at(Vector2D position) {
  for (const auto& entity : entities_) {
    if (!entity->is_blocking() &&
        entity->get_transform_component().get_position() == position) {
      return std::reference_wrapper<Entity>(*entity);
    }
  }
  return std::nullopt;
}

void EntityManager::remove(Entity* entity) {
  entities_.erase(
      std::remove_if(
          entities_.begin(),
          entities_.end(),
          [&entity](const Entity* e) { return e == entity; }),
      entities_.end());
}

std::optional<std::reference_wrapper<Entity>>
EntityManager::get_closest_living_monster(
    Vector2D position, float range) const {
  std::optional<std::reference_wrapper<Entity>> closest = std::nullopt;
  float best_distance = 1E6f;
  for (const auto& entity : entities_) {
    auto* defense_component = &entity->get_defense_component();
    auto* ai_component = &entity->get_ai_component();
    if (ai_component && defense_component) {
      float distance = position.distance_to(
          entity->get_transform_component().get_position());
      if (distance < best_distance && (distance <= range || range == 0.0f)) {
        best_distance = distance;
        closest = *entity;
      }
    }
  }
  return closest;
}
}  // namespace cpprl
