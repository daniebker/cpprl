#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <core/entity_manager.hpp>
#include <core/math.hpp>

#include "entity_factory.hpp"
#include "game_entity.hpp"
#include "rectangular_room.hpp"

namespace cpprl {

  typedef std::optional<std::reference_wrapper<Entity>> OptEntityRef;

  class EntityManager {
    public:
      EntityManager() {
        orc_factory_ = std::make_unique<OrcFactory>();
        troll_factory_ = std::make_unique<TrollFactory>();
        entity_manager_ = SupaRL::EntityManager();
      };

      EntityManager(
          std::unique_ptr<AbstractEntityFactory> orc_factory,
          std::unique_ptr<AbstractEntityFactory> troll_factory)
        : orc_factory_(std::move(orc_factory)),
        troll_factory_(std::move(troll_factory)){};
      void clear();
      void clear_except_player();
      std::optional<std::reference_wrapper<Entity>> get_blocking_entity_at(
          SupaRL::Vector2D position);
      std::optional<std::reference_wrapper<Entity>> get_non_blocking_entity_at(
          SupaRL::Vector2D position);
      std::optional<std::reference_wrapper<Entity>> get_closest_living_monster(
          SupaRL::Vector2D position, float range) const;
      std::vector<std::reference_wrapper<Entity>> get_entities_at(
          SupaRL::Vector2D position);
      void place_entities(
          RectangularRoom room, int max_monsters_per_room, int max_items_per_room);
      Entity* spawn(Entity* entity);
      Entity* spawn(Entity* entity, SupaRL::Vector2D position);
      void reserve(size_t size) { entities_.reserve(size); }
      void shrink_to_fit() { entities_.shrink_to_fit(); }
      void remove(const Entity* entity);
      size_t size() const { return entities_.size(); }

      using iterator = std::vector<Entity*>::iterator;
      using const_iterator = std::vector<Entity*>::const_iterator;
      // Iterator for EntityManager

      iterator begin() { return entities_.begin(); }

      iterator end() { return entities_.end(); }

      const_iterator begin() const { return entities_.begin(); }

      const_iterator end() const { return entities_.end(); }

      template <class Archive>
        void save(Archive& archive) const {
          archive(entities_.size() - 1);
          for (auto& entity : entities_) {
            if (entity->get_name() == "Player") {
              continue;
            }
            entity->pack(archive);
          }
        }

      template <class Archive>
        void load(Archive& archive) {
          size_t size;
          archive(size);
          entities_.reserve(size);
          for (size_t i = 0; i < size; i++) {
            auto entity = new Entity("", false, nullptr, nullptr);
            entity->unpack(archive);
            entities_.emplace_back(entity);
          }
        }

    private:
      std::vector<Entity*> entities_;
      SupaRL::EntityManager entity_manager_;
      std::unique_ptr<AbstractEntityFactory> orc_factory_;
      std::unique_ptr<AbstractEntityFactory> troll_factory_;
  };
}
