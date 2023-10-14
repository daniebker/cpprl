#ifndef INCLUDE_DIE_EVENT_HPP_
#define INCLUDE_DIE_EVENT_HPP_

#include "engine.hpp"
#include "events/engine_event.hpp"
#include "game_entity.hpp"

namespace cpprl {
class DieEvent : public EngineEvent {
 public:
  DieEvent(Engine& engine, Entity& entity)
      : EngineEvent(engine), entity_(entity) {}
  virtual ~DieEvent() = default;
  virtual void execute() override;

 private:
  Entity& entity_;
};
}  // namespace cpprl

#endif
