#ifndef SCROLLCOMMAND_HPP
#define SCROLLCOMMAND_HPP

#include "engine.hpp"
#include "engine_event.hpp"

namespace cpprl {
class ScrollCommand : public EngineEvent {
 public:
  ScrollCommand(Engine& engine, int scroll_amount)
      : EngineEvent(engine), scroll_amount_(scroll_amount){};
  virtual void execute();

 private:
  int scroll_amount_;
};
}  // namespace cpprl
#endif
