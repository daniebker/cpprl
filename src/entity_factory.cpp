#include "entity_factory.hpp"

#include "basic_ai_component.hpp"
#include "colours.hpp"
#include "components.hpp"

namespace cpprl {
Entity* OrcFactory::create() {
  Entity* entity = new Entity(
      "Orc",
      true,
      std::make_unique<TransformComponent>(0, 0),
      std::make_unique<ASCIIComponent>("o", DARK_GREEN, 1));

  entity->set_attack_component(std::make_unique<AttackComponent>(3));
  entity->set_defense_component(std::make_unique<DefenseComponent>(0, 10));
  entity->set_ai_component(std::make_unique<HostileAI>());

  return entity;
}

Entity* TrollFactory::create() {
  Entity* entity = new Entity(
      "Troll",
      true,
      std::make_unique<TransformComponent>(0, 0),
      std::make_unique<ASCIIComponent>("T", DARK_GREEN, 1));

  entity->set_attack_component(std::make_unique<AttackComponent>(4));
  entity->set_defense_component(std::make_unique<DefenseComponent>(1, 16));
  entity->set_ai_component(std::make_unique<HostileAI>());

  return entity;
}

Entity* PlayerFactory::create() {
  Entity* entity = new Entity(
      "player",
      true,
      std::make_unique<TransformComponent>(0, 0),
      std::make_unique<ASCIIComponent>("@", TEAL, 1));

  entity->set_attack_component(std::make_unique<AttackComponent>(5));
  entity->set_defense_component(std::make_unique<DefenseComponent>(2, 30));
  entity->set_container(std::make_unique<Container>(26));

  return entity;
}
}  // namespace cpprl
