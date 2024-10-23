#include "../include/entity_manager.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <optional>

#include "consumable_factory.hpp"
#include "util.hpp"

#include <components/identity.hpp>
#include <components/physique.hpp>
#include <components/defence.hpp>
#include <core/coordinator.hpp>

extern SupaRL::Coordinator g_coordinator;
namespace cpprl {
  void EntityManager::clear() { entities_.clear(); }

  void EntityManager::clear_except_player() {
    entities_.erase(
        std::begin(std::ranges::remove_if(
            entities_,
            [](const Entity* entity) {
            auto& entity_name = g_coordinator.get_component<SupaRL::IdentityComponent>(
                entity->get_id()).name_;
            return entity_name != "Player"; })),
        entities_.end());
  }

  void EntityManager::place_entities(
      RectangularRoom room, int max_monsters_per_room, int max_items_per_room) {
    auto* random = TCODRandom::getInstance();
    int number_of_monsters = random->getInt(0, max_monsters_per_room);
    int number_of_items = random->getInt(0, max_items_per_room);

    for (int i = 0; i < number_of_monsters; i++) {
      SupaRL::Vector2D bottom_left;
      SupaRL::Vector2D top_right;
      std::tie(bottom_left, top_right) = room.innerBounds();
      int x = random->getInt(bottom_left.x + 1, top_right.x - 1);
      int y = random->getInt(bottom_left.y + 1, top_right.y - 1);

      if (auto iterator = util::find_entity_at(entities_, x, y);
          iterator != entities_.end()) {
        continue;
      }

      if (random->getFloat(0.0f, 1.0f) < 0.8f) {
        Entity* entity = orc_factory_->create({x,y});
        spawn(std::move(entity));
      } else {
        Entity* entity = troll_factory_->create({x,y});
        spawn(std::move(entity));
      }
    }

    for (int i = 0; i < number_of_items; i++) {
      SupaRL::Vector2D bottom_left;
      SupaRL::Vector2D top_right;
      std::tie(bottom_left, top_right) = room.innerBounds();
      int x = random->getInt(bottom_left.x + 1, top_right.x - 1);
      int y = random->getInt(bottom_left.y + 1, top_right.y - 1);

      if (auto iterator = util::find_entity_at(entities_, x, y);
          iterator != entities_.end()) {
        continue;
      }

      float dice = random->getFloat(.0f, 1.0f);
      if (dice <= 0.7f) {
        auto health_potion_factory = std::make_unique<HealthPotionFactory>();
        Entity* entity = health_potion_factory->create({x,y});
        spawn(std::move(entity));
      } else if (dice <= .8f) {
        auto lighting_scroll_factory = std::make_unique<LightningScrollFactory>();
        Entity* entity = lighting_scroll_factory->create({x,y});
        spawn(std::move(entity));
      } else if (dice <= .9f) {
        auto fireball_scroll_factory = std::make_unique<FireballScrollFactory>();
        Entity* entity = fireball_scroll_factory->create({x,y});
        spawn(std::move(entity));
      } else if (dice <= 1.0f) {
        auto confusion_scroll_factory =
          std::make_unique<ConfusionScrollFactory>();
        Entity* entity = confusion_scroll_factory->create({x,y});
        spawn(std::move(entity));
      }
    }
  }

  Entity* EntityManager::spawn(Entity* src) {
    return entities_.emplace_back(src);
  }

  std::vector<std::reference_wrapper<Entity>> EntityManager::get_entities_at(
      SupaRL::Vector2D position) {
    std::vector<std::reference_wrapper<Entity>> entities_at_position;
    // At max there can be 3? things at a position.
    // Corpse, Item, Actor...
    entities_at_position.reserve(3);
    for (auto& entity : entities_) {
      auto entity_position = g_coordinator.get_component<SupaRL::TransformComponent>(
          entity->get_id()).position_;
      if (entity_position == position) {
        entities_at_position.push_back(*entity);
      }
    }
    entities_at_position.shrink_to_fit();
    return entities_at_position;
  }

  std::optional<std::reference_wrapper<Entity>>
    EntityManager::get_entity(SupaRL::Entity entity_) {
      for( const auto& entity : entities_) {
        const auto& entity_id = entity->get_id();
        if(entity_id == entity_) {
          return std::reference_wrapper<Entity>(*entity);
        }
      }
      return std::nullopt;
    }

  std::optional<std::reference_wrapper<Entity>>
    EntityManager::get_blocking_entity_at(SupaRL::Vector2D position) {
      for (const auto& entity : entities_) {
      auto& entity_position = g_coordinator.get_component<SupaRL::TransformComponent>(
          entity->get_id()).position_;
      auto entity_is_blocking = g_coordinator.get_component<SupaRL::PhysiqueComponent>(
          entity->get_id()).is_blocking_;
        if (entity_is_blocking &&
            entity_position == position) {
          return std::reference_wrapper<Entity>(*entity);
        }
      }
      return std::nullopt;
    }

  std::optional<std::reference_wrapper<Entity>>
    EntityManager::get_non_blocking_entity_at(SupaRL::Vector2D position) {
      for (const auto& entity : entities_) {
        auto& entity_position = g_coordinator.get_component<SupaRL::TransformComponent>(
            entity->get_id()).position_;
        auto entity_is_not_blocking = !g_coordinator.get_component<SupaRL::PhysiqueComponent>(
          entity->get_id()).is_blocking_;
        if (entity_is_not_blocking &&
            entity_position == position) {
          return std::reference_wrapper<Entity>(*entity);
        }
      }
      return std::nullopt;
    }

  void EntityManager::remove(const Entity* entity) {
    const auto condition = [&entity](const Entity* e) { return e == entity; };
    entities_.erase(
        std::begin(std::ranges::remove_if(entities_, condition)),
        std::end(entities_));
  }

  std::optional<std::reference_wrapper<Entity>>
    EntityManager::get_closest_living_monster(
        SupaRL::Vector2D position, float range) const {
      std::optional<std::reference_wrapper<Entity>> closest = std::nullopt;
      float best_distance = 1E6f;
      for (const auto& entity : entities_) {
        auto& defence_component = g_coordinator.get_component<SupaRL::DefenceComponent>(
            entity->get_id());
        const std::optional<std::reference_wrapper<AIComponent>> ai_component =
          entity->get_ai_component();

        if (ai_component.has_value() && defence_component.is_not_dead()) {
          auto entity_position = g_coordinator.get_component<SupaRL::TransformComponent>(
              entity->get_id()).position_;
          float distance = position.distance_to(
              entity_position);
          if (distance < best_distance && (distance <= range || range == 0.0f)) {
            best_distance = distance;
            closest = *entity;
          }
        }
      }
      return closest;
    }
}
