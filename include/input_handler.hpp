#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include <SDL2/SDL.h>

#include <vector>

#include "events/close_view_command.hpp"
#include "events/directional_command.hpp"
#include "events/engine_event.hpp"
#include "events/quit_command.hpp"
#include "events/view_history_command.hpp"
#include "globals.hpp"

namespace cpprl {

class Engine;
class EngineEvent;

class InputHandler {
 public:
  InputHandler(Engine& engine)
      : engine_(engine), noop(engine), quitCommand(engine){};
  virtual EngineEvent& handle_input(SDL_Event event);

 protected:
  Engine& engine_;
  NoOpEvent noop;

 private:
  QuitCommand quitCommand;
};

class GameInputHandler final : public InputHandler {
 private:
  DirectionalCommand buttonRight;
  DirectionalCommand buttonUp;
  DirectionalCommand buttonDown;
  DirectionalCommand buttonUpRight;
  DirectionalCommand buttonUpLeft;
  DirectionalCommand buttonLeft;
  DirectionalCommand buttonDownRight;
  DirectionalCommand buttonDownLeft;
  ViewHistoryCommand viewHistoryCommand;

 public:
  GameInputHandler(Engine& engine, GameEntity& controllableEntity)
      : InputHandler(engine),
        buttonRight(engine_, controllableEntity, Vector2D{1, 0}),
        buttonUp(engine_, controllableEntity, Vector2D{0, -1}),
        buttonDown(engine_, controllableEntity, Vector2D{0, 1}),
        buttonUpRight(engine_, controllableEntity, Vector2D{1, -1}),
        buttonUpLeft(engine_, controllableEntity, Vector2D{-1, -1}),
        buttonLeft(engine_, controllableEntity, Vector2D{-1, 0}),
        buttonDownRight(engine_, controllableEntity, Vector2D{1, 1}),
        buttonDownLeft(engine_, controllableEntity, Vector2D{-1, 1}),
        viewHistoryCommand(engine_){};

  virtual EngineEvent& handle_input(SDL_Event event) override;
};

class MenuInputHandler final : public InputHandler {
 private:
  ResetGameCommand resetGameCommand;

 public:
  MenuInputHandler(Engine& engine)
      : InputHandler(engine), resetGameCommand(engine){};
  virtual EngineEvent& handle_input(SDL_Event event) override;
};

class HistoryViewInputHandler final : public InputHandler {
 private:
  CloseViewCommand closeViewCommand_;

 public:
  HistoryViewInputHandler(Engine& engine)
      : InputHandler(engine), closeViewCommand_(engine){};
  virtual EngineEvent& handle_input(SDL_Event event) override;
};

}  // namespace cpprl
#endif
