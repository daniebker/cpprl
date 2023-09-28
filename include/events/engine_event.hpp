#pragma once

#include "../engine.hpp"

namespace cpprl {
class EngineEvent {
 protected:
  Engine& engine_;

 public:
  EngineEvent(Engine& engine) : engine_(engine) {}
  virtual void execute() = 0;
};

class NoOpEvent : public EngineEvent {
 public:
  NoOpEvent(Engine& engine) : EngineEvent(engine) {}
  void execute() override {}
};
}  // namespace cpprl
