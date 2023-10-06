#ifndef VIEW_HISTORY_COMMAND_HPP
#define VIEW_HISTORY_COMMAND_HPP

#include "events/engine_event.hpp"

namespace cpprl {

class ViewHistoryCommand : public EngineEvent {
 public:
  ViewHistoryCommand(Engine& engine) : EngineEvent(engine){};
  virtual void execute();
};
}  // namespace cpprl

#endif
