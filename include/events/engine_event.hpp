#ifndef ENGINE_EVENT_HPP
#define ENGINE_EVENT_HPP

#include "types/math.hpp"
#include "types/state_result.hpp"

namespace cpprl {
class Engine;
class EngineEvent {
 protected:
  Engine& engine_;

 public:
  EngineEvent(Engine& engine) : engine_(engine) {}
  virtual ~EngineEvent() {}
  virtual CommandResult execute() = 0;
};

class NoOpEvent : public EngineEvent {
 public:
  NoOpEvent(Engine& engine) : EngineEvent(engine) {}
  CommandResult execute() override { return {}; }
};

class ResetGameCommand : public EngineEvent {
 public:
  ResetGameCommand(Engine& engine) : EngineEvent(engine) {}
  CommandResult execute() override { return Reset{}; }
};

class MouseInputEvent final : public EngineEvent {
 private:
  Vector2D position_;

 public:
  MouseInputEvent(Engine& engine, Vector2D position)
      : EngineEvent(engine), position_(position) {}
  CommandResult execute() override;
};

class MouseClickEvent final : public EngineEvent {
 private:
  Vector2D position_;

 public:
  MouseClickEvent(Engine& engine, Vector2D position)
      : EngineEvent(engine), position_(position) {}
  CommandResult execute() override;
};

class ExitTargetingModeCommand final : public EngineEvent {
 public:
  ExitTargetingModeCommand(Engine& engine) : EngineEvent(engine) {}
  CommandResult execute() override;
};
}  // namespace cpprl

#endif
