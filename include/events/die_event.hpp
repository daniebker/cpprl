#ifndef INCLUDE_DIE_EVENT_HPP_
#define INCLUDE_DIE_EVENT_HPP_

#include "../engine.hpp"
#include "../game_entity.hpp"
#include "engine_event.hpp"

namespace cpprl {
class DieEvent : public EngineEvent {
 public:
  DieEvent(Engine& engine, GameEntity& entity) : EngineEvent(engine), entity_(entity) {}
  virtual ~DieEvent() = default;
  virtual void execute() override;

 private:
  GameEntity& entity_;
};
}  // namespace cpprl

#endif
