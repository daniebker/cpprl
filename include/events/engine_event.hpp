#pragma once

#include "../engine.hpp"

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
}  // namespace cpprl
