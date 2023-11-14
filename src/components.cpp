#include "components.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <memory>

#include "basic_ai_component.hpp"
#include "combat_system.hpp"
#include "events/command.hpp"
#include "exceptions.hpp"
#include "game_entity.hpp"
#include "globals.hpp"
#include "main.hpp"
#include "state.hpp"
#include "world.hpp"

namespace cpprl {
int DefenseComponent::heal(int amount) {
  if (hp_ == max_hp_) {
    return 0;
  };
  int new_hp = hp_ + amount;
  if (new_hp > max_hp_) {
    new_hp = max_hp_;
  }

  int healed = new_hp - hp_;

  hp_ = new_hp;

  return healed;
}

void DefenseComponent::die(Entity& owner) {
  owner.set_name("corpse of " + owner.get_name());
  owner.set_ascii_component(std::make_unique<ASCIIComponent>("%", RED, -1));
  owner.set_blocking(false);
  owner.set_ai_component(nullptr);
}

// void DefenseComponent::save(TCODZip& zip) {
//   zip.putInt(defense_);
//   zip.putInt(hp_);
//   zip.putInt(max_hp_);
// }

// void DefenseComponent::load(TCODZip& zip) {
//   defense_ = zip.getInt();
//   hp_ = zip.getInt();
//   max_hp_ = zip.getInt();
// }
void DefenseComponent::save(cereal::JSONOutputArchive& archive) {
  archive(defense_, hp_, max_hp_);
}
void DefenseComponent::load(cereal::JSONInputArchive& archive) {
  archive(defense_, hp_, max_hp_);
}

// void AttackComponent::save(TCODZip& zip) { zip.putInt(damage_); }
// void AttackComponent::load(TCODZip& zip) { damage_ = zip.getInt(); }
void AttackComponent::save(cereal::JSONOutputArchive& archive) {
  archive(damage_);
}
void AttackComponent::load(cereal::JSONInputArchive& archive) {
  archive(damage_);
}

// void TransformComponent::save(TCODZip& zip) {
//   zip.putInt(position_.x);
//   zip.putInt(position_.y);
// }

// void TransformComponent::load(TCODZip& zip) {
//   position_.x = zip.getInt();
//   position_.y = zip.getInt();
// }
void TransformComponent::save(cereal::JSONOutputArchive& archive) {
  archive(position_.x, position_.y);
}
void TransformComponent::load(cereal::JSONInputArchive& archive) {
  archive(position_.x, position_.y);
}

// void ASCIIComponent::save(TCODZip& zip) {
//   zip.putString(symbol_.c_str());
//   zip.putInt(colour_.r);
//   zip.putInt(colour_.g);
//   zip.putInt(colour_.b);
//   zip.putInt(layer_);
// }

// void ASCIIComponent::load(TCODZip& zip) {
//   symbol_ = zip.getString();
//   colour_.r = zip.getInt();
//   colour_.g = zip.getInt();
//   colour_.b = zip.getInt();
//   layer_ = zip.getInt();
// }

void ASCIIComponent::save(cereal::JSONOutputArchive& archive) {
  archive(symbol_, colour_, layer_);
}
void ASCIIComponent::load(cereal::JSONInputArchive& archive) {
  archive(symbol_, colour_, layer_);
}

Container::Container(int size) : size_(size), inventory_({}) {
  inventory_.reserve(size);
}

bool Container::add(Entity* entity) {
  if (size_ > 0 && inventory_.size() >= size_) {
    return false;
  }
  inventory_.push_back(entity);
  return true;
}

void Container::remove(Entity* entityToRemove) {
  inventory_.erase(
      std::remove_if(
          inventory_.begin(),
          inventory_.end(),
          [&entityToRemove](const Entity* entity) {
            return entity == entityToRemove;
          }),
      inventory_.end());
}

// void Container::load(TCODZip& zip) {
//   size_ = zip.getInt();
//   int nbActors = zip.getInt();
//   while (nbActors > 0) {
//     Entity* entity = new Entity("", false, nullptr, nullptr);
//     entity->load(zip);
//     inventory_.emplace_back(entity);
//     nbActors--;
//   }
// }

// void Container::save(TCODZip& zip) {
//   zip.putInt(size_);
//   zip.putInt(inventory_.size());
//   for (Entity* entity : inventory_) {
//     entity->save(zip);
//   }
// }

void Container::load(cereal::JSONInputArchive& archive) {
  archive(size_);
  int nbActors;
  archive(nbActors);
  while (nbActors > 0) {
    Entity* entity = new Entity("", false, nullptr, nullptr);
    entity->load(archive);
    inventory_.emplace_back(entity);
    nbActors--;
  }
}

void Container::save(cereal::JSONOutputArchive& archive) {
  archive(size_);
  archive(inventory_.size());
  for (Entity* entity : inventory_) {
    entity->save(archive);
  }
}

ActionResult ConsumableComponent::pick_up(Entity* owner, Entity* wearer) {
  auto* container = &wearer->get_container();
  if (container && container->add(owner)) {
    // remove the owner?
    return Success{};
  }
  return Failure{"There's nothing to pick up."};
}

ActionResult ConsumableComponent::drop(Entity* owner, Entity* wearer) {
  auto* container = &wearer->get_container();
  if (container) {
    container->remove(owner);
    owner->get_transform_component().move(
        wearer->get_transform_component().get_position());
    return Success{};
  }
  return Failure{};
}

ActionResult ConsumableComponent::use(Entity* owner, Entity* wearer, World&) {
  auto* container = &wearer->get_container();
  if (container) {
    container->remove(owner);
    return Success{};
  }
  return Failure{""};
}

std::unique_ptr<ConsumableComponent> ConsumableComponent::create(
    cereal::JSONInputArchive& archive) {
  // ConsumableType type = (ConsumableType)zip.getInt();
  ConsumableType type;
  archive(type);
  std::unique_ptr<ConsumableComponent> component = nullptr;
  switch (type) {
    case HEALER:
      component = std::make_unique<HealingConsumable>(0);
      break;
    case LIGHTNING_BOLT:
      component = std::make_unique<LightningBolt>(0, 0);
      break;
    case CONFUSER:
      component = std::make_unique<ConfusionSpell>(0, 0);
      break;
    case FIREBALL:
      component = std::make_unique<FireSpell>(0, 0, 0);
      break;
  }
  component->load(archive);
  return component;
}

HealingConsumable::HealingConsumable(int amount) : amount_(amount){};
// void HealingConsumable::save(TCODZip& zip) {
//   zip.putInt(HEALER);
//   zip.putInt(amount_);
// }
// void HealingConsumable::load(TCODZip& zip) { amount_ = zip.getInt(); }

void HealingConsumable::save(cereal::JSONOutputArchive& archive) {
  archive(HEALER, amount_);
}
void HealingConsumable::load(cereal::JSONInputArchive& archive) {
  archive(amount_);
}

ActionResult HealingConsumable::use(
    Entity* owner, Entity* wearer, World& world) {
  DefenseComponent* defense_component = &wearer->get_defense_component();
  if (defense_component == nullptr) {
    return Failure{"There's nothing to heal."};
  }

  int amount_healed = wearer->get_defense_component().heal(amount_);
  if (amount_healed > 0) {
    ConsumableComponent::use(owner, wearer, world);
    std::string message = fmt::format("You healed for {} HP.", amount_healed);
    world.get_message_log().add_message(message, GREEN);
    return Success{};
  }

  return Failure{"You are already at full health."};
}

ActionResult LightningBolt::use(Entity* owner, Entity* wearer, World& world) {
  Entity* closest_monster = nullptr;
  closest_monster = world.get_entities().get_closest_living_monster(
      wearer->get_transform_component().get_position(), range_);
  if (!closest_monster) {
    return Failure{"No enemy is close enough to strike."};
  }
  // closest_monster->get_defense_component()->take_damage(damage_);
  int inflicted = combat_system::handle_spell(damage_, *closest_monster);
  ConsumableComponent::use(owner, wearer, world);
  if (inflicted > 0) {
    world.get_message_log().add_message(
        fmt::format(
            "A lightning bolt strikes the {} with a loud "
            "thunder! The damage is {} hit points.",
            closest_monster->get_name(),
            damage_),
        GREEN);

    if (closest_monster->get_defense_component().is_dead()) {
      auto action = DieEvent(world, closest_monster);
      action.execute();
    }
    return Success{};
  } else {
    return Failure{fmt::format(
        "The lightning bolt hits the {} but does no damage.",
        closest_monster->get_name())};
  }
}
// void LightningBolt::save(TCODZip& zip) {
//   zip.putInt(LIGHTNING_BOLT);
//   zip.putInt(range_);
//   zip.putInt(damage_);
// }

// void LightningBolt::load(TCODZip& zip) {
//   range_ = zip.getInt();
//   damage_ = zip.getInt();
// }

void LightningBolt::save(cereal::JSONOutputArchive& archive) {
  archive(LIGHTNING_BOLT, range_, damage_);
}
void LightningBolt::load(cereal::JSONInputArchive& archive) {
  archive(range_, damage_);
}

ActionResult FireSpell::use(Entity* owner, Entity* wearer, World& world) {
  // We want everything by reference, except the pointers which
  // we need by value. I need the memory address of the owner and
  // wearer in order to update them and have it persist beyond the lambda.
  auto on_pick = [&, owner, wearer]() {
    // TODO:: when I get here the pointers are garbage.
    ConsumableComponent::use(owner, wearer, world);
    for (Entity* entity : world.get_entities()) {
      auto* defense_component = &entity->get_defense_component();
      if (defense_component && defense_component->is_not_dead() &&
          entity->get_transform_component().get_position().distance_to(
              world.get_map().get_highlight_tile()) <= max_range_) {
        world.get_message_log().add_message(
            fmt::format(
                "The {} gets burned for {} hit points.",
                entity->get_name(),
                damage_),
            RED);
        int inflicted = combat_system::handle_spell(damage_, *entity);
        if (inflicted > 0) {
          // TODO: this is repeated everywhere. Put it in take_damage
          if (entity->get_defense_component().is_dead()) {
            auto action = DieEvent(world, entity);
            action.execute();
          }
        }
      }
    }
  };
  // world.set_targeting_tile(max_range_, on_pick);
  return Poll{
      std::make_unique<PickTileAOEState>(world, on_pick, max_range_, aoe_)};
}
// void FireSpell::save(TCODZip& zip) {
//   zip.putInt(FIREBALL);
//   zip.putInt(max_range_);
//   zip.putInt(aoe_);
//   zip.putInt(damage_);
// }
// void FireSpell::load(TCODZip& zip) {
//   max_range_ = zip.getInt();
//   aoe_ = zip.getInt();
//   damage_ = zip.getInt();
// }
void FireSpell::save(cereal::JSONOutputArchive& archive) {
  archive(FIREBALL, max_range_, aoe_, damage_);
}
void FireSpell::load(cereal::JSONInputArchive& archive) {
  archive(max_range_, aoe_, damage_);
}

ActionResult ConfusionSpell::use(Entity* owner, Entity* wearer, World& world) {
  auto on_pick = [&, owner, wearer]() {
    Entity* target = world.get_entities().get_blocking_entity_at(
        world.get_map().get_highlight_tile());
    if (target) {
      std::unique_ptr<AIComponent> old_ai = target->transfer_ai_component();

      std::unique_ptr<AIComponent> confusion_ai =
          std::make_unique<ConfusionAI>(num_turns_, std::move(old_ai));
      target->set_ai_component(std::move(confusion_ai));
      world.get_message_log().add_message(
          fmt::format(
              "The eyes of the {} look vacant, as it starts to "
              "stumble around!",
              target->get_name()),
          GREEN);
      ConsumableComponent::use(owner, wearer, world);
    } else {
      throw Impossible("There is no enemy at that location.");
    }
  };
  return Poll{std::make_unique<PickTileState>(world, on_pick, max_range_)};
}
// void ConfusionSpell::save(TCODZip& zip) {
//   zip.putInt(CONFUSER);
//   zip.putInt(num_turns_);
//   zip.putInt(max_range_);
// }

// void ConfusionSpell::load(TCODZip& zip) {
//   num_turns_ = zip.getInt();
//   max_range_ = zip.getInt();
// }
void ConfusionSpell::save(cereal::JSONOutputArchive& archive) {
  archive(CONFUSER, num_turns_, max_range_);
}
void ConfusionSpell::load(cereal::JSONInputArchive& archive) {
  archive(num_turns_, max_range_);
}
}  // namespace cpprl
