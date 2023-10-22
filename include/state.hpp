#ifndef STATE_HPP
#define STATE_HPP

#include <SDL2/SDL.h>

#include "components.hpp"
#include "input_handler.hpp"
#include "types/engine_fwd.hpp"
#include "types/state_result.hpp"

namespace cpprl {
class Renderer;
class UiWindow;
class State {
 protected:
  World& world_;
  std::unique_ptr<EventHandler> input_handler_;

 public:
  State(World& world) : world_(world), input_handler_(nullptr){};
  virtual ~State() = default;
  virtual void on_enter(){};
  virtual StateResult on_update(SDL_Event& sdl_event) = 0;
  virtual void render(Renderer&){};
  virtual void on_exit(){};
};

class InGameState final : public State {
 public:
  InGameState(World& world) : State(world) {}
  void on_enter() override;
  StateResult on_update(SDL_Event& sdl_event) override;
  void render(Renderer& renderer) override;
  void on_exit() override;
};

class PickTileAOEState final : public State {
 private:
  std::function<void()> on_pick_;
  float max_radius_;

 public:
  PickTileAOEState(
      World& world, std::function<void()> on_pick, float max_radius)
      : State(world), on_pick_(on_pick), max_radius_(max_radius) {}
  void on_enter() override;
  StateResult on_update(SDL_Event& sdl_event) override;
  void render(Renderer& renderer) override;
  void on_exit() override;
};

class GuiViewState : public State {
 protected:
  UiWindow* window_;

 public:
  GuiViewState(World& world, UiWindow* window)
      : State(world), window_(window) {}
  void on_enter() override;
  StateResult on_update(SDL_Event& sdl_event) override = 0;
  void render(Renderer& renderer) override;
  void on_exit() override = 0;
};

class ViewMessageHistoryState final : public GuiViewState {
 public:
  ViewMessageHistoryState(World& world, UiWindow* window)
      : GuiViewState(world, window) {}
  void on_enter() override;
  StateResult on_update(SDL_Event& sdl_event) override;
  void render(Renderer& renderer) override;
  void on_exit() override;
};

class ViewInventoryState final : public GuiViewState {
 public:
  ViewInventoryState(World& world, UiWindow* window)
      : GuiViewState(world, window) {}
  void on_enter() override;
  StateResult on_update(SDL_Event& sdl_event) override;
  void render(Renderer& renderer) override;
  void on_exit() override;
};

}  // namespace cpprl

#endif
