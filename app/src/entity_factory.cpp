#include "entity_factory.hpp"

#include "basic_ai_component.hpp"
#include "colours.hpp"
#include "components.hpp"
#include "core/coordinator.hpp"
#include "components/defence.hpp"
#include "components/attack.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"
#include "components/ascii.hpp"
#include <components/physique.hpp>
#include <components/identity.hpp>
#include <components/status_condition.hpp>

extern SupaRL::Coordinator g_coordinator;

namespace cpprl {
  Entity* AbstractEntityFactory::create_base(
      const std::string& name, tcod::ColorRGB color, std::string symbol) {

    auto entity = new Entity();
    SupaRL::Entity entity_id = g_coordinator.create_entity();
    entity->set_id(entity_id);
    g_coordinator.add_component(entity_id, SupaRL::AsciiComponent{
        .symbol_ = symbol,
        .colour_ = SupaRL::ColourRGB{.r = color.r, .g = color.g, .b = color.b },
        .layer_ = 1});

    g_coordinator.add_component(entity_id, SupaRL::IdentityComponent{
        .name_ = name});
    g_coordinator.add_component(entity_id, SupaRL::PhysiqueComponent{
        .is_blocking_ = true});
    g_coordinator.add_component(entity_id, SupaRL::StatusConditionComponent{
        // Should this be an array of status conditions?
        .damage_per_tick_ = 0,
        .max_ticks_ = 0,
        .ticks_ = 0,
        .name_ = "none"});
    return entity;
  }

  Entity* OrcFactory::create(SupaRL::Vector2D at_position) {
    Entity* entity = create_base("Orc", DARK_GREEN, "o");
    auto entity_id = entity->get_id();

    entity->set_ai_component(std::make_unique<HostileAI>());
    entity->set_stats_component(
        std::make_unique<StatsComponent>(10, 1, 10, 10, 2));

    g_coordinator,add_component(entity_id, SupaRL::AttackComponent{
        .damage_ = 3});
    g_coordinator.add_component(entity_id, SupaRL::DefenceComponent{
        .defence_ = 0,
        .hp_ = 10,
        .max_hp_ = 10});
    g_coordinator.add_component(entity_id, SupaRL::AttackComponent{
        .damage_ = 3});
    g_coordinator.add_component(entity_id, SupaRL::TransformComponent{
        .position_ = at_position});
    g_coordinator.add_component(
        entity_id, SupaRL::VelocityComponent{
        .velocity_ = {0,0}});

    return entity;
  }

  Entity* TrollFactory::create(SupaRL::Vector2D at_position) {
    Entity* entity = create_base("Troll", DARK_GREEN, "T");

    entity->set_attack_component(std::make_unique<AttackComponent>(4));
    entity->set_ai_component(std::make_unique<HostileAI>());
    entity->set_stats_component(
        std::make_unique<StatsComponent>(20, 1, 10, 20, 2));

    auto entity_id = entity->get_id();

    g_coordinator.add_component(entity_id, SupaRL::AttackComponent{
        .damage_ = 4});
    g_coordinator.add_component(entity_id, SupaRL::DefenceComponent{
        .defence_ = 1,
        .hp_ = 16,
        .max_hp_ = 16});
    g_coordinator.add_component(entity_id, SupaRL::AttackComponent{
        .damage_ = 4});
    g_coordinator.add_component(entity_id, SupaRL::TransformComponent{
        .position_ = at_position});
    g_coordinator.add_component(
        entity_id, SupaRL::VelocityComponent{
        .velocity_ = {0,0}});

    return entity;
  }

  Entity* PlayerFactory::create(SupaRL::Vector2D at_position) {
    Entity* entity = create_base("Player", TEAL, "@");

    entity->set_container(std::make_unique<Container>(26));
    entity->set_stats_component(
        std::make_unique<StatsComponent>(0, 1, 150, 150, 2));

    auto entity_id = entity->get_id();

    g_coordinator.add_component(entity_id, SupaRL::AttackComponent{
        .damage_ = 5});
    g_coordinator.add_component(entity_id, SupaRL::DefenceComponent{
        .defence_ = 2,
        .hp_ = 30,
        .max_hp_ = 30});
    g_coordinator.add_component(entity_id, SupaRL::AttackComponent{
        .damage_ = 5});
    g_coordinator.add_component(entity_id, SupaRL::TransformComponent{
        .position_ = at_position});
    g_coordinator.add_component(
        entity_id, SupaRL::VelocityComponent{
        .velocity_ = {0,0}});

    return entity;
  }
}
