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
  virtual StateResult on_update(SDL_Event& sdl_event);
  virtual void render(Renderer&){};
  virtual void on_exit(){};
};

class InGameState final : public State {
 public:
  InGameState(World& world) : State(world) {}
  void on_enter() override;
};

class NextLevelState final : public State {
 public:
  NextLevelState(World& world) : State(world) {}
  void on_enter() override;
  StateResult on_update(SDL_Event& sdl_event) override;
};

class PickTileState : public State {
 protected:
  int max_range_;
  std::function<void()> on_pick_;

 public:
  PickTileState(World& world, std::function<void()> on_pick, int max_range)
      : State(world), max_range_(max_range), on_pick_(on_pick) {}
  ~PickTileState() override = default;

  void on_enter() override;
  StateResult on_update(SDL_Event& sdl_event) override;
  void render(Renderer& renderer) override;
  void on_exit() override;
};

class PickTileAOEState final : public PickTileState {
 private:
  int aoe_;
  int aoe_squared_;

 public:
  PickTileAOEState(
      World& world, std::function<void()> on_pick, int max_range_, int aoe)
      : PickTileState(world, on_pick, max_range_),
        aoe_(aoe),
        aoe_squared_(aoe_ * aoe_) {}
  void render(Renderer& renderer) override;
};

class GuiViewState : public State {
 protected:
  UiWindow* window_;

 public:
  GuiViewState(World& world, UiWindow* window)
      : State(world), window_(window) {}
  void render(Renderer& renderer) override;
};

class ViewMessageHistoryState final : public GuiViewState {
 public:
  ViewMessageHistoryState(World& world, UiWindow* window)
      : GuiViewState(world, window) {}
  void on_enter() override;
};

class ViewInventoryState final : public GuiViewState {
 public:
  using GuiViewState::GuiViewState;
  void on_enter() override;
};

class GameOverState final : public GuiViewState {
 public:
  GameOverState(World& world) : GuiViewState(world, nullptr) {}
  void on_enter() override;
};

class MainMenuState final : public GuiViewState {
 public:
  MainMenuState(World& world, UiWindow* window) : GuiViewState(world, window) {}
  ~MainMenuState() { delete window_; }
  void on_enter() override;
};  // namespace cpprl

}  // namespace cpprl

#endif
