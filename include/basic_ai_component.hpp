#ifndef INCLUDE_BASIC_AI_COMPONENT_HPP_
#define INCLUDE_BASIC_AI_COMPONENT_HPP_

namespace cpprl {
class Engine;
class GameEntity;

class BasicAIComponent {
 public:
  virtual ~BasicAIComponent() = default;
  virtual void update(Engine& engine) = 0;
};

class BaseBasicAI : BasicAIComponent {
 public:
  BaseBasicAI(GameEntity& entity) : entity_(entity) {}
  virtual ~BaseBasicAI() = default;
  virtual void update(Engine& engine) = 0;

 protected:
  GameEntity& entity_;
};

class HostileAI : BaseBasicAI {
 public:
  HostileAI(GameEntity& entity) : BaseBasicAI(entity) {}
  virtual ~HostileAI() = default;
  virtual void update(Engine& engine) override;
};
}  // namespace cpprl

#endif
