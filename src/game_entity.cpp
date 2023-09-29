#include "../include/game_entity.hpp"

#include <libtcod.hpp>
#include <string_view>

#include "../include/basic_ai_component.hpp"

namespace cpprl {
GameEntity::GameEntity(
    std::string name,
    Vector2D position,
    std::string_view symbol,
    const tcod::ColorRGB& colour,
    bool blocker,
    AttackComponent attackComponent,
    DefenseComponent defenseComponent)
    : name_(name),
      position_(position),
      symbol_(symbol),
      colour_(colour),
      blocker_(blocker),
      attackComponent_(attackComponent),
      defenseComponent_(defenseComponent) {}

void GameEntity::move(cpprl::Vector2D& vector2D) { position_ = vector2D; }

void GameEntity::act(Engine& engine) {
  if (name_ != "player") {
    auto ai = HostileAI(*this);
    ai.update(engine);
  }
}

void GameEntity::die() {
  blocker_ = false;
  symbol_ = "%";
  colour_ = DARK_RED;
}
}  // namespace cpprl
