#ifndef CLOSE_VIEW_COMMAND_HPP
#define CLOSE_VIEW_COMMAND_HPP

#include "engine.hpp"
#include "events/engine_event.hpp"

namespace cpprl {
class CloseViewCommand final : public EngineEvent {
 public:
  CloseViewCommand(Engine& engine) : EngineEvent(engine){};
  virtual CommandResult execute() override;
};
}  // namespace cpprl

#endif  // CLOSE_VIEW_COMMAND_HPP
