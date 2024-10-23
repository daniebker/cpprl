#include "basic_ai_component.hpp"

#include <algorithm>
#include <libtcod.hpp>

#include "events/command.hpp"
#include "game_entity.hpp"
#include "entity_manager.hpp"
#include "world.hpp"
#include <core/math.hpp>

extern SupaRL::Coordinator g_coordinator;
namespace cpprl {

bool can_path_to_target(tcod::BresenhamLine& path, World& world) {
  for (const auto [x, y] : path) {
    if (world.get_entities().get_blocking_entity_at({x, y})) {
      return false;
    }
  }

  return true;
}

void HostileAI::update(World& world, Entity* entity) {
}

ConfusionAI::ConfusionAI(int num_turns, std::unique_ptr<AIComponent> old_ai)
    : num_turns_(num_turns), old_ai_(std::move(old_ai)) {}
void ConfusionAI::update(World& world, Entity* entity) {
  TCODRandom* random = TCODRandom::getInstance();
  int dx = random->getInt(-1, 1);
  int dy = random->getInt(-1, 1);
  if ((dx != 0 || dy != 0) && num_turns_ > 0) {
    auto action = DirectionalCommand(world, entity, {dx, dy});
    action.execute();
    --num_turns_;
  } else {
    entity->set_ai_component(std::move(old_ai_));
  }
}
}  // namespace cpprl
