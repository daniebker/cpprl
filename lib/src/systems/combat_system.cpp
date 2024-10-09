#include "systems/combat_system.hpp"
#include <iostream>

namespace SupaRL
{
  void CombatSystem::init()
  {
    std::cout << "Combat System Init" << std::endl;
  }

  void CombatSystem::update()
  {
    std::cout << "Combat System Update" << std::endl;
  }
}
