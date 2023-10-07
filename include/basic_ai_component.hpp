#ifndef INCLUDE_BASIC_AI_COMPONENT_HPP_
#define INCLUDE_BASIC_AI_COMPONENT_HPP_

namespace cpprl {
class Engine;
class Entity;

class AIComponent {
 public:
  AIComponent(){};
  virtual ~AIComponent() = default;
  virtual void update(Engine& engine, Entity* entity) = 0;
};

class HostileAI final : public AIComponent {
 public:
  HostileAI() : AIComponent(){};
  void update(Engine& engine, Entity* entity) override;
};
}  // namespace cpprl

#endif
