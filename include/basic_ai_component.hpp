#ifndef INCLUDE_BASIC_AI_COMPONENT_HPP_
#define INCLUDE_BASIC_AI_COMPONENT_HPP_

#include "types/engine_fwd.hpp"
#include "types/entity_fwd.hpp"
#include "types/world_fwd.hpp"

namespace cpprl {

class AIComponent {
 public:
  AIComponent(){};
  virtual ~AIComponent() = default;
  virtual void update(World& world, Entity* entity) = 0;
};

class HostileAI final : public AIComponent {
 public:
  HostileAI() : AIComponent(){};
  void update(World& world, Entity* entity) override;
};
}  // namespace cpprl

#endif
