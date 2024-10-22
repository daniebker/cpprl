#include "systems/enemy_turns.hpp"
#include "core/coordinator.hpp"
#include "components/defence.hpp"

extern SupaRL::Coordinator g_coordinator;

void EnemyTurnsSystem::update() {
  for (const auto& entity : entities_) {
    // so this was used to get all the moveable entities
    // that were controlled by the engine. We'll need to create a new
    // ai component so that this system runs on only things that have an AI
    // component and a defence component.
      /*const std::optional<std::reference_wrapper<AIComponent>> ai_component =*/
      /*  entity->get_ai_component();*/
      auto& defence_component = g_coordinator.get_component<SupaRL::DefenceComponent>(
          entity->get_id());
      if (defence_component.is_not_dead()) {
        entity->update(*this);
      }
    }
}
