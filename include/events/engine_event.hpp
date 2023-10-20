#ifndef ENGINE_EVENT_HPP
#define ENGINE_EVENT_HPP

#include "engine.hpp"

namespace cpprl {
class EngineEvent {
 protected:
  Engine& engine_;

 public:
  EngineEvent(Engine& engine) : engine_(engine) {}
  virtual ~EngineEvent() {}
  virtual void execute() = 0;
};

class NoOpEvent : public EngineEvent {
 public:
  NoOpEvent(Engine& engine) : EngineEvent(engine) {}
  void execute() override {}
};

class ResetGameCommand : public EngineEvent {
 public:
  ResetGameCommand(Engine& engine) : EngineEvent(engine) {}
  void execute() override { engine_.reset_game(); }
};

class MouseInputEvent final : public EngineEvent {
 private:
  Vector2D position_;

 public:
  MouseInputEvent(Engine& engine, Vector2D position)
      : EngineEvent(engine), position_(position) {}
  void execute() override;
};

class MouseClickEvent final : public EngineEvent {
 private:
  Vector2D position_;

 public:
  MouseClickEvent(Engine& engine, Vector2D position)
      : EngineEvent(engine), position_(position) {}
  void execute() override;
};

class ExitTargetingModeCommand final : public EngineEvent {
 public:
  ExitTargetingModeCommand(Engine& engine) : EngineEvent(engine) {}
  void execute() override;
};
}  // namespace cpprl

#endif
