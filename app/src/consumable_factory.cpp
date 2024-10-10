#include "consumable_factory.hpp"

#include <memory>

#include "components.hpp"

extern SupaRL::Coordinator g_coordinator;
namespace cpprl {
  Entity* AbstractConsumableFactory::create_base(
      std::string name, tcod::ColorRGB color, std::string_view symbol) {
    Entity* new_entity = new Entity(
        name,
        false,
        std::make_unique<TransformComponent>(0, 0),
        std::make_unique<ASCIIComponent>(symbol, color, 1));
    auto entity_id = g_coordinator.create_entity();
    new_entity->set_id(entity_id);
    g_coordinator.add_component(entity_id, SupaRL::TransformComponent{
        .position_ = {0, 0}});
    return new_entity;

  }

  Entity* HealthPotionFactory::create() {
    Entity* entity = create_base("Health Potion", RED, "!");
    entity->set_consumable_component(std::make_unique<HealingConsumable>(10));
    return entity;
  }

  Entity* LightningScrollFactory::create() {
    Entity* entity = create_base("Lightning Scroll", DARK_RED, "#");
    entity->set_consumable_component(std::make_unique<LightningBolt>(5, 20));
    return entity;
  }

  Entity* FireballScrollFactory::create() {
    Entity* entity = create_base("Fireball Scroll", DARK_RED, "#");
    entity->set_consumable_component(std::make_unique<FireSpell>(3, 5, 20));
    return entity;
  }

  Entity* ConfusionScrollFactory::create() {
    Entity* entity = create_base("Confusion Scroll", DARK_RED, "#");
    entity->set_consumable_component(std::make_unique<ConfusionSpell>(5, 0));
    return entity;
  }
}
