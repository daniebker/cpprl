#include "../include/entity_manager.hpp"

#include <SDL2/SDL.h>

#include "basic_ai_component.hpp"
#include "colours.hpp"
#include "components.hpp"
#include "util.hpp"

namespace cpprl {

void EntityManager::clear() { entities_.clear(); }

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
      Entity* entity = new Entity(
          "orc",
          true,
          std::make_unique<TransformComponent>(x, y),
          std::make_unique<ASCIIComponent>("o", WHITE, 1));
      entity->set_defense_component(std::make_unique<DefenseComponent>(0, 10));
      entity->set_attack_component(std::make_unique<AttackComponent>(3));
      entity->set_ai_component(new HostileAI());
      spawn(entity);
    } else {
      Entity* entity = new Entity(
          "troll",
          true,
          std::make_unique<TransformComponent>(x, y),
          std::make_unique<ASCIIComponent>("T", WHITE, 1));
      entity->set_attack_component(std::make_unique<AttackComponent>(4));
      entity->set_defense_component(std::make_unique<DefenseComponent>(1, 16));
      entity->set_ai_component(new HostileAI());
      spawn(entity);
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
      Entity* entity = new Entity(
          "healing potion",
          false,
          std::make_unique<TransformComponent>(x, y),
          std::make_unique<ASCIIComponent>("!", DARK_RED, 0));
      entity->set_consumable_component(new HealingConsumable(10));
      spawn(entity);
    } else if (dice <= .8f) {
      Entity* entity = new Entity(
          "Lightning Scroll",
          false,
          std::make_unique<TransformComponent>(x, y),
          std::make_unique<ASCIIComponent>("#", DARK_RED, 0));
      entity->set_consumable_component(new LightningBolt(5, 20));
      spawn(entity);
    } else if (dice <= .9f) {
      Entity* entity = new Entity(
          "Fire Scroll",
          false,
          std::make_unique<TransformComponent>(x, y),
          std::make_unique<ASCIIComponent>("#", DARK_RED, 0));
      entity->set_consumable_component(new FireSpell(5, 3, 20));
      spawn(entity);
    } else if (dice <= 1.0f) {
      Entity* entity = new Entity(
          "Confusion Scroll",
          false,
          std::make_unique<TransformComponent>(x, y),
          std::make_unique<ASCIIComponent>("#", DARK_RED, 0));
      entity->set_consumable_component(new ConfusionSpell(3, 5));
      spawn(entity);
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

Entity* EntityManager::get_blocking_entity_at(Vector2D position) {
  for (Entity* entity : entities_) {
    if (entity->is_blocking() &&
        entity->get_transform_component().get_position() == position) {
      return entity;
    }
  }
  return nullptr;
}

Entity* EntityManager::get_non_blocking_entity_at(Vector2D position) {
  for (Entity* entity : entities_) {
    if (!entity->is_blocking() &&
        entity->get_transform_component().get_position() == position) {
      return entity;
    }
  }
  return nullptr;
}

void EntityManager::remove(Entity* entity) {
  entities_.erase(
      std::remove_if(
          entities_.begin(),
          entities_.end(),
          [&entity](const Entity* e) { return e == entity; }),
      entities_.end());
}

Entity* EntityManager::get_closest_living_monster(
    Vector2D position, float range) const {
  Entity* closest = nullptr;
  float best_distance = 1E6f;
  for (Entity* entity : entities_) {
    auto* defense_component = &entity->get_defense_component();
    if (entity->get_ai_component() && defense_component) {
      float distance = position.distance_to(
          entity->get_transform_component().get_position());
      if (distance < best_distance && (distance <= range || range == 0.0f)) {
        best_distance = distance;
        closest = entity;
      }
    }
  }
  return closest;
}
}  // namespace cpprl
