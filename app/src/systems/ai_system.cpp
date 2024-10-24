#include "systems/ai_system.hpp"
#include "entity_manager.hpp"
#include "world.hpp"
#include <core/coordinator.hpp>
#include <components/ai.hpp>
#include <components/transform.hpp>
#include "events/command.hpp"

extern SupaRL::Coordinator g_coordinator;

namespace cpprl {
  // TODO: implement Bresenham line
  bool can_path_to_target(tcod::BresenhamLine& path, World& world) {
    for (const auto [x, y] : path) {
      if (world.get_entities().get_blocking_entity_at({x, y})) {
        return false;
      }
    }

    return true;
  }

  void AISystem::update() {
    for (const auto& entity : entities_) {
      auto& ai_component = g_coordinator.get_component<AIComponent>(
          entity);
      if (ai_component.type_ == AIType::HOSTILE) {
        auto position = g_coordinator.get_component<SupaRL::TransformComponent>(entity).position_;
        // we need to know where the player is.
        if (world_->get_map().is_in_fov(position)) {
          auto& player_position = g_coordinator.get_component<SupaRL::TransformComponent>(player_).position_;
          SupaRL::Vector2D delta = player_position - position;

          int distance = std::max(std::abs(delta.x), std::abs(delta.y));
          if (distance <= 1) {
            auto melee_command = MeleeCommand(*world_, entity, delta);
            melee_command.execute();
          }

          tcod::BresenhamLine path =
            tcod::BresenhamLine({position.x, position.y}, {player_position.x, player_position.y}).without_endpoints();

          if (can_path_to_target(path, *world_)) {
            auto dest = path[0];
            auto destination = SupaRL::Vector2D{dest[0], dest[1]} - position;

            auto action = MovementCommand(*world_, entity, destination);
            action.execute();

            return;
          }

          auto action = NoOpEvent(world_);
          action.execute();
        }
      }
    }
  }


}
