#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include <SDL2/SDL.h>

#include <vector>

#include "events/directional_command.hpp"
#include "events/engine_event.hpp"
#include "events/quit_command.hpp"

namespace cpprl {

class Engine;
class EngineEvent;

class InputHandler {
 private:
  QuitCommand* quitCommand;
  EngineEvent* noop;

 public:
  InputHandler(Engine& engine) : engine_(engine) {
    quitCommand = new QuitCommand(engine_);
    noop = new QuitCommand(engine_);
  };
  virtual ~InputHandler() {
    delete quitCommand;
    delete noop;
  };
  virtual EngineEvent* handle_input(SDL_Keycode key);

 protected:
  Engine& engine_;
};

class GameInputHandler : public InputHandler {
 private:
  DirectionalCommand* buttonRight;
  DirectionalCommand* buttonLeft;
  DirectionalCommand* buttonUp;
  DirectionalCommand* buttonDown;
  DirectionalCommand* buttonUpRight;
  DirectionalCommand* buttonUpLeft;
  DirectionalCommand* buttonDownRight;
  DirectionalCommand* buttonDownLeft;

 public:
  GameInputHandler(Engine& engine) : InputHandler(engine) {
    buttonRight = new DirectionalCommand(engine_, engine_.get_player(), Vector2D{1, 0});
    buttonLeft = new DirectionalCommand(engine_, engine_.get_player(), Vector2D{-1, 0});
    buttonUp = new DirectionalCommand(engine_, engine_.get_player(), Vector2D{0, -1});
    buttonDown = new DirectionalCommand(engine_, engine_.get_player(), Vector2D{0, 1});
    buttonUpRight = new DirectionalCommand(engine_, engine_.get_player(), Vector2D{1, -1});
    buttonUpLeft = new DirectionalCommand(engine_, engine_.get_player(), Vector2D{-1, -1});
    buttonDownRight = new DirectionalCommand(engine_, engine_.get_player(), Vector2D{1, 1});
    buttonDownLeft = new DirectionalCommand(engine_, engine_.get_player(), Vector2D{-1, 1});
  };
  ~GameInputHandler() {
    delete buttonRight;
    delete buttonLeft;
    delete buttonUp;
    delete buttonDown;
    delete buttonUpRight;
    delete buttonUpLeft;
    delete buttonDownRight;
    delete buttonDownLeft;
  };
  virtual EngineEvent* handle_input(SDL_Keycode key) override;
};

class MenuInputHandler : public InputHandler {
 private:
  ResetGameCommand* resetGameCommand;

 public:
  MenuInputHandler(Engine& engine) : InputHandler(engine) { resetGameCommand = new ResetGameCommand(engine_); };
  ~MenuInputHandler() { delete resetGameCommand; };
  virtual EngineEvent* handle_input(SDL_Keycode key) override;
};

}  // namespace cpprl
#endif
