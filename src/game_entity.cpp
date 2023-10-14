#include "game_entity.hpp"

#include <libtcod.hpp>
#include <string_view>

#include "basic_ai_component.hpp"

namespace cpprl {

void Entity::update(Engine& engine) { aiComponent_->update(engine, this); }

}  // namespace cpprl
