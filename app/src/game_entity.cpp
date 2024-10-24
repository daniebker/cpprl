#include "game_entity.hpp"

#include <libtcod.hpp>

#include "components.hpp"
#include "world.hpp"

namespace cpprl {
  void Entity::set_consumable_component(
      std::unique_ptr<ConsumableComponent> consumableComponent) {
    consumableComponent_ = std::move(consumableComponent);
  };

  void Entity::set_stats_component(
      std::unique_ptr<StatsComponent> statsComponent) {
    statsComponent_ = std::move(statsComponent);
  };

  void Entity::set_container(std::unique_ptr<Container> container) {
    container_ = std::move(container);
  };
}
