#include "systems/death_system.hpp"

#include "core/coordinator.hpp"
#include "core/colour.hpp"
#include "components/identity.hpp"
#include "components/physique.hpp"
#include "components/ascii.hpp"
#include "components/defence.hpp"

extern SupaRL::Coordinator g_coordinator;

namespace SupaRL {

  void DeathSystem::update() {
    for(auto& entity : entities_) {
      auto& defence_component = g_coordinator.get_component<SupaRL::DefenceComponent>(
          entity);

      if(defence_component.hp_ > 0) {
        continue;
      }

      auto& identity_comp = g_coordinator.get_component<SupaRL::IdentityComponent>(
          entity);
      identity_comp.name_ = "Corpse of " + identity_comp.name_;

      auto& ascii_comp = g_coordinator.get_component<SupaRL::AsciiComponent>(
          entity);
      ascii_comp.symbol_ = "%";
      ascii_comp.colour_ = RED;
      ascii_comp.layer_ = -1;

      auto& physique_component = g_coordinator.get_component<SupaRL::PhysiqueComponent>(
          entity);

      physique_component.is_blocking_ = false;
      // TODO: Move the AI component to ECS and remove it here
      /*g_coordinator.remove_component<SupaRL::AIComponent>(entity);*/
    }
  }
}

