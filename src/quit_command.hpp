#include "engine_event.hpp"

namespace cpprl {
class QuitCommand : public EngineEvent {
 public:
  QuitCommand(Engine& engine) : EngineEvent(engine){};
  virtual void execute();
};
}  // namespace cpprl
