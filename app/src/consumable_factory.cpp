#include "consumable_factory.hpp"

#include <memory>

#include "components.hpp"

namespace cpprl {
std::unique_ptr<Entity> AbstractConsumableFactory::create_base(
    std::string name, tcod::ColorRGB color, std::string_view symbol) {
  return std::make_unique<Entity>(
      name,
      false,
      std::make_unique<TransformComponent>(0, 0),
      std::make_unique<ASCIIComponent>(symbol, color, 1));
}

std::unique_ptr<Entity> HealthPotionFactory::create() {
  auto entity = create_base("Health Potion", RED, "!");
  entity->set_consumable_component(std::make_unique<HealingConsumable>(10));
  return entity;
}

std::unique_ptr<Entity> LightningScrollFactory::create() {
  auto entity = create_base("Lightning Scroll", DARK_RED, "#");
  entity->set_consumable_component(std::make_unique<LightningBolt>(5, 20));
  return entity;
}

std::unique_ptr<Entity> FireballScrollFactory::create() {
  auto entity = create_base("Fireball Scroll", DARK_RED, "#");
  entity->set_consumable_component(std::make_unique<FireSpell>(3, 5, 20));
  return entity;
}

std::unique_ptr<Entity> ConfusionScrollFactory::create() {
  auto entity = create_base("Confusion Scroll", DARK_RED, "#");
  entity->set_consumable_component(std::make_unique<ConfusionSpell>(5, 0));
  return entity;
}
}  // namespace cpprl
