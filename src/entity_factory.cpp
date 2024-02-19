#include "entity_factory.hpp"

#include "basic_ai_component.hpp"
#include "colours.hpp"
#include "components.hpp"

namespace cpprl {
Entity* AbstractEntityFactory::create_base(
    const std::string& name, tcod::ColorRGB color, std::string_view symbol) {
  return new Entity(
      name,
      true,
      std::make_unique<TransformComponent>(0, 0),
      std::make_unique<ASCIIComponent>(symbol, color, 1));
}

Entity* OrcFactory::create() {
  Entity* entity = create_base("Orc", DARK_GREEN, "o");

  entity->set_attack_component(std::make_unique<AttackComponent>(3));
  entity->set_defense_component(std::make_unique<DefenseComponent>(0, 10));
  entity->set_ai_component(std::make_unique<HostileAI>());
  entity->set_stats_component(std::make_unique<StatsComponent>(10, 1, 10, 10));

  return entity;
}

Entity* TrollFactory::create() {
  Entity* entity = create_base("Troll", DARK_GREEN, "T");

  entity->set_attack_component(std::make_unique<AttackComponent>(4));
  entity->set_defense_component(std::make_unique<DefenseComponent>(1, 16));
  entity->set_ai_component(std::make_unique<HostileAI>());
  entity->set_stats_component(std::make_unique<StatsComponent>(20, 1, 10, 20));

  return entity;
}

Entity* PlayerFactory::create() {
  Entity* entity = create_base("Player", TEAL, "@");

  entity->set_attack_component(std::make_unique<AttackComponent>(5));
  entity->set_defense_component(std::make_unique<DefenseComponent>(2, 30));
  entity->set_container(std::make_unique<Container>(26));
  entity->set_stats_component(std::make_unique<StatsComponent>(0, 1, 150, 2));

  return entity;
}
}  // namespace cpprl
