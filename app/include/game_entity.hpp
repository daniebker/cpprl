#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <memory>
#include <libtcod.hpp>

#include "types/world_fwd.hpp"
#include <core/types.hpp>
#include <core/coordinator.hpp>
#include <components/transform.hpp>

extern SupaRL::Coordinator g_coordinator;

namespace cpprl {

  class ConsumableComponent;
  class StatsComponent;
  class Container;

  class Entity {
    private:
      SupaRL::Entity id_;

      std::unique_ptr<ConsumableComponent> consumableComponent_;
      std::unique_ptr<Container> container_;
      std::unique_ptr<StatsComponent> statsComponent_;

    public:
      Entity() = default;
      ~Entity() = default;

      void set_id(SupaRL::Entity id) { id_ = id; };
      SupaRL::Entity get_id() const { return id_; };

      ConsumableComponent& get_consumable_component() {
        return *consumableComponent_;
      };

      std::optional<std::reference_wrapper<StatsComponent>> get_stats_component() {
        if (statsComponent_) {
          return std::ref(*statsComponent_);
        } else {
          return std::nullopt;
        }
      };

      Container& get_container() { return *container_; };
      float get_distance_to(Entity* other) const;


      void update(World& world);
      void set_consumable_component(
          std::unique_ptr<ConsumableComponent> consumableComponent);
      void set_container(std::unique_ptr<Container> container);
      void set_stats_component(std::unique_ptr<StatsComponent> aiComponent);

      template <class Archive>
        void pack(Archive& archive) {
          archive(consumableComponent_ != nullptr);
          archive(container_ != nullptr);
          archive(statsComponent_ != nullptr);

          if (consumableComponent_) archive(consumableComponent_);
          if (container_) archive(container_);
          if (statsComponent_) archive(statsComponent_);
        }

      template <class Archive>
        void unpack(Archive& archive) {
          bool hasConsumableComponent;
          bool hasContainer;
          bool hasStatsComponent;

          archive(hasConsumableComponent);
          archive(hasContainer);
          archive(hasStatsComponent);

          if (hasConsumableComponent) {
            archive(consumableComponent_);
          }
          if (hasContainer) {
            archive(container_);
          }
          if (hasStatsComponent) {
            archive(statsComponent_);
          }
        }
  };

}

#endif
