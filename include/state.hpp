#ifndef STATE_HPP
#define STATE_HPP

#include <SDL2/SDL.h>

#include "components.hpp"
#include "input_handler.hpp"
#include "types/engine_fwd.hpp"
#include "types/state_result.hpp"

namespace cpprl {
class State {
 public:
  virtual ~State() = default;
  virtual void on_enter() = 0;
  virtual CommandResult on_update(SDL_Event& sdl_event) = 0;
  virtual void on_exit() = 0;
};

class InGameState final : public State {
 private:
  Engine& engine_;
  std::unique_ptr<EventHandler> input_handler_;

 public:
  InGameState(World& engine) : engine_(engine), input_handler_(nullptr) {}
  void on_enter() override;
  CommandResult on_update(SDL_Event& sdl_event) override;
  void on_exit() override;
};

class PickTileAOEState final : public State {
 private:
  std::function<void()> on_pick_;
  float max_radius_;

 public:
  PickTileAOEState(std::function<void()> on_pick, float max_radius)
      : on_pick_(on_pick), max_radius_(max_radius) {}
  void on_enter() override;
  CommandResult on_update(SDL_Event& sdl_event) override;
  void on_exit() override;
};

}  // namespace cpprl

#endif
