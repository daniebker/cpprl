#include "systems/physics_system.hpp"

#include "core/coordinator.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"

extern SupaRL::Coordinator g_coordinator;

namespace SupaRL {
  void PhysicsSystem::init() {
  }

  void PhysicsSystem::update() {
    auto entities_to_remove = std::vector<Entity>();
    for (auto const& entity : entities_) {
      auto& transform = g_coordinator.get_component<TransformComponent>(entity);
      auto& velocity = g_coordinator.get_component<VelocityComponent>(entity);

      transform.position_.x += velocity.velocity_.x;
      transform.position_.y += velocity.velocity_.y;

      velocity.velocity_.x = 0;
      velocity.velocity_.y = 0;
    }
  }
}
