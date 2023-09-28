#ifndef COMMAND_H
#define COMMAND_H

#include "../engine.hpp"
#include "../game_entity.hpp"
#include "../types/map.hpp"
#include "engine_event.hpp"

namespace cpprl {
class Command : public EngineEvent {
 protected:
  GameEntity& entity_;

 public:
  Command(Engine& engine, GameEntity& entity) : EngineEvent(engine), entity_(entity) {}
  virtual void execute() = 0;
};
}  // namespace cpprl
#endif
