#ifndef QUIT_COMMAND_HPP
#define QUIT_COMMAND_HPP
#include "engine_event.hpp"

namespace cpprl {
class QuitCommand : public EngineEvent {
 public:
  QuitCommand(Engine& engine) : EngineEvent(engine){};
  virtual CommandResult execute();
};
}  // namespace cpprl

#endif  // QUIT_COMMAND_HPP
