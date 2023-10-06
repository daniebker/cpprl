#include "../include/game_entity.hpp"

#include <libtcod.hpp>
#include <string_view>

#include "../include/basic_ai_component.hpp"

namespace cpprl {
GameEntity::GameEntity(
    std::string name,
    bool blocker,
    TransformComponent transformComponent,
    SpriteComponent spriteComponent,
    AttackComponent attackComponent,
    DefenseComponent defenseComponent)
    : name_(name),
      transformComponent_(transformComponent),
      spriteComponent_(spriteComponent),
      blocker_(blocker),
      attackComponent_(attackComponent),
      defenseComponent_(defenseComponent) {}

void GameEntity::move(cpprl::Vector2D& vector2D) { set_position(vector2D); }

void GameEntity::act(Engine& engine) {
  if (name_ != "player") {
    auto ai = HostileAI(*this);
    ai.update(engine);
  }
}

void GameEntity::die() {
  blocker_ = false;
  spriteComponent_.symbol = "%";
  spriteComponent_.colour = DARK_RED;
}
}  // namespace cpprl
