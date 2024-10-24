#include "consumable_factory.hpp"

#include <memory>

#include "components.hpp"
#include <components/ascii.hpp>
#include <components/transform.hpp>
#include <components/physique.hpp>
#include <components/identity.hpp>

extern SupaRL::Coordinator g_coordinator;
namespace cpprl {
  Entity* AbstractConsumableFactory::create_base(
      std::string name, tcod::ColorRGB color, std::string symbol,SupaRL::Vector2D at_position) {
    Entity* new_entity = new Entity();
    auto entity_id = g_coordinator.create_entity();
    new_entity->set_id(entity_id);
    g_coordinator.add_component(entity_id, SupaRL::TransformComponent{
        .position_ = at_position});
    g_coordinator.add_component(entity_id, SupaRL::AsciiComponent{
        .symbol_ = symbol,
        .colour_ = SupaRL::ColourRGB{.r = color.r, .g = color.g, .b = color.b},
        .layer_ = 1});
    g_coordinator.add_component(entity_id, SupaRL::IdentityComponent{
        .name_ = name});
    g_coordinator.add_component(entity_id, SupaRL::PhysiqueComponent{
        .is_blocking_ = false});
    return new_entity;

  }

  Entity* HealthPotionFactory::create(SupaRL::Vector2D at_position) {
    Entity* entity = create_base("Health Potion", RED, "!", at_position);
    entity->set_consumable_component(std::make_unique<HealingConsumable>(10));
    return entity;
  }

  Entity* LightningScrollFactory::create(SupaRL::Vector2D at_position) {
    Entity* entity = create_base("Lightning Scroll", DARK_RED, "#", at_position);
    entity->set_consumable_component(std::make_unique<LightningBolt>(5, 20));
    return entity;
  }

  Entity* FireballScrollFactory::create(SupaRL::Vector2D at_position) {
    Entity* entity = create_base("Fireball Scroll", DARK_RED, "#", at_position);
    entity->set_consumable_component(std::make_unique<FireSpell>(3, 5, 20));
    return entity;
  }

  Entity* ConfusionScrollFactory::create(SupaRL::Vector2D at_position) {
    Entity* entity = create_base("Confusion Scroll", DARK_RED, "#", at_position);
    entity->set_consumable_component(std::make_unique<ConfusionSpell>(5, 0));
    return entity;
  }
}
