#include "systems/status_condition_system.hpp"
#include "core/coordinator.hpp"
#include "components/defence.hpp"
#include "components/status_condition.hpp"
#include <iostream>

extern SupaRL::Coordinator g_coordinator;

namespace SupaRL
{

  void StatusConditionSystem::init()
  {
  }

  void StatusConditionSystem::update()
  {
    for (auto const& entity : entities_)
    {
      auto& status_condition = g_coordinator.get_component<StatusConditionComponent>(entity);
      auto& defence = g_coordinator.get_component<DefenceComponent>(entity);


      std::cout << "Status condition: " << status_condition.name_ <<  std::endl;


      status_condition.ticks_++;
      if (status_condition.ticks_ <= status_condition.max_ticks_)
      {
        std::cout << "Ticks: " << status_condition.ticks_ << std::endl;

        std::cout << "HP: " << defence.hp_ << std::endl;
        defence.hp_ -= status_condition.damage_per_tick_;
        std::cout << "HP: " << defence.hp_ << std::endl;
      }
    }
  }
}
